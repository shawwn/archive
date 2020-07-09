#include "EffectInterface.h"
#include "LoggingInterface.h"

EffectInterface gEffects;


void EffectInterface::OnDestroyDevice()
{
	OnLostDevice();
}

void EffectInterface::OnLostDevice()
{
	for(UINT i = 0; i < NumEffects(); i++) 
	{
		Effect *eff = mEffects[i];
		if(eff) 
			eff->Release();
	}
}

void EffectInterface::OnResetDevice(LPDIRECT3DDEVICE9 Device)
{
	mUseDevice = Device;

	for(UINT i = 0; i < NumEffects(); i++) {
		if(!mEffects[i]->Reset(Device)) {
			gLog << "EffectInterface::Reset WARNING:  Failed to reload effect [" << mEffects[i]->GetFilename() << "]\n";
			SAFE_DELETE(mEffects[i]);
			if(i)
				mEffects[i] = mEffects[0];
		}
	}
}

UINT EffectInterface::Load(const std::string &Filename)
{
	if(!mUseDevice) {
		gLog ^ "EffectInterface ERROR: Device null.\n";
		return -1;
	}

	Effect *eff = new Effect;

	if(mEffects.size() == 0) {
		if(eff->Load(mUseDevice,"Data/Shaders/white.fx")) {
			mEffects.push_back(eff);
			eff = new Effect;
		}
		else
		{
			gLog << "EffectInterface WARNING: Error shader [white.fx] not found!  If a shader cannot be loaded, the interface will return the index of the first available shader.\n";
		}
	}

	if(!eff->Load(mUseDevice,Filename)) {
		SAFE_DELETE(eff);
		return -1;
	}

	mEffects.push_back(eff);

	return NumEffects()-1;
}