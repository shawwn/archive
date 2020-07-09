#ifndef MADNESS_SERVER_USE_DIRECT3D
#include "ServerInterface.h"
#include "LoggingInterface.h"
#include "ServerSettingsInterface.h"

int main()
{	
	gSettings.Load("ServerSettings.cfg");
	gLog.SetOutputFile("Server.log");
	//gLog.SetUseCout(false);
	gLog.SetUseCout(true);

	gServer.Run(gSettings.ListenPort);
	
	return 0;
}
#endif