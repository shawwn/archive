#ifndef _PARSINGINTERFACE_H
#define _PARSINGINTERFACE_H

#include <vector>
#include <string>
#include <fstream>
using namespace std;

class ExtentionDescription
{
public:
	ExtentionDescription(const char *Folder, const char *Extention) {
		mFolder = Folder;
		mExtention = Extention;
	}
	string mFolder;
	string mExtention;
};

class ParsingInterface
{
private:
	std::vector<ExtentionDescription> mExtentions;
	bool Try(const string &File);
public:
	ParsingInterface();
	~ParsingInterface();

	string GetPath(const string &Path);
	string GetName(const string &Path);
};
extern ParsingInterface *gParsing;

extern wstring StringToWideString(const string &str);
extern string WideStringToString(const wstring &str);
extern string UpperString(const string &str);

#endif