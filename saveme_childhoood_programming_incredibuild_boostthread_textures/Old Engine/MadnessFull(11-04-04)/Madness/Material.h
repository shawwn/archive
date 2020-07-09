#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include "Types.h"
using namespace std;

class Material
{
private:
	string mPath;
	string mName;

	LPDIRECT3DTEXTURE9 mDecal;

	Word mGUID;

	bool LoadTextureNoExtention(LPDIRECT3DDEVICE9 Device, LPDIRECT3DTEXTURE9 *Texture, string Filename);
public:
	Material();
	~Material();

    bool Load(LPDIRECT3DDEVICE9 Device, const string &Filename);	
	void Destroy();

	void		SetDecal(LPDIRECT3DTEXTURE9 Decal) { mDecal = Decal; }
	void		SetGUID(Word GUID) { mGUID = GUID; }
	void		SetName(string Name) { mName = Name; }

	Word	GetGUID() { return mGUID; }
	string		GetName() { return mName; }
	LPDIRECT3DTEXTURE9 GetDecal() { return mDecal; }
};

#endif