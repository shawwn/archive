#include "LoginHandler.h"
#include "LoginServer.h"

LoginHandler::LoginHandler()
{
	mNextGUID = 0x0000000000030001ull;
}

LoginHandler::~LoginHandler()
{

}

void LoginHandler::Register()
{
	gServer->RegisterHandler(CMSG_AUTH_SESSION, (PacketHandler *)this);
	gServer->RegisterHandler(CMSG_PICK_WORLDSERVER, (PacketHandler *)this);
	gServer->RegisterHandler(SMSG_PREP_CONNECTION, (PacketHandler *)this);
}


bool LoginHandler::Handle(Packet *packet, Client *cli)
{
	switch(packet->GetOpCode())
	{
	case CMSG_AUTH_SESSION:
		HandleCMSG_AUTH_SESSION(packet,cli);
		return true;
	case CMSG_PICK_WORLDSERVER:
		HandleCMSG_PICK_WORLDSERVER(packet,cli);
		return true;
	case SMSG_PREP_CONNECTION:
		HandleSMSG_PREP_CONNECTION(packet,cli);
		return true;
	}

	return false;
}

bool LoginHandler::HandleCMSG_AUTH_SESSION(Packet *packet, Client *cli)
{
	Account *acct;
	Packet pack;
	int Offset = 0;
	Word version = packet->GetWord(0);
	Offset += sizeof(Word);
	string Username = (char *)packet->GetBytes(Offset);
	Offset += (int)Username.length()+1;
	string Password = (char *)packet->GetBytes(Offset);
	Offset += (int)Password.length()+1;
	ConfigParameter *param = gServer->Configuration().Find("WorldServerLoginUsername");
	if(!param)
	{
		gLog->Stream << "Login Server: You must specify a WorldServerLoginUsername in the configuration file.";
		gLog->Print();
		gServer->Stop();
	}
	string WorldServerLoginUsername = param->Arguments[0];
	param = gServer->Configuration().Find("WorldServerLoginPassword");
	if(!param)
	{
		gLog->Stream << "Login Server: You must specify a WorldServerLoginPassword in the configuration file.";
		gLog->Print();
		gServer->Stop();
	}
	string WorldServerLoginPassword = param->Arguments[0];

	if(Username == WorldServerLoginUsername && Password == WorldServerLoginPassword)
	{
		string WorldServerName = (char *)packet->GetBytes(Offset);
		Offset += (int)WorldServerName.length()+1;
		DoubleWord WorldServerIP = packet->GetDoubleWord(Offset);
		Offset += sizeof(DoubleWord);
		Word WorldServerListenPort = packet->GetWord(Offset);
		Offset += sizeof(Word);
		gServer->TransferToWorldClient(cli,WorldServerName,WorldServerIP,WorldServerListenPort);

		gLog->Stream << "Client is a WorldServer.  Worldserver " << WorldServerName << ": Connected from IP " << cli->GetIP();
		gLog->Print();
		return true;
	}

	if(gServer->WorldCount() == 0)
	{
		gLog->Stream << "Client tried to connect with username " << Username << " and password " << Password << ", but there are no world servers. ";
		gLog->Print();
			
		packet->Build(SMSG_AUTH_RESPONSE);
		packet->AddByte(07);
		gServer->WriteData(cli,packet);
		cli->CloseWhenClear();

		return false;
	}

	gLog->Stream << "Client connecting with username " << Username << " and password " << Password;
	gLog->Print();

	if(Username.length() < 3) {
		gLog->Print("Username too short.");
		packet->Build(SMSG_AUTH_RESPONSE);
		packet->AddByte(02);
		gServer->WriteData(cli,packet);
		cli->CloseWhenClear();
		return false;
	}

	acct = gServer->Accounts().Find(Username.c_str());
	if(!acct)
	{
		acct = new Account;
		acct->SetName(Username.c_str());
		acct->SetPassword(Password.c_str());
		gServer->Accounts().Add(acct);
		gLog->Print("Account created.");
	}
	else if(acct->GetPassword() != Password)
	{
		gLog->Print("Invalid password.");
		packet->Build(SMSG_AUTH_RESPONSE);
		packet->AddByte(03);
		gServer->WriteData(cli,packet);
		cli->CloseWhenClear();
		return false;
	}
	if(version != 1)
	{
		packet->Build(SMSG_AUTH_RESPONSE);
		packet->AddByte(04);
		gServer->WriteData(cli,packet);
		gLog->Print("Wrong version.");
		cli->CloseWhenClear();
		return false;
	}

	if(acct->GetClient())
	{
		// Already connected
		pack.Build(SMSG_AUTH_RESPONSE);
		pack.AddByte(05);
		gServer->WriteData(cli,&pack);
		gLog->Print("Already connected.");
		return false;
	}

	acct->SetClient(cli);

	pack.Build(SMSG_AUTH_RESPONSE);
	pack.AddByte(0x01);
	pack.AddByte((Byte)gServer->WorldCount());
	
	for(LoginServer::WorldIterator it = gServer->Begin(); it != gServer->End(); it++)
	{
		pack.AddDoubleWord((*it)->GetNetworkIP());
		pack.AddWord((*it)->GetPort());
		pack.AddBytes((Byte *)(*it)->GetName().c_str(),(Word)(*it)->GetName().length()+1);
	}

	gServer->WriteData(cli,&pack);

	gLog->Stream << "Account " << Username << " connected.";
	gLog->Print();

	return true;
}

