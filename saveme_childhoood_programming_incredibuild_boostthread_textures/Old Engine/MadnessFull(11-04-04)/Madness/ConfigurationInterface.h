#ifndef _CONFIGURATIONINTERFACE_H
#define _CONFIGURATIONINTERFACE_H

#include <vector>
#include <string>
#include <fstream>
using namespace std;

class ConfigurationInterface
{
private:
	string mFilename;
	std::vector<string> mFile;
	string GetParameter(const int Line);
	void GetOneArgument(const int Line, string *Arg);
	void GetOneArgument(const int Line, char *c);
	void GetTwoArguments(const int Line, string *Arg1, string *Arg2, char Separator);
public:
	ConfigurationInterface();
	~ConfigurationInterface();

	/*
	  D3DFORMAT           Format;
    D3DRESOURCETYPE     Type;
    DWORD               Usage;
    D3DPOOL             Pool;

    D3DMULTISAMPLE_TYPE MultiSampleType;
    DWORD               MultiSampleQuality;
    UINT                Width;
    UINT                Height;
	*/

	// Not found in config file //
	bool mLookWithMouse;
	//////////////////////////////

    int mWidth;
	int mHeight;
	bool mWindowed;
	string mMap;
	bool mUseLightmaps;
	float mMouseSensitivity;
	int mMouseSmoothingDegree;
	float mLightmapIntensity;

	D3DFORMAT mBackBufferFormat;
	D3DPOOL mBackBufferPool;
	D3DMULTISAMPLE_TYPE mMultiSampleType;
	DWORD mMultiSampleQuality;

	D3DPOOL mPool;
	D3DFORMAT mTextureFormat;
    DWORD mTextureFilter;
	DWORD mFilter;

	int mKeyMoveForward;
	int mKeyMoveBackward;
	int mKeyStrafeLeft;
	int mKeyStrafeRight;
	int mKeyLookWithMouse;
	int mKeyMenu;
	int mKeyConsole;

	void SetLookWithMouse(const bool &Set);
	void ToggleLookWithMouse();

	void SetWidth(const int &Width )							{ mWidth = Width; }
	void SetHeight(const int &Height)							{ mHeight = Height; }
	void SetWindowed(const bool &Set)							{ mWindowed = Set; }
	void SetBackBufferFormat(const D3DFORMAT &Format)			{ mBackBufferFormat = Format; }
	void SetBackBufferPool(const D3DPOOL &Pool)					{ mBackBufferPool = Pool; }
	void SetMultiSampleType(const D3DMULTISAMPLE_TYPE &Type)	{ mMultiSampleType = Type; }
	void SetMultiSampleQuality(const DWORD &Quality)			{ mMultiSampleQuality = Quality; }

	void SetBackBuffer(const D3DSURFACE_DESC* Surface) {
		if(!Surface)
			return;
		mWidth = Surface->Width;
		mHeight = Surface->Height;
		mBackBufferFormat = Surface->Format;
		mBackBufferPool = Surface->Pool;
		mMultiSampleType = Surface->MultiSampleType;
		mMultiSampleQuality = Surface->MultiSampleQuality;
	}

	bool GetLookWithMouse()	{ return mLookWithMouse; }
	
	int GetWidth()					{ return mWidth; } 
	int GetHeight()					{ return mHeight; }
	bool GetWindowed()				{ return mWindowed; }
	string GetMap()					{ return mMap; }
	bool GetUseLightmaps()			{ return mUseLightmaps; }
	float GetLightmapIntensity()	{ return mLightmapIntensity; }
	float GetMouseSensitivity()		{ return mMouseSensitivity; }
	int GetMouseSmoothingDegree()	{ return mMouseSmoothingDegree; }

	D3DFORMAT GetBackBufferFormat()			{ return mBackBufferFormat; }
	D3DPOOL GetBackBufferPool()				{ return mBackBufferPool; }
	D3DMULTISAMPLE_TYPE GetMultiSampleType(){ return mMultiSampleType; }
	DWORD GetMultiSampleQuality()			{ return mMultiSampleQuality; }

	D3DPOOL GetPool()			{ return mPool; }
	D3DFORMAT GetTextureFormat(){ return mTextureFormat; }
	DWORD GetTextureFilter()	{ return mTextureFilter; }
	DWORD GetFilter()			{ return mFilter; }

	int GetKeyMoveForward()		{ return mKeyMoveForward; }
	int GetKeyMoveBackward()	{ return mKeyMoveBackward; }
	int GetKeyStrafeLeft()		{ return mKeyStrafeLeft; }
	int GetKeyStrafeRight()		{ return mKeyStrafeRight; }
	int GetKeyLookWithMouse()	{ return mKeyLookWithMouse; }
	int GetKeyMenu()			{ return mKeyMenu; }
	int GetKeyConsole()			{ return mKeyConsole; }

	string GetPath()			{ return "Data/"; }
	string GetMapFolder()		{ return "maps/"; }
	string GetTextureFolder()	{ return "textures/"; }

	float GetUnitsPerMeter() { return 32.0f; }

	bool Load(const string &Filename);
	void Destroy();
};
extern ConfigurationInterface *gConfig;

#endif