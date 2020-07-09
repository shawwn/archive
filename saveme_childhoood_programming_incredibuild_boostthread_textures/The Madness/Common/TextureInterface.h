#ifndef TEXTUREINTERFACE_H
#define TEXTUREINTERFACE_H

#include <d3dx9.h>
#include <vector>
#include <string>
#include <map>
#include "Types.h"
#include "Macros.h"

class TDATA
{
public:
	TDATA() : Texture(NULL), Pool(D3DPOOL_SYSTEMMEM), Usage(0) { }
	~TDATA();
	LPDIRECT3DTEXTURE9 Texture;
	D3DXIMAGE_INFO Info;
	D3DPOOL Pool;
	DWORD Usage;
	D3DCOLOR ColorKey;
	std::string Filename;
	std::vector<VTEXCOORD2> TexCoords; // Stores texture coords to use smaller-than-texsize textures (if 
										//  several textures are being stored in just one image file)
	std::map<int,int> TextureNames; // Stores texture names to use smaller-than-texsize textures (if several
										// textures are being stored in just one image file)
};

typedef TDATA* LPTDATA;

class TextureInterface
{
private:
	std::vector<LPTDATA> mTextures;
	LPDIRECT3DDEVICE9 mUseDevice;

	LPTDATA mBlankTexture;

	bool LoadCutterFile(const std::string &Filename, LPTDATA TextureData, std::vector<std::string> &CutterNames);
public:
	TextureInterface() : mUseDevice(NULL), mBlankTexture(NULL) { }
	~TextureInterface() { Destroy(); }

	UINT LoadMultiple(std::string Filename, std::vector<std::string> &CutterNames, D3DCOLOR ColorKey = 0, D3DPOOL Pool = D3DPOOL_MANAGED, D3DFORMAT Format = D3DFMT_A8R8G8B8);
	UINT Load(const std::string &Filename, D3DCOLOR ColorKey = 0, D3DPOOL Pool = D3DPOOL_MANAGED, DWORD Usage = 0, UINT Width = 0, UINT Height = 0, D3DFORMAT Format = D3DFMT_A8R8G8B8);
	inline LPTDATA Find(UINT Index) { return (Index < NumTextures() ? mTextures[Index] : mBlankTexture); }
	void Destroy();
	void OnDestroyDevice();
	void OnLostDevice();
	bool OnResetDevice(LPDIRECT3DDEVICE9 Device);
	inline UINT NumTextures() { return (UINT)mTextures.size(); }
};

extern TextureInterface gTextures;

#endif