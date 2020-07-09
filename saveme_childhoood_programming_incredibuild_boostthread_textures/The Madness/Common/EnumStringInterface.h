#ifndef ENUMSTRINGINTERFACE_H
#define ENUMSTRINGINTERFACE_H

#include <map>
#include <string>
#include <vector>

class EnumStringInterface
{
private:
	typedef std::map<int,std::vector<std::string> >::iterator Iterator;
	std::map<int,std::vector<std::string> > mStrings;
	inline Iterator Begin() { return mStrings.begin(); }
	inline Iterator End() { return mStrings.end(); }
public:
	EnumStringInterface();
	void Add(int Value, const std::string &String) { mStrings[Value].push_back(String); }
	const std::string &Find(int Value, const std::string &Prefix = "", const std::string &AlternatePrefix = "");
	void Clear() { mStrings.clear(); }
};
extern EnumStringInterface EnumToString;

#endif