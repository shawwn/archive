#include "dxstdafx.h"
#include "EffectManager.h"
#include "LoggingInterface.h"
#include "ParsingInterface.h"

EffectManager *gEffects;

EffectManager::EffectManager()
{
}

EffectManager::~EffectManager()
{
	Clear();
}

bool EffectManager::OnResetDevice(LPDIRECT3DDEVICE9 Device)
{
	LoadEffect(Device,gParsing->GetPath("textured.fx"));
	LoadEffect(Device,gParsing->GetPath("white.fx"));
	LoadEffect(Device,gParsing->GetPath("texlight.fx"));
	return true;
}

void EffectManager::OnLostDevice()
{
	Clear();
}

string EffectManager::LoadEffect(LPDIRECT3DDEVICE9 Device, const string &Filename)
{
	if(!Device)
		return false;

	Effect *eff = new Effect;
	if(!eff->Load(Device,Filename))
	{
		delete eff;
		return "";
	}

	return Add(eff);
}

Effect *EffectManager::BeginTexlight(const D3DXMATRIX &ViewProj, const D3DXMATRIX &World, const D3DXMATRIX &WorldInverseTranspose, const D3DXMATRIX &ViewInverse,
					LPDIRECT3DTEXTURE9 Texture, const D3DXVECTOR3 &lightDir, const D3DXVECTOR3 &lightColor, const D3DXVECTOR3 &lightAmbient, 
					const D3DXVECTOR3 &materialDiffuse, const D3DXVECTOR3 &materialSpecular, const float &Shininess, UINT &passes)
{
	if(!Texture)
		return NULL;
    
	Effect *eff = Find("TEXLIGHT");
	if(!eff)
		return NULL;

	float a[4];

	eff->mEffect->SetMatrix("worldViewProj",&ViewProj);
	eff->mEffect->SetMatrix("world",&World);
	eff->mEffect->SetMatrix("worldInverseTranspose",&WorldInverseTranspose);
	eff->mEffect->SetMatrix("viewInverse",&ViewInverse);

	eff->mEffect->SetTexture("diffuseTexture",Texture);

    a[0] = lightDir.x; a[1] = lightDir.y; a[2] = lightDir.z; a[3] = 1;
	eff->mEffect->SetFloatArray("lightDir",a,4);

	a[0] = lightColor.x; a[1] = lightColor.y; a[2] = lightColor.z; a[3] = 1;
	eff->mEffect->SetFloatArray("lightColor",a,4);

	a[0] = lightAmbient.x; a[1] = lightAmbient.y; a[2] = lightAmbient.z; a[3] = 1;
	eff->mEffect->SetFloatArray("lightAmbient",a,4);

	a[0] = materialDiffuse.x; a[1] = materialDiffuse.y; a[2] = materialDiffuse.z; a[3] = 1;
	eff->mEffect->SetFloatArray("materialDiffuse",a,4);

	a[0] = materialSpecular.x; a[1] = materialSpecular.y; a[2] = materialSpecular.z; a[3] = 1;
	eff->mEffect->SetFloatArray("materialSpecular",a,4);

	eff->mEffect->SetFloat("shininess",Shininess);

	if(eff->mEffect->Begin(&passes,0) != D3D_OK)
	{
		gLog->Print("Err");
		return NULL;
	}

	return eff;
}

Effect *EffectManager::BeginTextured(const D3DXMATRIX &WorldViewProj, LPDIRECT3DTEXTURE9 Texture, UINT &passes)
{
	if(!Texture)
		return NULL;
    
	Effect *eff = Find("TEXTURED");
	if(!eff)
		return NULL;

	
	if(eff->mEffect->SetMatrix("WorldViewProj",&WorldViewProj) != D3D_OK)
	{
		return NULL;
	}

	if(eff->mEffect->SetTexture("Decal",Texture) != D3D_OK)
	{
		return NULL;
	}

	if(eff->mEffect->Begin(&passes,0) != D3D_OK)
	{
		gLog->Print("Err");
		return NULL;
	}

	return eff;
}

Effect *EffectManager::BeginWhite(const D3DXMATRIX &WorldViewProj, UINT &passes)
{    
	Effect *eff = Find("WHITE");
	if(!eff)
		return NULL;

	if(eff->mEffect->Begin(&passes,0) != D3D_OK)
	{
		gLog->Print("Err");
		return NULL;
	}

	if(eff->mEffect->SetMatrix("WorldViewProj",&WorldViewProj) != D3D_OK)
	{
		return NULL;
	}

	return eff;
}

string EffectManager::Add(Effect *eff)
{
	if(!eff)
		return "";
	
	D3DXHANDLE Set;
	eff->mEffect->FindNextValidTechnique(NULL,&Set);
	if(FAILED(eff->mEffect->ValidateTechnique(Set)))
		return "";
	eff->mEffect->SetTechnique(Set);

	mEffects[UpperString(eff->GetName())] = eff;
	return eff->GetName();
}

bool EffectManager::Remove(const string &Name)
{
	mEffects.erase(Name);
	return true;

}

bool EffectManager::Remove(Effect *eff)
{
	if(!eff)
		return false;
	mEffects.erase(eff->GetName());
	return true;
}

Effect *EffectManager::Find(const string &Name)
{
	Iterator it = mEffects.find(UpperString(Name));
	if(it == mEffects.end())
		return NULL;
	return (*it).second;
}

void EffectManager::Clear()
{
	for(Iterator it = mEffects.begin(); it != mEffects.end(); it++)
	{
		delete (*it).second;
	}
	mEffects.clear();
}
