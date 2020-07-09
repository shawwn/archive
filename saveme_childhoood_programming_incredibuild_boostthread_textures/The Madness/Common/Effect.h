#ifndef EFFECT_H
#define EFFECT_H

#define NOMINMAX
#include <d3dx9.h>
#include <string>
#include "Macros.h"

class Effect
{
private:
	LPD3DXEFFECT mEffect;
	std::string mFilename;
public:
	Effect() : mEffect(NULL) { }
	~Effect() { Destroy();  }
	bool Load(LPDIRECT3DDEVICE9 Device, const std::string &Filename);
	inline void Destroy() { SAFE_RELEASE(mEffect); mFilename = ""; }
	inline void Release() { SAFE_RELEASE(mEffect); }
	inline bool Reset(LPDIRECT3DDEVICE9 Device) { return Load(Device,mFilename); }

	inline LPD3DXEFFECT GetEffect() { return mEffect; }
	inline const std::string &GetFilename() { return mFilename; }

	bool Begin(UINT *Passes, DWORD Flags);
	bool BeginPass(UINT Pass);
	inline void EndPass() { mEffect->EndPass(); }
	inline void End() { mEffect->End(); }
};

#endif