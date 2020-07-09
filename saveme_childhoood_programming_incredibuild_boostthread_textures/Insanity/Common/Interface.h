#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include "Resource.h"

class Interface : public Resource
{
public:
	Interface(const std::string &Name = "Interface Class", ResourceType Type = NULLTYPE);
	virtual ~Interface();

};

#endif