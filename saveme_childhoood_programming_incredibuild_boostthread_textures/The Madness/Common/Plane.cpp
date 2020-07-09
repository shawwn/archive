#include "GameInterface.h"

#include "Plane.h"
#include "LoggingInterface.h"
#include "Direct3DInterface.h"
#include "EffectInterface.h"
#include "EnumStringInterface.h"
#include "TextureInterface.h"
#include "LightingInterface.h"
#include "BufferInterface.h"
#include "VertexDeclarationInterface.h"
#include "ClientSettingsInterface.h"

Plane::~Plane()
{
	gBuffers.ReleaseVertexBuffer(mVBIndex);
}

bool Plane::Initialize(LPDIRECT3DDEVICE9 Device, float Width, float Depth)
{
	mWidth = Width;
	mDepth = Depth;
	
	mGridSize = 32;

	mPositions[0] = D3DXVECTOR3(-mWidth,0, mDepth);
	mPositions[1] = D3DXVECTOR3( mWidth,0, mDepth); 
	mPositions[2] = D3DXVECTOR3( mWidth,0,-mDepth); 
	mPositions[3] = D3DXVECTOR3(-mWidth,0,-mDepth); 

	mSize = sizeof(D3DXVECTOR3)*4;
	
	D3DVERTEXELEMENT9 Elements[2];
	gDeclarations.Start(Elements).Fill(0,0,D3DDECLTYPE_FLOAT3,0,D3DDECLUSAGE_POSITION,0).End();

	mDeclIndex = gDeclarations.CreateDeclaration(Elements);
	mVBIndex = gBuffers.CreateVertexBuffer(mSize,sizeof(D3DXVECTOR3),D3DPOOL_MANAGED,0);

	LPDIRECT3DVERTEXBUFFER9 VB = gBuffers.AccessVertexBuffer(mVBIndex);
	if(!VB)
		return false;
	
	char *ptr = NULL;
	if(FAILED(VB->Lock(0,mSize,(void**)&ptr,0))) {
		gLog ^ "Plane ERROR: Could not lock vertex buffer.\n";
		SAFE_RELEASE(VB);
		return false;
	}

	memcpy(ptr,mPositions,mSize);

	VB->Unlock();

	/*if(FAILED(Device->CreateVertexDeclaration(Elements,&mDeclaration))) {
		gLog ^ "Plane ERROR: Could not create vertex declaration.\n";
		SAFE_RELEASE(mVB);
		return false;
	}*/

	mEffectIndex = gEffects.Load("Data/Shaders/plane.fx");
	if(mEffectIndex == -1) {
		gLog ^ "Plane WARNING: Could not load plane shader.\n";
	}

	mTextureIndex = gTextures.Load("Data/Textures/grass.jpg");

	return true;
}

#ifndef min
#define min( x, y ) (x < y ? x : y)
#endif

bool SortPred(Light *First, Light *Second) { if(First->Position.y < Second->Position.y) { return true; } return false; }

bool Plane::Render(LPDIRECT3DDEVICE9 Device)
{
	HRESULT hr;
	if(!gBuffers.SetStream(mVBIndex,0,0))
		return false;
	if(!gDeclarations.SetDeclaration(mDeclIndex))
		return false;

	D3DXMATRIX Translation;
	NxVec3 vTrans = NxVec3( floor(gEngine.GetCamera().GetEye().x / mGridSize) * mGridSize,
						  0,floor(gEngine.GetCamera().GetEye().z / mGridSize) * mGridSize);
	D3DXVECTOR3 lightPos;
	D3DXMatrixTranslation(&Translation,vTrans.x,vTrans.y,vTrans.z);
	Device->SetTransform(D3DTS_WORLD,&Translation);

	const D3DXVECTOR3 globalAmbient = D3DXVECTOR3(0.2f,0.2f,0.2f);

	LPD3DXEFFECT eff = gEffects.FindEffect(mEffectIndex);
	if(eff)
	{
		UINT Passes;
		eff->SetMatrix("matWVP",&(Translation * gEngine.GetMatVP()));
		eff->SetFloat("gridSize",mGridSize);
		eff->SetFloatArray("globalAmbient",&globalAmbient.x,3);

		if(FAILED(hr = eff->Begin(&Passes,0))) {
			gLog << "Plane WARNING: Effect failed with [" << DxErrStr(hr) << "]\n";
			return false;
		}

		if(mTextureIndex != -1)
			eff->SetTexture("decalTexture",gTextures.Find(mTextureIndex)->Texture);
		if(FAILED(hr = eff->BeginPass(0))) {
			gLog << "Plane WARNING: Effect failed with [" << DxErrStr(hr) << "]\n";
			return false;
		}
		if(FAILED(Device->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2))) {
			eff->EndPass();
			eff->End();
			return false;
		}
		eff->EndPass();


		if(gSettings.UseDynamicLighting)
		{
		
			gLights.Lights().sort(SortPred);

			UINT i = 0;
			for(LightingInterface::Iterator it = gLights.Begin(); it != gLights.End(); it++) {
				Light *l = (*it);

				if(i > gSettings.NumDynamicLights && gSettings.NumDynamicLights) {
					break;
				}

				lightPos = l->Position - NV3TODV3(vTrans);
				l->Color.x = min(l->Color.x,1.0f);
				l->Color.y = min(l->Color.y,1.0f);
				l->Color.z = min(l->Color.z,1.0f);
				

				eff->SetFloatArray("Light.lightPos",&lightPos.x,3);
				eff->SetFloat("Light.lightIntensity",l->Intensity);
				eff->SetFloatArray("Light.lightColor",&l->Color.x,3);
				eff->SetFloat("Light.lightDecayDistance",l->DecayDistance);
				eff->SetFloatArray("Light.lightDecayAttributes",&l->DecayAttributes.x,3);


				if(FAILED(eff->BeginPass(1))) {
					gLog ^ "Plane ERROR: BeginPass(1) failed.\n";
					return false;
				}

				if(FAILED(Device->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2))) {
					eff->EndPass();
					eff->End();
					return false;
				}

				eff->EndPass();
				i++;
			}
		}

		eff->End();

		Device->SetTransform(D3DTS_WORLD,&gMatIdent);

		return true;
	}
	if(mTextureIndex != -1)
		Device->SetTexture(0,gTextures.Find(mTextureIndex)->Texture);
	Device->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
	
	return true;
}