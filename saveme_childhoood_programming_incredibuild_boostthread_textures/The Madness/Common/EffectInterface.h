#ifndef EFFECTINTERFACE_H
#define EFFECTINTERFACE_H

#include <vector>
#include "Effect.h"
#include "Macros.h"
#include "Types.h"

class EffectInterface
{
private:
	std::vector<Effect *> mEffects;
	LPDIRECT3DDEVICE9 mUseDevice;

public:
	EffectInterface() : mUseDevice(NULL) { }
	~EffectInterface() { Destroy(); }

	void OnDestroyDevice();
	void OnLostDevice();
	void OnResetDevice(LPDIRECT3DDEVICE9 Device);

	UINT Load(const std::string &Filename);
	inline void Destroy() { for(UINT i = 0; i < NumEffects(); i++) { SAFE_DELETE(mEffects[i]); }  mEffects.clear(); } 
	inline Effect *FindEffectInterface(UINT Index) { return mEffects[(Index < NumEffects()-1 ? Index : NumEffects()-1)]; }
	inline LPD3DXEFFECT FindEffect(UINT Index) { return mEffects[(Index < NumEffects()-1 ? Index : NumEffects()-1)]->GetEffect(); }

	inline UINT NumEffects() { return (UINT)mEffects.size(); }
};
extern EffectInterface gEffects;

#endif