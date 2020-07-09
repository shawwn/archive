#include "stdafx.h"
#include "BasicNetworking.h"

BasicNetworking::BasicNetworking()
{
	mConfigurationFile = "Networking.cfg";
}

BasicNetworking::~BasicNetworking()
{
}

string NetworkIPToString(DoubleWord NetIP)
{
	ostringstream stream;
	stream << (NetIP & 0x000000FF) << "." << ((NetIP & 0x0000FF00) >> 8) << "." << ((NetIP & 0x00FF0000) >> 16) << "." << ((NetIP & 0xFF000000) >> 24);
	return stream.str();
}