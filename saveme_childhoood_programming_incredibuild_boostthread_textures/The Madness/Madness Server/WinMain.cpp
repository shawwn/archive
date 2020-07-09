
#define NOMINMAX
#include "ServerInterface.h"
#include "GameInterface.h"
#include "ClientSettingsInterface.h"
#include "LoggingInterface.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR, INT)
{
	gLog.SetUseCout(false);
	gLog.SetOutputFile("Game.log");
	gSettings.Load("../Madness/Settings.cfg");
	gServer.Run(31337);
	
	return 0;
}