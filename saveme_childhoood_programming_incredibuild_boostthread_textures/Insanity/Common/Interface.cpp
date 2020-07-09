#include "Interface.h"
#include "Globals.h"
#include "LoggingInterface.h"

Interface::Interface(const std::string &Name, ResourceType Type) : Resource(Name, Type)
{
}

Interface::~Interface()
{
}