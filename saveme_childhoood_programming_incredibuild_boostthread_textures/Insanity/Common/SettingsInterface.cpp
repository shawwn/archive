#include "SettingsInterface.h"
#include "LoggingInterface.h"

SettingsInterface::SettingsInterface() : Interface("SettingsInterface",INTERFACE_SETTINGS)
{
	SetDone(false);
}

SettingsInterface::~SettingsInterface()
{
}