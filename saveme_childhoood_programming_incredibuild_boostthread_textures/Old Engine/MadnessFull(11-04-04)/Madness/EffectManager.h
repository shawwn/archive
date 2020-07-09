#ifndef _EFFECTMANAGER_H
#define _EFFECTMANAGER_H

#include <d3dx9.h>
#include <string>
#include <map>
#include "Effect.h"
using namespace std;

class EffectManager
{
private:
	std::map<string,Effect *> mEffects;

public:
	EffectManager();
	~EffectManager();
	typedef std::map<string,Effect *>::iterator Iterator;

	bool OnResetDevice(LPDIRECT3DDEVICE9 Device);
	void OnLostDevice();

	string LoadEffect(LPDIRECT3DDEVICE9 Device, const string &Filename);
	Effect *GetEffect(const string &Name);

	Effect *BeginTexlight(const D3DXMATRIX &ViewProj, const D3DXMATRIX &World, const D3DXMATRIX &WorldInverseTranspose, const D3DXMATRIX &ViewInverse,
						LPDIRECT3DTEXTURE9 Texture, const D3DXVECTOR3 &lightDir, const D3DXVECTOR3 &lightColor, const D3DXVECTOR3 &lightAmbient, 
						const D3DXVECTOR3 &materialDiffuse, const D3DXVECTOR3 &materialSpecular, const float &Shininess, UINT &passes);
	Effect *BeginTextured(const D3DXMATRIX &WorldViewProj, LPDIRECT3DTEXTURE9 Texture, UINT &passes);
	Effect *BeginWhite(const D3DXMATRIX &WorldViewProj, UINT &passes);

	string Add(Effect *eff);
	bool Remove(const string &Name);
	bool Remove(Effect *eff);
	Effect *Find(const string &Name);
	void Clear();

	int Count() { return (int)mEffects.size(); }
};
extern EffectManager *gEffects;

#endif
