#include "Effect.h"
#include "LoggingInterface.h"
#include "Macros.h"
#include "ClientSettingsInterface.h"
using namespace std;

bool Effect::Load(LPDIRECT3DDEVICE9 Device, const std::string &Filename)
{
	if(!Device)
		return false;
	SAFE_RELEASE(mEffect);

	if(FAILED(D3DXCreateEffectFromFileEx(Device,Filename.c_str(),NULL,NULL,NULL,(gSettings.DebugVertexShaders ? D3DXSHADER_SKIPOPTIMIZATION | D3DXSHADER_DEBUG : 0),NULL,&mEffect,NULL))) {
		gLog << "Effect [" << Filename << "] WARNING: Could not open file.\n";
		return false;
	}

	mFilename = Filename;

	gLog << "Effect [" << mFilename << "]: Loaded.\n";

	return true;
}
bool Effect::Begin(UINT *Passes, DWORD Flags)
{
	if(FAILED(mEffect->Begin(Passes,Flags))) {
		gLog ^ "Effect [" ^ mFilename ^ "] ERROR: Begin() failed.\n";
		return false;
	}
	return true;
}

bool Effect::BeginPass(UINT Pass)
{
	if(FAILED(mEffect->BeginPass(Pass))) {
		gLog ^ "Effect [" ^ mFilename ^ "] ERROR: BeginPass([" ^ Pass ^ "]) failed.  (Invalid pass or parameter?)\n";
		return false;
	}
	return true;
}