#include <iostream>
using namespace std;

#include "LoginServer.h"

int main()
{
	gLog = new LoggingInterface;
	gLog->SetOutputFile("Server.log");
	gLog->Print("Session start.");

	gServer = new LoginServer;
	gServer->Run();

	delete gServer;
	delete gLog;

	return 0;
}
