#include "dxstdafx.h"
#include "Effect.h"
#include "LoggingInterface.h"
#include "Parsinginterface.h"

Effect::Effect()
{
	mEffect = NULL;
	mGUID = -1;
	mName = "Null Shader";
}

Effect::~Effect()
{
	if(mEffect)
		mEffect->Release();
}

bool Effect::Load(LPDIRECT3DDEVICE9 Device, const string &Filename)
{
	if(!Device)
		return false;
	
	HRESULT hr;

	//hr = D3DXCreateEffectFromFile(Device,StringToWideString(Filename).c_str(),NULL,NULL,D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION,NULL,&mEffect,NULL);
	hr = D3DXCreateEffectFromFile(Device,StringToWideString(Filename).c_str(),NULL,NULL,0,NULL,&mEffect,NULL);

	if(hr != D3D_OK)
	{
		gLog->Stream << "Effect " << Filename << ": Could not load.";
		gLog->Print();
		return false;
	}

	mPath = Filename;
	mName = gParsing->GetName(mPath);

	gLog->Stream << "Effect " << mName << ": Loaded.";
	gLog->Print();

	return true;
}