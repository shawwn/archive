#ifndef _EFFECT_H
#define _EFFECT_H

#include <d3dx9.h>
#include <string>
#include "Types.h"
using namespace std;

class Effect
{
private:
	Word mGUID;
	string mName;
	string mPath;
public:
	Effect();
	~Effect();

	LPD3DXEFFECT mEffect;

	bool Load(LPDIRECT3DDEVICE9 Device, const string &Filename);

	void SetGUID(const Word &GUID) { mGUID = GUID; }
	const Word &GetGUID() { return mGUID; }
	const string &GetName() { return mName; }
};

#endif