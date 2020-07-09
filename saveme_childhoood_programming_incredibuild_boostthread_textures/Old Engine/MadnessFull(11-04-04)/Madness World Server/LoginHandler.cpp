#include "Loginhandler.h"
#include "WorldServer.h"

void LoginHandler::Register()
{
	gServer->RegisterHandler(CMSG_PREP_CONNECTION,(PacketHandler *)this);
	gServer->RegisterHandler(CMSG_AUTH_WORLD,(PacketHandler *)this);
}

bool LoginHandler::Handle(Packet *packet, Client *cli)
{
	switch(packet->GetOpCode())
	{
	case CMSG_PREP_CONNECTION:
		HandleCMSG_PREP_CONNECTION(packet,cli);
		return true;
	case CMSG_AUTH_WORLD:
		HandleCMSG_AUTH_WORLD(packet,cli);
		return true;
	}

	return false;
}

bool LoginHandler::HandleCMSG_PREP_CONNECTION(Packet *packet, Client *cli)
{
	if(!gServer->LoginServer())
		return false;

	if(cli->GetSocket() != gServer->LoginServer()->GetSocket())
		return false;

	Packet pack;
	Word Offset = 0;
	string Username = (char *)packet->GetBytes(Offset);
	Offset += (int)Username.length()+1;
	DoubleWord IP = packet->GetDoubleWord(Offset);
	Offset += sizeof(DoubleWord);

	Account *acct = gServer->Accounts().Find(Username.c_str());
	if(!acct)
	{
		acct = new Account;
		acct->SetName(Username.c_str());
		gServer->Accounts().Add(acct);
	}

	acct->SetIP(IP);

	pack.Build(SMSG_PREP_CONNECTION);
	pack.AddByte(1);
	pack.AddBytes((Byte *)Username.c_str(),(int)Username.length()+1);
	gServer->WriteData(gServer->LoginServer(),&pack);

	return true;
}

bool LoginHandler::HandleCMSG_AUTH_WORLD(Packet *packet, Client *cli)
{
	Word Offset = 0;
	string Username = (char *)packet->GetBytes(Offset);
	Offset += (int)Username.length()+1;

	Account *acc = gServer->Accounts().Find(Username.c_str());
	if(!acc)
	{
		gLog->Stream << "Client " << cli->GetIP() << " tried to connect with accountname " << Username << ", but that account doesn't exist.";
		gLog->Print();

		packet->Build(SMSG_AUTH_WORLD);
		packet->AddByte(02);
		gServer->WriteData(cli,packet);
		cli->CloseWhenClear();
		return false;
	}

	if(acc->GetIP() != cli->GetNetworkIP())
	{
		gLog->Stream << "Client " << cli->GetIP() << " tried to connect with accountname " << Username << ", but that account wasn't notified by the login server to accept connections from that IP.";
		gLog->Print();

		packet->Build(SMSG_AUTH_WORLD);
		packet->AddByte(02);
		gServer->WriteData(cli,packet);
		cli->CloseWhenClear();
		return false;
	}
	
	acc->SetClient(cli);
	gLog->Stream << "Client " << cli->GetIP() << " logged in with account " << Username;
	gLog->Print();
	packet->Build(SMSG_AUTH_WORLD);
	packet->AddByte(01);
	gServer->WriteData(cli,packet);

	return true;
}