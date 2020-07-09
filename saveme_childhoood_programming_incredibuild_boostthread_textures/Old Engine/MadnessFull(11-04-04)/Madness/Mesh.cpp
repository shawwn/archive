#include "dxstdafx.h"
#include "Mesh.h"
#include "ConfigurationInterface.h"
#include "LoggingInterface.h"
#include "MaterialManager.h"

Mesh::Mesh()
{
	mVertexBuffer = NULL;

	mFVF = 0;
	mVertexSize = 0;
}

Mesh::~Mesh()
{
	DestroyMesh();
}

void Mesh::DestroyMesh()
{
	mMeshPath = "";

	if(mVertexBuffer)
		mVertexBuffer->Release();
	mVertexBuffer = NULL;

	mFVF = 0;
	mVertexSize = 0;

	mVertices.clear();

	mBatch.Destroy();

	DestroyObject();
}

bool Mesh::Render(LPDIRECT3DDEVICE9 Device)
{
	if(!Device || !mVertexBuffer)
		return false;

	//gMaterials->Reset();


	if( !(mFVF & D3DFVF_TEX1) && 
		!(mFVF & D3DFVF_TEX2) && 
		!(mFVF & D3DFVF_TEX3) && 
		!(mFVF & D3DFVF_TEX4) )
	{
		Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_DISABLE);
		Device->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);
	}
	else if(mFVF & D3DFVF_TEX1)
	{
		Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
		Device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
		Device->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);
	}
	else if(mFVF & D3DFVF_TEX2)
	{
		Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
		Device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);

		Device->SetTextureStageState(1,D3DTSS_COLORARG1,D3DTA_TEXTURE);
		Device->SetTextureStageState(1,D3DTSS_COLORARG2,D3DTA_CURRENT);
		Device->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_MODULATE);
	}

	mBatch.Render(Device,mVertexBuffer,mFVF,VertexSize());

	return true;
}

bool Mesh::CreateVertexBuffer(LPDIRECT3DDEVICE9 Device)
{
	if(mVertexBuffer)
		mVertexBuffer->Release();

	mFVF = mVertices[0].FVF();

	mVertexSize = 0;
	if(mFVF & D3DFVF_XYZ)
		mVertexSize += sizeof(D3DXVECTOR3);
	if(mFVF & D3DFVF_NORMAL)
		mVertexSize += sizeof(D3DXVECTOR3);
	if(mFVF & D3DFVF_DIFFUSE)
		mVertexSize += sizeof(DWORD);
	
	bool Set = false;
	if(mFVF & D3DFVF_TEX1) {
		mVertexSize += sizeof(TexCoord);
		Set = true;
	}
	if(mFVF & D3DFVF_TEX2 && !Set) {
		mVertexSize += sizeof(TexCoord) * 2;
		Set = true;
	}
	if(mFVF & D3DFVF_TEX3 && !Set) {
		mVertexSize += sizeof(TexCoord) * 3;
		Set = true;
	}
	if(mFVF & D3DFVF_TEX4 && !Set) {
		mVertexSize += sizeof(TexCoord) * 4;
		Set = true;
	}
	
	D3DPOOL Pool = gConfig->GetPool();
	if(FAILED(Device->CreateVertexBuffer(VertexCount()*mVertexSize,0,NULL,Pool,&mVertexBuffer,NULL)))
	{
		gLog->Stream << "BSP " << mMeshPath << ": Could not create the vertex buffer.";
		gLog->Print();
		return false;
	}

	char *verts = NULL;

	if(FAILED(mVertexBuffer->Lock(0,0,(void**)&verts,0)))
	{
		gLog->Stream << "BSP " << mMeshPath << ": Could not access the vertex buffer.";
		gLog->Print();
		mVertexBuffer->Release();
		mVertexBuffer = NULL;
		return false;
	}

	for(int i = 0; i < VertexCount(); i++)
	{
		Vertex *V = &mVertices[i];
		
		memcpy(verts,(void *)&V->Position,sizeof(D3DXVECTOR3));
		verts += sizeof(D3DXVECTOR3);

		if(mFVF & D3DFVF_NORMAL)
		{
			memcpy(verts,(void *)&V->Normal,sizeof(D3DXVECTOR3));
			verts += sizeof(D3DXVECTOR3);
		}

		if(mFVF & D3DFVF_DIFFUSE)
		{
			memcpy(verts,(void *)&V->Color,sizeof(DWORD));
			verts += sizeof(DWORD);
		}

		Set = false;
		if(mFVF & D3DFVF_TEX1)
		{
			for(int i = 0; i < 1; i++)
			{
				memcpy(verts,(void *)&V->Coord[i],sizeof(TexCoord));
				verts += sizeof(TexCoord);
			}
			Set = true;
		}
		if(mFVF & D3DFVF_TEX2 && !Set)
		{
			for(int i = 0; i < 2; i++)
			{
				memcpy(verts,(void *)&V->Coord[i],sizeof(TexCoord));
				verts += sizeof(TexCoord);
			}
			Set = true;
		}
		if(mFVF & D3DFVF_TEX3 && !Set)
		{
			for(int i = 0; i < 3; i++)
			{
				memcpy(verts,(void *)&V->Coord[i],sizeof(TexCoord));
				verts += sizeof(TexCoord);
			}
			Set = true;
		}
		if(mFVF & D3DFVF_TEX4 && !Set)
		{
			for(int i = 0; i < 4; i++)
			{
				memcpy(verts,(void *)&V->Coord[i],sizeof(TexCoord));
				verts += sizeof(TexCoord);
			}
			Set = true;
		}

	}

	if(FAILED(mVertexBuffer->Unlock()))
	{
		gLog->Stream << "BSP " << mMeshPath << ": Could not unlock the vertex buffer.";
		gLog->Print();
		mVertexBuffer->Release();
		mVertexBuffer = NULL;
		return false;
	}

    return true;
}