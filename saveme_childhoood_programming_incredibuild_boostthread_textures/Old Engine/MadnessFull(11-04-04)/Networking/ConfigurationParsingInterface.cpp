#include "stdafx.h"
#include "ConfigurationParsingInterface.h"
#include "LoggingInterface.h"

ConfigurationParsingInterface::ConfigurationParsingInterface()
{

}

ConfigurationParsingInterface::~ConfigurationParsingInterface()
{
	for(std::map<string,ConfigParameter *>::iterator it = mParameters.begin(); it != mParameters.end(); it++)
		delete (*it).second;
}

bool iswhite(char c)
{
	if(c == ' ')
		return true;
	return false;
}

void ConfigurationParsingInterface::RemoveWhitespace(string &str)
{
	while(str[0] == ' ')
		str = str.substr(1,str.length());
	while(str[str.length()-1] == ' ')
		str = str.substr(0,str.length()-1);
}

string ConfigurationParsingInterface::UpperString(const string &str)
{
	string ret = str;
	for(int i = 0; i < (int)ret.length(); i++)
	{
		ret[i] = toupper(ret[i]);
	}
	return ret;
}

void ConfigurationParsingInterface::ProcessFile()
{
	string ArgumentList;
	size_t pos;
	string Command;
	for(int i = 0; i < (int)mFile.size(); i++)
	{
		if(mFile[i].length() == 0)
			continue;
        pos = mFile[i].find_first_of('=');
		if(pos == std::string::npos)
			continue;
		Command = UpperString(mFile[i].substr(0,pos));
		ArgumentList = mFile[i].substr(pos+1,mFile[i].length());
		RemoveWhitespace(Command);
		RemoveWhitespace(ArgumentList);
		ConfigParameter *param = new ConfigParameter;
		pos = ArgumentList.find_first_of(' ');
		while(pos != std::string::npos)
		{
			param->Arguments.push_back(ArgumentList.substr(0,pos));
			ArgumentList.substr(pos+1,ArgumentList.length());
			pos = ArgumentList.find_first_of(' ');
		}
		param->Arguments.push_back(ArgumentList);
		if(param->ArgCount() != 0)
			mParameters[Command] = param;
		else
			delete param;
	}
	mFile.clear();
}

bool ConfigurationParsingInterface::Load(const string &Filename)
{
	ifstream in(Filename.c_str());
	if(!in.is_open())
		return false;

	for(std::map<string,ConfigParameter *>::iterator it = mParameters.begin(); it != mParameters.end(); it++)
		delete (*it).second;

	string Line;	
	do
	{
		getline(in,Line,'\n');
		mFile.push_back(Line);
	} while(!in.eof());

	in.close();

	ProcessFile();

	return true;
}

ConfigParameter *ConfigurationParsingInterface::Find(const string &Command)
{
	std::map<string,ConfigParameter *>::iterator it = mParameters.find(UpperString(Command));
	if(it != mParameters.end()) {
		return (*it).second;
	}

	gLog->Stream << "Configuration: " << Command << " not found in configuration file.";
	gLog->Print();
	return NULL;
}
