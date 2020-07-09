#include "dxstdafx.h"
#include "LoginHandler.h"
#include "ClientNetworking.h"
#include "Engine.h"
#include "LoggingInterface.h"

void LoginHandler::Register()
{
	gNetworking->RegisterHandler(SMSG_AUTH_RESPONSE,(PacketHandler *)this);
	gNetworking->RegisterHandler(SMSG_PICK_WORLDSERVER,(PacketHandler *)this);
	gNetworking->RegisterHandler(SMSG_AUTH_WORLD,(PacketHandler *)this);
}

bool LoginHandler::Handle(Packet *packet, Client *cli)
{
	switch(packet->GetOpCode())
	{
	case SMSG_AUTH_RESPONSE:
		HandleSMSG_AUTH_RESPONSE(packet,cli);
		return true;
	case SMSG_PICK_WORLDSERVER:
		HandleSMSG_PICK_WORLDSERVER(packet,cli);
		return true;
	case SMSG_AUTH_WORLD:
		HandleSMSG_AUTH_WORLD(packet,cli);
		return true;
	}

	return false;
}

bool LoginHandler::HandleSMSG_AUTH_RESPONSE(Packet *packet, Client *cli)
{
	int Offset = 0;
	Byte b = packet->GetByte(Offset);
	Offset += sizeof(Byte);

	switch(b)
	{
	case 2: // Username Invalid
		gLog->Print("Username invalid.");
		return false;
	case 3: // Password Invalid
		gLog->Print("Password invalid.");
		return false;
	case 4: // Bad Version
		gLog->Print("Invalid client version.");
		return false;
	case 5: // Already Logged In
		gLog->Print("That account is already logged in.");
		return false;
	case 6: // Banned
		gLog->Print("You have been banned.");
		return false;
	case 7: // No Worldservers to connect to
		gLog->Print("All worldservers are down.  Please connect later.");
		return false;
	case 1:
		break;
	default:
		return false;
	}
	
	Byte NumWorldservers = packet->GetByte(Offset);
	Offset += sizeof(Byte);

	DoubleWord IP;
	Word Port;
	for(int i = 0; i < NumWorldservers; i++)
	{
		IP = packet->GetDoubleWord(Offset);
		Offset += sizeof(DoubleWord);
		Port = packet->GetWord(Offset);
		Offset += sizeof(Word);
		string Name = (char *)packet->GetBytes(Offset);
		Offset += (int)Name.length()+1;

		gLog->Stream << "Worldserver " << Name << ": IP " << NetworkIPToString(IP) << " and Port " << Port;
		gLog->Print();
	}

    Packet pack;
	pack.Build(CMSG_PICK_WORLDSERVER);
	pack.AddDoubleWord(IP);
	pack.AddWord(Port);

	gNetworking->WriteData(&pack);

	return true;
}

bool LoginHandler::HandleSMSG_PICK_WORLDSERVER(Packet *packet, Client *cli)
{
	Word Offset = 0;
	Byte b = packet->GetByte(Offset);
	Offset += sizeof(Byte);
	// byte (Worldserver prepared connection?)
	//  01 - Prepared
	//  02 - Client not authorized
	//  03 - No worldserver with that IP and port
	//  04 - Worldserver could not prepare connection
	switch(b)
	{
	case 1:
		break;
	case 2:
		gLog->Stream << "Worldserver authorization failed.";
		gLog->Print();
		return false;
	case 3:
		gLog->Stream << "Worldserver at that IP and port is down.  Please try again later.";
		gLog->Print();
		return false;
	case 4:
		gLog->Stream << "Worldserver could not prepare client connection.  Please try again.";
		gLog->Print();
		return false;
	}

	DoubleWord IP = packet->GetDoubleWord(Offset);
	Offset += sizeof(DoubleWord);
	Word Port = packet->GetWord(Offset);
	Offset += sizeof(Word);

	gNetworking->ConnectToWorldServer(IP,Port,gNetworking->GetUsername());

	return true;
}

bool LoginHandler::HandleSMSG_AUTH_WORLD(Packet *packet, Client *cli)
{
    Byte b = packet->GetByte(0);
	switch(b)
	{
	case 1:
		gLog->Stream << "Connected to worldserver " << cli->GetIP();
		gLog->Print();
		return true;
	case 2:
		gLog->Stream << "Could not authorize session with worldserver.  Please try again.";
		gLog->Print();
		return true;
	}

	return false;
}