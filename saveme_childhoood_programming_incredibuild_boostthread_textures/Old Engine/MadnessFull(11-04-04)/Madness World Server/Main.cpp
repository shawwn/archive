#include <iostream>
using namespace std;
#include "WorldServer.h"

#ifdef WIN32
#pragma comment(lib,"Networking.lib")
#endif

int main()
{
	gLog = new LoggingInterface;
	gLog->SetOutputFile("Server.log");
	gLog->Print("Session start.");

	gServer = new WorldServer;
	gServer->Run();

	delete gServer;
	delete gLog;

	return 0;
}