bool LoginHandler::HandleCMSG_PICK_WORLDSERVER(Packet *packet, Client *cli)
{
	Account *acct = gServer->Accounts().Find(cli);
	Packet pack;
	if(!acct) {
		gLog->Stream << "Client " << cli->GetIP() << " tried to pick a worldserver, but he was not authorized.";
		gLog->Print();
		
		pack.Build(SMSG_PICK_WORLDSERVER);
		pack.AddByte(2);
		gServer->WriteData(cli,&pack);
		return false;
	}

	int Offset = 0;
    DoubleWord IP = packet->GetDoubleWord(Offset);
	Offset += sizeof(DoubleWord);
	Word Port = packet->GetWord(Offset);
	Offset += sizeof(Word);

	for(LoginServer::WorldIterator it = gServer->Begin(); it != gServer->End(); it++)
	{
		if((*it)->GetNetworkIP() == IP && (*it)->GetPort())
		{
			pack.Build(CMSG_PREP_CONNECTION);
			pack.AddBytes((Byte *)acct->GetName().c_str(),(int)acct->GetName().length()+1);
			pack.AddDoubleWord(cli->GetNetworkIP());
			gServer->WriteData((*it),&pack);
			return true;
		}
	}

	gLog->Stream << "Client " << cli->GetIP() << " tried to pick a worldserver with the IP " << NetworkIPToString(IP) << " and port " << Port << ", but there are no worldservers matching.";
	gLog->Print();

	pack.Build(SMSG_PICK_WORLDSERVER);
	pack.AddByte(3);
	gServer->WriteData(cli,&pack);
	cli->CloseWhenClear();
	return true;
}

bool LoginHandler::HandleSMSG_PREP_CONNECTION(Packet *packet, Client *cli)
{
	for(LoginServer::WorldIterator it = gServer->Begin(); it != gServer->End(); it++)
	{
		if((*it)->GetSocket() == cli->GetSocket())
			break;
	}

	if(it == gServer->End()) {
		gLog->Stream << "LoginServer: SMSG_RPEP_CONNECTION was recieved from client " << cli->GetIP() << ", but client is not a worldserver.";
		gLog->Print();
		return false;
	}

	WorldClient *srv = (*it);

	Word Offset = 0;
	Byte b = packet->GetByte(Offset);
	Offset += sizeof(Byte);
	string Accountname = (char *)packet->GetBytes(Offset);
	Offset += (int)Accountname.length()+1;

	Account *acct = gServer->Accounts().Find(Accountname.c_str());
	if(!acct)
	{
		gLog->Stream << "LoginServer: WorldServer " << cli->GetIP() << " prepared an account that is not active.";
		gLog->Print();
		return false;
	}

	Client *usr = acct->GetClient();
	if(!usr)
	{
		gLog->Stream << "LoginServer: WorldServer " << cli->GetIP() << " prepared an account that does not have an active connection.";
		gLog->Print();
		return false;
	}

	if(b != 1)
	{
		gLog->Stream << "LoginServer: SMSG_PREP_CONNECTION recieved from " << cli->GetIP() << " with accountname " << Accountname << " and the byteflag was not 1.";
		gLog->Print();
	
		packet->Build(SMSG_PICK_WORLDSERVER);
		packet->AddByte(04);
		gServer->WriteData(usr,packet);
		return false;
	}

	packet->Build(SMSG_PICK_WORLDSERVER);
	packet->AddByte(01);
	packet->AddDoubleWord(srv->GetNetworkIP());
	packet->AddWord(srv->GetPort());
	gServer->WriteData(usr,packet);

	return true;
}

