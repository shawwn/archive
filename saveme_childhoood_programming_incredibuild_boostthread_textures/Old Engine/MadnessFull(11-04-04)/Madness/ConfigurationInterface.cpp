#include "dxstdafx.h"
#include "ConfigurationInterface.h"
#include "InputInterface.h"
#include "LoggingInterface.h"

ConfigurationInterface *gConfig;

ConfigurationInterface::ConfigurationInterface()
{
	Destroy();
}

ConfigurationInterface::~ConfigurationInterface()
{
}

string ConfigurationInterface::GetParameter(const int Line)
{
	string str;
	if(mFile.size() <= 0 || Line > (int)mFile.size())
		return str;

	str = mFile[Line];
	str = str.substr(0,str.find_first_of('='));
	while(str[0] == ' ')
	{
		str = str.substr(1,str.length());
	}
	while(str[str.length()-1] == ' ')
	{
		str = str.substr(0,str.length()-1);
	}
	return str;
}

void ConfigurationInterface::GetOneArgument(const int Line, string *Arg)
{
	string str;
	if(mFile.size() <= 0 || Line > (int)mFile.size())
		return;

	str = mFile[Line];
	str = str.substr(str.find_first_of('=')+1,str.length());
	while(str[0] == ' ')
	{
		str = str.substr(1,str.length());
	}
	while(str[str.length()-1] == ' ')
	{
		str = str.substr(0,str.length()-1);
	}
	*Arg = str;
}

void ConfigurationInterface::GetOneArgument(const int Line, char *c)
{
	string str;
	if(mFile.size() <= 0 || Line > (int)mFile.size())
		return;

	str = mFile[Line];
	str = str.substr(str.find_first_of('=')+1,str.length());
	while(str[0] == ' ')
	{
		str = str.substr(1,str.length());
	}
	while(str[str.length()-1] == ' ')
	{
		str = str.substr(0,str.length()-1);
	}
	*c = gInput->ASCIItoDIK(str);
}

void ConfigurationInterface::GetTwoArguments(const int Line, string *Arg1, string *Arg2, char Separator)
{
	string str1;
	string str2;
	if(mFile.size() <= 0 || Line > (int)mFile.size())
		return;

	str1 = mFile[Line];
	str2 = mFile[Line];
	str1 = str1.substr(str1.find('=')+1,str1.length());
	str2 = str2.substr(str2.find('=')+1,str2.length());
	str1 = str1.substr(0,str1.find(Separator));
	str2 = str2.substr(str2.find(Separator)+1,str2.length());
	while(str1[0] == ' ')
	{
		str1 = str1.substr(1,str1.length());
	}
	while(str1[str2.length()-1] == ' ')
	{
		str1 = str1.substr(0,str1.length()-1);
	}
	while(str2[0] == ' ')
	{
		str2 = str2.substr(1,str2.length());
	}
	while(str2[str2.length()-1] == ' ')
	{
		str2 = str2.substr(0,str2.length()-1);
	}
	*Arg1 = str1;
	*Arg2 = str2;
}

