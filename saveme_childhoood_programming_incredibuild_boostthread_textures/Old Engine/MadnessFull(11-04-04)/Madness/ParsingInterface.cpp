#include "dxstdafx.h"
#include "ParsingInterface.h"
#include "LoggingInterface.h"

ParsingInterface *gParsing;

ParsingInterface::ParsingInterface()
{
	mExtentions.push_back(ExtentionDescription("textures","jpg"));
	mExtentions.push_back(ExtentionDescription("textures","png"));
	mExtentions.push_back(ExtentionDescription("textures","bmp"));
	mExtentions.push_back(ExtentionDescription("textures","jpeg"));
	mExtentions.push_back(ExtentionDescription("heightmaps","bmp"));
	mExtentions.push_back(ExtentionDescription("heightmaps","raw"));
	mExtentions.push_back(ExtentionDescription("","cfg"));
	mExtentions.push_back(ExtentionDescription("maps","bsp"));
	mExtentions.push_back(ExtentionDescription("shaders","fx"));
	mExtentions.push_back(ExtentionDescription("models","x"));
}

ParsingInterface::~ParsingInterface()
{
	mExtentions.clear();
}

bool ParsingInterface::Try(const string &File)
{
	ifstream in;
	in.open(File.c_str());
	if(in.is_open()) {
		return true;	
	}
	return false;
}

string ParsingInterface::GetPath(const string &Path)
{
	if(Try(Path))
		return Path;
	string Get;
	size_t pos = Path.find_last_of('/');
	if(pos != std::string::npos)
	{
		Get = Path.substr(pos,Path.length());
		if(Try(Get))
			return Get;
	}
	else
		Get = Path;

    string Ext;
	string Data = "Data";

	pos = Get.find_last_of('.');
	if(pos != std::string::npos)
	{
		string Extention = Get.substr(Get.find_last_of('.')+1,Get.length());

        for(int i = 0; i < (int)mExtentions.size(); i++)
		{
			if(Extention == mExtentions[i].mExtention)
			{
				if(mExtentions[i].mFolder.length() == 0)
				{
					if(Try(Get))
						return Get;
					continue;
				}
				Ext = Data + "/" + mExtentions[i].mFolder + "/" + Get;
				if(Try(Ext))
					return Ext;
			}
		}

		gLog->Stream << "Parser " << Get << ": Couldn't find file.";
		gLog->Print();

		return Get;
	}

	for(int i = 0; i < (int)mExtentions.size(); i++)
	{
		if(mExtentions[i].mFolder.length() == 0) {
			Ext = Get + "." + mExtentions[i].mExtention;
			if(Try(Ext))
				return Ext;
			continue;
		}
		
		Ext = Data + "/" + mExtentions[i].mFolder + "/" + Get + "." + mExtentions[i].mExtention;
		if(Try(Ext))
			return Ext;
	}

	gLog->Stream << "Parser " << Get << ": Couldn't find file.";
	gLog->Print();

	return Get.c_str();
}
string ParsingInterface::GetName(const string &Path)
{
	string Get = Path;
	size_t pos = Get.find_last_of('/');
	if(pos != std::string::npos)
	{
		Get = Get.substr(pos+1,Get.length());
	}

	pos = Get.find_last_of('.');
	if(pos != std::string::npos)
	{
		Get = Get.substr(0,pos);
	}

	return Get;
}
wstring StringToWideString(const string &str)
{
	/*int Len = (int)str.length()+1;
	if(Len <= 0)
		return L"";
	LPWSTR lpszW = new WCHAR[Len];
	if(!MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,lpszW,Len))
		return L"";
	return lpszW;*/

	return std::wstring(str.begin(),str.end());
}

string WideStringToString(const wstring &str)
{
	int Length = (int)str.length()+1;
	CHAR *lpszA = new CHAR[Length];
	if(!WideCharToMultiByte(CP_ACP,0,str.c_str(),-1,lpszA,Length,NULL,NULL))
		return "";
	return lpszA;

	//return std::string(str.begin(),str.end());
}

string UpperString(const string &str)
{
	string ret = str;
	for(size_t i = 0; i < ret.size(); i++) { ret[i] = toupper(ret[i]); }
	return ret;
}
