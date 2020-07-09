#ifndef ENGINE_H
#define ENGINE_H

#include "Interface.h"
#include <string>

class Engine : public Interface
{
public:
	Engine(const std::string &Name = "Engine", ResourceType Type = ENGINE);
	virtual ~Engine();

};

#endif