bool ConfigurationInterface::Load(const string &Filename)
{
	Destroy();

	ifstream in(Filename.c_str());
	if(!in.is_open())
	{
		gLog->Stream << "Config: Couldn't open " << Filename;
		gLog->Print();
		return false;
	}
	mFilename = Filename;

	string line;	
	do
	{
		getline(in,line,'\n');
		mFile.push_back(line);
	} while(!in.eof());

	string temp;

	for(int i = 0; i < (int)mFile.size(); i++)
	{
		if(stricmp(GetParameter(i).c_str(),"resolution") == 0) {
			string strWidth;
			string strHeight;
			GetTwoArguments(i,&strWidth,&strHeight,'x');
			mWidth = atoi(strWidth.c_str());
			mHeight = atoi(strHeight.c_str());
		}
		if(stricmp(GetParameter(i).c_str(),"windowed") == 0) {
			string strWind;
			GetOneArgument(i,&strWind);
			if((stricmp(strWind.c_str(),"true") == 0) || (stricmp(strWind.c_str(),"t") == 0))
				mWindowed = true;
			else
				mWindowed = false;
		}

		if(stricmp(GetParameter(i).c_str(),"map") == 0) {
			GetOneArgument(i,&mMap);
		}

		if(stricmp(GetParameter(i).c_str(),"uselightmaps") == 0) {
			string strUse;
			GetOneArgument(i,&strUse);
			if((stricmp(strUse.c_str(),"true") == 0) || (stricmp(strUse.c_str(),"t") == 0))
				mUseLightmaps = true;
			else
				mUseLightmaps = false;
		}

		if(stricmp(GetParameter(i).c_str(),"lightmapintensity") == 0) {
			string strSens;
			GetOneArgument(i,&strSens);
			mLightmapIntensity = (float)atof(strSens.c_str());
		}

		if(stricmp(GetParameter(i).c_str(),"mousesmoothingdegree") == 0) {
			string strSmooth;
			GetOneArgument(i,&strSmooth);
			mMouseSmoothingDegree = atoi(strSmooth.c_str());
		}

		if(stricmp(GetParameter(i).c_str(),"mousesensitivity") == 0) {
			string strSens;
			GetOneArgument(i,&strSens);
			mMouseSensitivity = (float)atof(strSens.c_str());
		}

		if(stricmp(GetParameter(i).c_str(),"keymoveforward") == 0) {
			GetOneArgument(i,&temp);
			mKeyMoveForward = gInput->ASCIItoDIK(temp);
		}

		if(stricmp(GetParameter(i).c_str(),"keymovebackward") == 0) {
			GetOneArgument(i,&temp);
			mKeyMoveBackward = gInput->ASCIItoDIK(temp);
		}

		if(stricmp(GetParameter(i).c_str(),"keystrafeleft") == 0) {
			GetOneArgument(i,&temp);
			mKeyStrafeLeft = gInput->ASCIItoDIK(temp);
		}

		if(stricmp(GetParameter(i).c_str(),"keystraferight") == 0) {
			GetOneArgument(i,&temp);
			mKeyStrafeRight = gInput->ASCIItoDIK(temp);
		}

		if(stricmp(GetParameter(i).c_str(),"keylookwithmouse") == 0) {
			GetOneArgument(i,&temp);
			mKeyLookWithMouse = gInput->ASCIItoDIK(temp);
		}

		if(stricmp(GetParameter(i).c_str(),"keymenu") == 0) {
			GetOneArgument(i,&temp);
			mKeyMenu = gInput->ASCIItoDIK(temp);
		}
	}

	return true;
}


void ConfigurationInterface::Destroy()
{
	mWidth = 1024;
	mHeight = 768;
	mWindowed = true;
	mUseLightmaps = true;
	mLightmapIntensity = 3;
	mMouseSmoothingDegree = 3;
	mMouseSensitivity = 0.1f;

	mBackBufferFormat = D3DFMT_R5G6B5;
	mBackBufferPool = D3DPOOL_MANAGED;
	mMultiSampleType = D3DMULTISAMPLE_NONE;
	mMultiSampleQuality = 0;

	mPool = D3DPOOL_MANAGED;
	mTextureFormat = D3DFMT_A8R8G8B8;
	mTextureFilter = D3DX_FILTER_LINEAR;
	mFilter = D3DTEXF_LINEAR;

	mKeyMoveForward = DIK_W;
	mKeyMoveBackward = DIK_S;
	mKeyStrafeLeft = DIK_A;
	mKeyStrafeRight = DIK_D;
	mKeyLookWithMouse = DIK_TAB;
	mKeyMenu = DIK_ESCAPE;
	mKeyConsole = DIK_GRAVE;
}

void ConfigurationInterface::SetLookWithMouse(const bool &Set)
{
	if(mLookWithMouse == Set)
		return;
	mLookWithMouse = Set;
	if(mLookWithMouse) {
		gMouse->CenterAll();
		ShowCursor(false);
	}
	else
		ShowCursor(true);
}

void ConfigurationInterface::ToggleLookWithMouse()
{
	SetLookWithMouse(!mLookWithMouse);	
}