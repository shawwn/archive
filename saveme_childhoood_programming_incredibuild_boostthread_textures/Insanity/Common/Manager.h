#ifndef MANAGER_H
#define MANAGER_H

#include "Resource.h"

class Manager : public Resource
{
public:
	Manager(const std::string &Name = "Manager Object", ResourceType Type = MANAGER);
	virtual ~Manager();
};

#endif