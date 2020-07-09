#include "Engine.h"
#include "Globals.h"
#include "LoggingInterface.h"

Engine::Engine(const std::string &Name, ResourceType Type) : Interface(Name, Type)
{
}

Engine::~Engine()
{
}