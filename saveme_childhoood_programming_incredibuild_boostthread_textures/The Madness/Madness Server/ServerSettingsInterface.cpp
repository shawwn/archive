#include "ServerSettingsInterface.h"

ServerSettingsInterface gSettings;


void ServerSettingsInterface::SetDefaults()
{
	ListenPort = 31337;
}

bool ServerSettingsInterface::SetApplicationVariable(const std::string &Variable, const std::string &Value)
{
	AddWord(Variable,"LISTENPORT",ListenPort,Value);

	return true;
}