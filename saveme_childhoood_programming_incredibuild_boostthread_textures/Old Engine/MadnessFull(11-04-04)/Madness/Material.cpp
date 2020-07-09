#include "dxstdafx.h"
#include "Material.h"
#include "ConfigurationInterface.h"
#include "LoggingInterface.h"

Material::Material()
{
	mDecal = NULL;
}

Material::~Material()
{
	Destroy();
}

bool Material::LoadTextureNoExtention(LPDIRECT3DDEVICE9 Device, LPDIRECT3DTEXTURE9 *Texture, string Filename)
{
	D3DPOOL Pool = gConfig->GetPool();
	D3DFORMAT Format = gConfig->GetTextureFormat();
	int TexFilter = gConfig->GetTextureFilter();
	int MipFilter = gConfig->GetTextureFilter();

	string AddedExtention;

	AddedExtention = Filename + ".jpg";
	if(!FAILED(D3DXCreateTextureFromFileExA(Device,AddedExtention.c_str(),0,0,0,0,Format,Pool,TexFilter,MipFilter,0,NULL,NULL,Texture))) {
		mPath = AddedExtention;
		return true;
	}

	AddedExtention = Filename + ".bmp";
	if(!FAILED(D3DXCreateTextureFromFileExA(Device,AddedExtention.c_str(),0,0,0,0,Format,Pool,TexFilter,MipFilter,0,NULL,NULL,Texture))) {
		mPath = AddedExtention;
		return true;
	}	
	
	AddedExtention = Filename + ".tga";
	if(!FAILED(D3DXCreateTextureFromFileExA(Device,AddedExtention.c_str(),0,0,0,0,Format,Pool,TexFilter,MipFilter,0,NULL,NULL,Texture))) {
		mPath = AddedExtention;
		return true;
	}
	
	AddedExtention = Filename + ".png";
	if(!FAILED(D3DXCreateTextureFromFileExA(Device,AddedExtention.c_str(),0,0,0,0,Format,Pool,TexFilter,MipFilter,0,NULL,NULL,Texture))) {
		mPath = AddedExtention;
		return true;
	}
	
	AddedExtention = Filename + ".jpeg";
	if(!FAILED(D3DXCreateTextureFromFileExA(Device,AddedExtention.c_str(),0,0,0,0,Format,Pool,TexFilter,MipFilter,0,NULL,NULL,Texture))) {
		mPath = AddedExtention;
		return true;
	}

	return false;
}

bool Material::Load(LPDIRECT3DDEVICE9 Device, const string &Filename)
{
	mPath = Filename;


	mPath = mPath.substr(0,mPath.find_last_of('.'));
	mName = mPath.substr(mPath.find_last_of('/')+1,mPath.size());

	if(!LoadTextureNoExtention(Device,&mDecal,mPath))
		return false;

	return true;
}

void Material::Destroy()
{
	if(mDecal)
		mDecal->Release();
	mDecal = NULL;

	mName = "";
	mPath = "";
	mGUID = 0;
}