/*

bool LoginHandler::HandleCMSG_CHAR_CREATE(Packet *packet, Client *cli)
{
	string plyname;
	Byte retcode;
	size_t i;
	Account *acct;
	Player *ply;
	
	plyname = (char *)packet->GetBytes(0);

	retcode = 1;
	if((int)plyname.length() < 3)
		retcode = 2;
	else if((int)plyname.length() > 20)
		retcode = 3;
	else
	{
		for(i = 0; i < plyname.length(); i++)
		{
			if(plyname[i] >= 'a' && plyname <= 'z')
				continue;
			if(plyname[i] >= 'A' && plyname <= 'Z')
				continue;
		}
	}

	if(retcode != 1)
	{
		gLog->Stream << "Couldn't create character " << plyname;
		gLog->Print();

		packet->Build(SMSG_CHAR_CREATE);
		packet->AddByte(retcode);
		gServer->WriteData(cli,packet);
		return true;
	}

	if(gServer->Players().Find(plyname.c_str()))
	{
		gLog->Stream << "Character already created: " << plyname;
		gLog->Print();

		packet->Build(SMSG_CHAR_CREATE);
		packet->AddByte(4); // Already taken
		gServer->WriteData(cli,packet);
		return true;
	}
	
	acct = gServer->Accounts().Find(cli);
	if(!acct)
		return false;

	ply = new Player;
	ply->SetGuid(mNextGUID++);
	ply->SetName(plyname);
	ply->SetAccount(acct);

	gServer->Players().Add(ply);
	acct->Players().Add(ply);
	
	packet->Build(SMSG_CHAR_CREATE);
	packet->AddByte(1);
	gServer->WriteData(cli,packet);

	gLog->Stream << "Account " << acct->GetName() << " created character " << ply->GetName();
	gLog->Print();

	return true;
}

bool LoginHandler::HandleCMSG_CHAR_ENUM(Packet *packet, Client *cli)
{
	Account *acct;
	PlayerManager::Iterator i;
	Player *ply;

	acct = gServer->Accounts().Find(cli);
	if(!acct)
		return false;

	packet->Build(SMSG_CHAR_ENUM);
	packet->AddByte((Byte)acct->Players().Count());

	for(i = acct->Players().Begin(); i != acct->Players().End(); i++)
	{
		ply = (*i).second;
		packet->AddQuadWord(ply->GetGuid());
		packet->AddBytes((Byte *)ply->GetName().c_str(), (DoubleWord)ply->GetName().length() + 1);
	}

	gServer->WriteData(cli,packet);

	return true;
}

bool LoginHandler::HandleCMSG_CHAR_DELETE(Packet *packet, Client *cli)
{
	Account *acct;
	Player *ply;

	acct = gServer->Accounts().Find(cli);
	if(!acct) {
		gLog->Stream << "Account doesn't exit.";
		gLog->Print();
		packet->Build(SMSG_CHAR_DELETE);
		packet->AddByte(1);
		gServer->WriteData(cli,packet);
		return false;
	}

	ply = acct->Players().Find(packet->GetQuadWord(0x00));
	if(!ply) {
		gLog->Stream << "Account " << acct->GetName() << " couldn't delete player.  Doesn't exit.";
		gLog->Print();
		packet->Build(SMSG_CHAR_DELETE);
		packet->AddByte(2);
		gServer->WriteData(cli,packet);
		return false;
	}

	gLog->Stream << "Account " << acct->GetName() << " deleted character " << ply->GetName();
	gLog->Print();

	gServer->Players().Remove(ply);
	acct->Players().Remove(ply);
	delete ply;

	packet->Build(SMSG_CHAR_DELETE);
	packet->AddByte(0);
	gServer->WriteData(cli,packet);

	return true;
}

bool LoginHandler::HandleCMSG_CHAR_PICK(Packet *packet, Client *cli)
{
	if(!gServer->WorldCount() == 0) 
		return false;
	
//	Client *WorldServer = gServer->GetWorldServer();

//	packet->Build(CMSG_PREPARE_CHARACTER);
//	packet->AddBytes

	return true;
}
*/
