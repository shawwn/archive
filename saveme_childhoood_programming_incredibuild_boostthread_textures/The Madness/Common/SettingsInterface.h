#ifndef SETTINGSINTERFACE_H
#define SETTINGSINTERFACE_H

#include <string>
#include <vector>
#include <map>
#include "Types.h"
#include "Parsing.h"

class Setting
{
public:
	std::string Variable;
	std::string Value;
};

class SettingsInterface
{
protected:
	typedef std::map<unsigned long,std::vector<Setting *> >::iterator Iterator;
	std::map<unsigned long, std::vector<Setting *> > mSettings;
	inline Iterator Begin() { return mSettings.begin(); }
	inline Iterator End() { return mSettings.end(); }

	virtual void SetDefaults() { }
	virtual bool SetApplicationVariable(const std::string &Variable, const std::string &Value) = 0;

	void AddInt(const std::string &Current, const std::string &Name, int &Var, const std::string &Value)			{ if(Current == Name) { Var = atoi(Value.c_str()); } }
	void AddUINT(const std::string &Current, const std::string &Name, UINT &Var, const std::string &Value)			{ if(Current == Name) { Var = (UINT)atoi(Value.c_str()); } }
	void AddWord(const std::string &Current, const std::string &Name, Word &Var, const std::string &Value)			{ if(Current == Name) { Var = (Word)atoi(Value.c_str()); } }
	void AddFloat(const std::string &Current, const std::string &Name, float &Var, const std::string &Value)		{ if(Current == Name) { Var = (float)atof(Value.c_str()); } }
	void AddBool(const std::string &Current, const std::string &Name, bool &Var, const std::string &Value )			{ if(Current == Name) { Var = ((Value != "0") && (Upper(Value) != "F") && (Upper(Value) != "FALSE")); } }
	void AddString(const std::string &Current, const std::string &Name, std::string &Var, const std::string &Value) { if(Current == Name) { Var = Value; } }
public:
	SettingsInterface() { }
	~SettingsInterface() { ClearSettings(); }

	bool Load(const std::string &Filename);
	Setting *Find(std::string Variable);
	void ClearSettings();
	
	void Test();
};


#endif