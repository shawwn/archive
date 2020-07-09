#include "SettingsInterface.h"
#include "LoggingInterface.h"
#include "Parsing.h"
#include "Macros.h"
#include <fstream>
using namespace std;

bool SettingsInterface::Load(const string &Filename)
{
	SetDefaults();
	ifstream in(Filename.c_str());
	if(!in.is_open())
	{
		gLog << "WARNING: Settings file [" << Filename << "] not found!\n";
		return false;
	}

	std::string Line;
	std::string Variable;
	std::string Value;
	Setting *Token = NULL;
	while(!in.eof()) 
	{
		getline(in,Line,'\n');
		Line = Trim(Line);
		if(!Line.length()) 
			continue;
		ParseToken(Line,Variable,Value);
		if(Variable.length() == 0 || Value.length() == 0)
			continue;
		if(SetApplicationVariable(Upper(Variable),Value))
			continue;
		else
			gLog << "Settings WARNING: Variable [" << Variable << "] with value [" << Value << "] unhandled.\n";
		Token = new Setting;
		Token->Variable = Upper(Variable);
		Token->Value = Value;
		mSettings[Hash(Token->Variable)].push_back(Token);
	}

	return true;
}

Setting *SettingsInterface::Find(std::string Variable)
{
	Variable = Upper(Variable);
	Iterator it = mSettings.find(Hash(Variable));
	const vector<Setting *> &v = (*it).second;
	for(size_t i = 0; i < v.size(); i++)
	{
		if(v[i]->Variable == Variable)
			return v[i];
	}
	static Setting *Blank = new Setting;
	return Blank;
}

void SettingsInterface::Test()
{
	// Test bad filename
	if(Load("ASDF")) {
		gLog ^ "TEST FAILED: <SettingsInterface> Load() returned true on a bad filename.\n";
	}

	// Create fake settings
	Setting **t = new Setting*;
	int i;
	for(i = 0; i < 3; i++)
		t[i] = new Setting;
	t[0]->Variable = "TESTING";
	t[0]->Value = "123";
	t[1]->Variable = "OMFG";
	t[1]->Value = "wtf";
	t[2]->Variable = "LOL";
	t[2]->Value = "Ninja";

	for(i = 0; i < 3; i++)
		mSettings[Hash(t[i]->Variable)].push_back(t[i]);

	Setting *ret;
	// Test good setting find
	ret = Find("TESTING");
	if(ret->Variable != "TESTING") {
		gLog ^ "TEST FAILED: <SettingsInterface> Find(Variable) returned a bad (Setting *), variable not correct.\n";
	}
	if(ret->Value != "123") {
		gLog ^ "TEST FAILED: <SettingsInterface> Find(Variable) returned a bad (Setting *), variable not correct.\n";
	}

	// Test bad setting find
	ret = Find("BAD");
	if(ret->Variable != "" || ret->Value != "") {
		gLog ^ "TEST FAILED: <SettingsInterface> Find(Variable) returned an incorrect value for a bad Variable.\n";
	}
	
	// Test non deletion of map
	ClearSettings();
	if(mSettings.size() > 0) {
		gLog ^ "TEST FAILED: <SettingsInterface> mSettings.size() is > 0 after a ClearSettings().\n";
	}

	// Test nonexistant setting find
	ret = Find("BAD");
	if(ret->Variable != "" || ret->Value != "") {
		gLog ^ "TEST FAILED: <SettingsInterface> Find(Variable) returned an incorrect value when there are no (Setting *)s hashed.\n";
	}

    for(i = 0; i < 3; i++)
		SAFE_DELETE(t[i]);
	SAFE_DELETE_ARRAY(t);
}

void SettingsInterface::ClearSettings() { 
	Iterator it = Begin();
	while(it != End())
	{
		const std::vector<Setting *> &v = (*it).second; 
		for(size_t i = 0; i < v.size(); i++)
			delete v[i]; 
        it = mSettings.erase(it);
	} 
}