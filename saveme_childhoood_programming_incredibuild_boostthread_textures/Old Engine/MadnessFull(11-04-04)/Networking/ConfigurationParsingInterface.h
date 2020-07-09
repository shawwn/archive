#ifndef _CONFIGURATIONPARSINGINTERFACE_H
#define _CONFIGURATIONPARSINGINTERFACE_H

#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;


class ConfigParameter
{
public:
	ConfigParameter() { }
	std::vector<string> Arguments;

	int ArgCount() { return (int)Arguments.size(); }
};


class ConfigurationParsingInterface
{
private:
	std::vector<string> mFile;
	std::map<string,ConfigParameter *> mParameters;

	void RemoveWhitespace(string &str);
	string UpperString(const string &str);
	void ProcessFile();
public:
	ConfigurationParsingInterface();
	~ConfigurationParsingInterface();

	bool Load(const string &Filename);
	bool IsLoaded() { if(mParameters.size() > 0) { return true; } return false; }
	ConfigParameter *Find(const string &Command);
};

#endif
