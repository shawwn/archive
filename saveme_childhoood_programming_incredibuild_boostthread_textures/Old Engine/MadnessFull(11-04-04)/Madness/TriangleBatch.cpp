#include "dxstdafx.h"
#include "TriangleBatch.h"
#include "ConfigurationInterface.h"
#include "LoggingInterface.h"
#include "MaterialManager.h"
#include "Direct3DError.h"
#include "EffectManager.h"
#include "Engine.h"

void TriangleBatch::Add(IndexedFace *F)
{
	int count = GroupCount();

	bool Equal;
	for(int i = 0; i < count; i++)
	{
		if(mGroup[i][0]->CoordIDCount() != F->CoordIDCount())
			continue;

		Equal = true;

		for(int j = 0; j < mGroup[i][0]->CoordIDCount(); j++)
		{
			if(mGroup[i][0]->CoordID[j] != F->CoordID[j]) {
				Equal = false;
				break;
			}
		}
		if(Equal)
		{
			mGroup[i].push_back(F);
			if(F->Min() < mMin[i])
				mMin[i] = F->Min();
			if(F->Max() > mMax[i])
				mMax[i] = F->Max();
			return;
		}
	}

	std::vector<IndexedFace *> Vec;
	mGroup.push_back(Vec);

	mGroup[GroupCount()-1].push_back(F);
	mMin.push_back(F->Min());
	mMax.push_back(F->Max());


	return;
}

bool TriangleBatch::Rasterize(LPDIRECT3DDEVICE9 Device)
{
	if(!Device)
	{
		return false;
	}
	if(BufferCount() > 0)
		ReleaseBuffers();
	for(int i = 0; i < GroupCount(); i++)
	{
		LPDIRECT3DINDEXBUFFER9 Buffer;
		D3DPOOL Pool = gConfig->GetPool();
		if(FAILED(Device->CreateIndexBuffer((int)mGroup[i].size()*3*sizeof(WORD),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,Pool,&Buffer,NULL)))
		{
			gLog->Stream << "TriangleBatch Warning: Could not create index buffer of size " << (int)mGroup[i].size()*3*sizeof(WORD) << " with group " << i << " with a CoordID1 of " << mGroup[i][0]->CoordID[0];
			gLog->Print();
			continue;
		}
		char *ind = NULL;
		if(FAILED(Buffer->Lock(0,0,(void **)&ind,0)))
		{
			gLog->Stream << "TriangleBatch Warning: Could not create index buffer of size " << (int)mGroup[i].size()*3*sizeof(WORD) << " with group " << i << " with a CoordID1 of " << mGroup[i][0]->CoordID[0];
			gLog->Print();
			continue;
		}

		for(int j = 0; j < (int)mGroup[i].size(); j++)
		{
			memcpy(ind,mGroup[i][j]->Indices,sizeof(WORD)*3);
			ind += sizeof(WORD)*3;
		}

		Buffer->Unlock();

        mBuffer.push_back(Buffer);		
	}
	return true;
}

bool TriangleBatch::Render(LPDIRECT3DDEVICE9 Device, LPDIRECT3DVERTEXBUFFER9 VertexBuffer, DWORD FVF, int VertexSize)
{
	if(!Device || !VertexBuffer)
		return false;

	Device->SetStreamSource(0,VertexBuffer,0,VertexSize);
	Device->SetFVF(FVF);

	for(int i = 0; i < GroupCount(); i++)
	{
		Device->SetIndices(mBuffer[i]);
		for(int j = 0; j < mGroup[i][0]->CoordIDCount(); j++)
		{
			gMaterials->SetTexture(Device,j,mGroup[i][0]->CoordID[j]);
		}
		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,mMin[i],mMax[i]-mMin[i],0,(int)mGroup[i].size());

		/*if(mGroup[i][0]->CoordIDCount() == 0) {
			Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,mMin[i],mMax[i]-mMin[i],0,(int)mGroup[i].size());
			continue;
		}
		LPDIRECT3DTEXTURE9 Tex = gMaterials->GetTexture(mGroup[i][0]->CoordID[0]);
		if(!Tex) {
			Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,mMin[i],mMax[i]-mMin[i],0,(int)mGroup[i].size());
			continue;
		}
		D3DXMATRIX WorldViewProj = gEngine->GetViewMatrix() * gEngine->GetProjMatrix();
		UINT passes = 0;
		//Effect *eff = gEffects->BeginTextured(WorldViewProj,Tex,passes);
		//Effect *eff = gEffects->BeginWhite(WorldViewProj,passes);

		D3DXMATRIX World = gEngine->GetWorldMatrix();
		D3DXMATRIX WorldInverseTranspose;
		float det = D3DXMatrixDeterminant(&World);
		D3DXMatrixInverse(&World,&det,&World);
		D3DXMatrixTranspose(&World,&World);
		D3DXMATRIX ViewInverse = gEngine->GetViewMatrix();
		det = D3DXMatrixDeterminant(&ViewInverse);
		D3DXMatrixInverse(&ViewInverse,&det,&ViewInverse);
		D3DXVECTOR3 lightDir = D3DXVECTOR3(-1,-1,0);
		D3DXVECTOR3 lightColor = D3DXVECTOR3(1,1,1);
		D3DXVECTOR3 lightAmbient = D3DXVECTOR3(0.2f,0.2f,0.2f);
		D3DXVECTOR3 materialDiffuse = D3DXVECTOR3(1,1,1);
		D3DXVECTOR3 materialSpecular = D3DXVECTOR3(1,1,1);
		float Shininess = 65;

		Effect *eff = gEffects->BeginTexlight(WorldViewProj,World,WorldInverseTranspose,ViewInverse,
			Tex,lightDir,lightColor,lightAmbient,materialDiffuse,materialSpecular,Shininess,passes);
		if(!eff) {
			gMaterials->SetTexture(Device,0,mGroup[i][0]->CoordID[0]);
			Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,mMin[i],mMax[i]-mMin[i],0,(int)mGroup[i].size());
			continue;
		}
		for(UINT j = 0; j < passes; j++)
		{
			if(eff->mEffect->BeginPass(j) != D3D_OK) {
				continue;
			}
			Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,mMin[i],mMax[i]-mMin[i],0,(int)mGroup[i].size());
			eff->mEffect->EndPass();
		}
		eff->mEffect->End();*/

		/*T("DrawIndexedPrimitive",Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			0,
			mGroup[i][0]->Indices[0],
			mGroup[i][mGroup[i].size()-1]->Indices[2]-mGroup[i][0]->Indices[0],
			0,
			(int)mGroup[i].size()));*/

		//Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,BaseVertex,MinIndex,NumVertices,StartIndex,PrimitiveCount);
//		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,mMin[i],mMax[i]-mMin[i],0,(int)mGroup[i].size());

	}

	return true;
}

void TriangleBatch::DestroyTriangles()
{
	int i, j;
	for(i = 0; i < GroupCount(); i++)
	{
		for(j = 0; j < (int)mGroup[i].size(); j++) {
			delete mGroup[i][j];
		}
	}
	mGroup.clear();
}

void TriangleBatch::ReleaseBuffers()
{
	int i;
	for(i = 0; i < BufferCount(); i++)
	{
		if(mBuffer[i])
			mBuffer[i]->Release();
	}
	mBuffer.clear();
}