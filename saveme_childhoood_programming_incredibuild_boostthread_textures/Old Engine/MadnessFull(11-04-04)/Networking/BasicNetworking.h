#ifndef _BASICNETWORKING_H
#define _BASICNETWORKING_H

#include "ConfigurationParsingInterface.h"
#include "Types.h"
#include <string>
#include <sstream>
using namespace std;

#ifdef WIN32
#pragma comment(lib,"ws2_32.lib")
#endif


class BasicNetworking
{
protected:
	string mConfigurationFile;
	ConfigurationParsingInterface mConfiguration;
public:
	BasicNetworking();
	~BasicNetworking();

	ConfigurationParsingInterface &Config() { return mConfiguration; }
};

extern string NetworkIPToString(DoubleWord NetIP);

#endif
