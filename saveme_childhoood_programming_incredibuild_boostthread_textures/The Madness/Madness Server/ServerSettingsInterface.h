#ifndef SERVERSETTINGSINTERFACE_H
#define SERVERSETTINGSINTERFACE_H

#include "SettingsInterface.h"

class ServerSettingsInterface : public SettingsInterface
{
private:
	void SetDefaults();
	bool SetApplicationVariable(const std::string &Variable, const std::string &Value);
public:
	Word ListenPort;
};

extern ServerSettingsInterface gSettings;

#endif