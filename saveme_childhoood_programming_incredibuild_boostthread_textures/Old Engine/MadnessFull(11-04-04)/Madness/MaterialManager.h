#ifndef _MATERIALMANAGER_H
#define _MATERIALMANAGER_H

#include <map>
#include "Material.h"
using namespace std;

#define NUMTEXSTAGESTATES 2

class MaterialManager
{
private:
	std::map<Word, Material *> mMaterials;

	Word mAssignGUID;
	Word mCurrentSetTexture[NUMTEXSTAGESTATES];

    string mBasePath;

	Word Add(Material *mat);
public:
	MaterialManager();
	~MaterialManager();

	typedef std::map<Word,Material *>::iterator Iterator;

	Iterator Begin() { return mMaterials.begin(); }
	Iterator End() { return mMaterials.end(); }
	size_t Count() { return mMaterials.size(); }

	void Remove(Word guid);
	void Remove(const char *name);
	void Clear();
	Material *FindMaterial(Word guid);
	Material *FindMaterial(const char *name);
	Word Find(const char *name);

	Word Load(LPDIRECT3DDEVICE9 Device, const string &Name);
	Word Add(LPDIRECT3DTEXTURE9 Texture);
	HRESULT SetTexture(LPDIRECT3DDEVICE9 Device, int Stage, Word GUID);
	LPDIRECT3DTEXTURE9 GetTexture(Word GUID);
	void Reset();

	string	GetBasePath()					{ return mBasePath; }
	void	SetBasePath(const char *Base)	{ mBasePath = Base; }
};
extern MaterialManager *gMaterials;

#endif