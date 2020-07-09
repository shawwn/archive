#include "dxstdafx.h"
#include "MaterialManager.h"
#include "LoggingInterface.h"
#include "Direct3DError.h"

MaterialManager *gMaterials;

MaterialManager::MaterialManager()
{
	mAssignGUID = 0;
	for(int i = 0; i < NUMTEXSTAGESTATES; i++)
		mCurrentSetTexture[i] = -1;
}

MaterialManager::~MaterialManager()
{
	Clear();
}

Word MaterialManager::Add(Material *mat)
{
	if(!mat)
		return -1;
	mat->SetGUID(mAssignGUID);
	mMaterials[mAssignGUID] = mat;
	mAssignGUID++;

	gLog->Stream << "Material " << mat->GetName() << ": Loaded.";
	gLog->Print();
	return mAssignGUID-1;
}

void MaterialManager::Remove(Word guid)
{
	Iterator it = mMaterials.find(guid);
	if(it != mMaterials.end())
	{
		gLog->Stream << "Material " << (*it).second->GetName() << ": Deallocating...";
		gLog->Print();
		delete (*it).second;
		mMaterials.erase(guid);
	}
}

void MaterialManager::Remove(const char *name)
{
	for(Iterator it = mMaterials.begin(); it != mMaterials.end(); it++)
	{
		if((*it).second->GetName() == name) {
			mMaterials.erase((*it).second->GetGUID());
			delete (*it).second;
			break;
		}
	}
}

void MaterialManager::Clear()
{
	for(Iterator it = mMaterials.begin(); it != mMaterials.end(); it++)
	{
		gLog->Stream << "Material " << (*it).second->GetName() << ": Deallocating...";
		gLog->Print();
		delete (*it).second;
	}
	mMaterials.clear();
}

Material *MaterialManager::FindMaterial(Word guid)
{
	Iterator it = mMaterials.find(guid);
	if(it != mMaterials.end())
		return (*it).second;
	return NULL;
}

Material *MaterialManager::FindMaterial(const char *name)
{
	for(Iterator it = mMaterials.begin(); it != mMaterials.end(); it++)
	{
		if((*it).second->GetName() == name)
			return (*it).second;
	}
	return NULL;
}

Word MaterialManager::Find(const char *name)
{
	for(Iterator it = mMaterials.begin(); it != mMaterials.end(); it++)
	{
		if((*it).second->GetName() == name)
			return (*it).second->GetGUID();
	}
	return -1;
}

Word MaterialManager::Load(LPDIRECT3DDEVICE9 Device, const string &Name)
{
	string FullPath = mBasePath + Name;
	string strName = Name;
	strName = strName.substr(strName.find_last_of('/')+1,strName.length());
	for(Iterator it = mMaterials.begin(); it != mMaterials.end(); it++)
	{
		if((*it).second->GetName() == strName)
			return (*it).second->GetGUID();
	}
	Material *mat = new Material;
	if(!mat->Load(Device,FullPath.c_str()))
	{
		delete mat;
		return false;
	}

	return Add(mat);
}
Word MaterialManager::Add(LPDIRECT3DTEXTURE9 Texture)
{
	if(!Texture)
		return -1;
	Material *mat = new Material;
	mat->SetDecal(Texture);
	ostringstream TexName;
	TexName << "Custom Texture: " << mAssignGUID;
	mat->SetName(TexName.str());
    return Add(mat);
}

void MaterialManager::Reset()
{
	for(int i = 0; i < NUMTEXSTAGESTATES; i++)
		mCurrentSetTexture[i] = -1;
}

HRESULT MaterialManager::SetTexture(LPDIRECT3DDEVICE9 Device, int Stage, Word GUID)
{
	Iterator it = mMaterials.find(GUID);
	if(it == mMaterials.end())
		return -1;
	Material *Mat = (*it).second;
	if(mCurrentSetTexture[Stage] != Mat->GetGUID()) {
		mCurrentSetTexture[Stage] = Mat->GetGUID();
		return Device->SetTexture(Stage,Mat->GetDecal());
	}
	return D3D_OK;
}

LPDIRECT3DTEXTURE9 MaterialManager::GetTexture(Word GUID)
{
	Iterator it = mMaterials.find(GUID);
	if(it == mMaterials.end())
		return NULL;
	Material *Mat = (*it).second;

	return Mat->GetDecal();
}