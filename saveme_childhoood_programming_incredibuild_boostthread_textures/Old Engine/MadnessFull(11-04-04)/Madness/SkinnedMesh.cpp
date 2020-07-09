#include "dxstdafx.h"
#include "LoggingInterface.h"
#include "SkinnedMesh.h"
#include "ParsingInterface.h"

HRESULT DerivedAllocateHierarchy::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	D3DXFRAME_DERIVED *fr = new D3DXFRAME_DERIVED;
	AllocateName(Name,&fr->Name);

	D3DXMatrixIdentity(&fr->TransformationMatrix);
	D3DXMatrixIdentity(&fr->CombinedTransformationMatrix);

	fr->pMeshContainer = NULL;
	fr->pFrameSibling = NULL;
	fr->pFrameFirstChild = NULL;

	*ppNewFrame = fr;

	return D3D_OK;
}

HRESULT DerivedAllocateHierarchy::CreateMeshContainer(THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, 
													  CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials,
													  CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED *Container = NULL;
	UINT NumFaces;
	UINT NumBones;
	UINT i;
	LPD3DXMESH Mesh = NULL;
	LPDIRECT3DDEVICE9 Device;
	D3DCAPS9 Caps;
	pMeshData->pMesh->GetDevice(&Device);
	Device->GetDeviceCaps(&Caps);

	if(Caps.VertexShaderVersion < D3DVS_VERSION(1,1))
	{
		gLog->Stream << "Mesh " << Name << ", System Requrement Error: Your video card does not support Vertex Shader 1.1.";
		gLog->Print();

		return E_FAIL;
	}

	if(pMeshData->Type != D3DXMESHTYPE_MESH)
	{
		return E_FAIL;
	}

	Mesh = pMeshData->pMesh;
	if(Mesh->GetFVF() == 0)
	{
		return E_FAIL;
	}

	Container = new D3DXMESHCONTAINER_DERIVED;

	AllocateName(Name,&Container->Name);

	NumFaces = Mesh->GetNumFaces();
	if(!(Mesh->GetFVF() && D3DFVF_NORMAL))
	{
		Container->MeshData.Type = D3DXMESHTYPE_MESH;

		HRESULT hr = Mesh->CloneMeshFVF(Mesh->GetOptions(),Mesh->GetFVF() | D3DFVF_NORMAL,Device,&Container->MeshData.pMesh);
		if(FAILED(hr))
		{
			return hr;
		}

		Mesh = Container->MeshData.pMesh;

		D3DXComputeNormals(Mesh,NULL);
	}
	else
	{
		Container->MeshData.pMesh = Mesh;
		Container->MeshData.Type = D3DXMESHTYPE_MESH;

		Mesh->AddRef();
	}

	Container->NumMaterials = max(1,NumMaterials);
	Container->pMaterials = new D3DXMATERIAL[Container->NumMaterials];
	Container->Textures = new Material[Container->NumMaterials];
	Container->pAdjacency = new DWORD[NumFaces * 3];

	memcpy(Container->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces * 3);
	if(NumMaterials > 0)
	{
		memcpy(Container->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for(i = 0; i < NumMaterials; i++)
		{
			string TexturePath = gParsing->GetPath(Container->pMaterials[i].pTextureFilename);
			if(!Container->Textures[i].Load(Device,TexturePath))
			{
				gLog->Stream << "Mesh Texture " << TexturePath << ": Couldn't load.";
				gLog->Print();
			}
		}
	}
	else
	{
		gLog->Stream << "WARNING: Mesh " << Container->Name << " has no textures.";
		gLog->Print();

		/*
		Container->pMaterials[0].pTextureFilename = NULL;
		memset(&Container->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		Container->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		Container->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		Container->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		Container->pMaterials[0].MatD3D.Specular = Container->pMaterials[0].MatD3D.Diffuse;
		*/
	}

    if(pSkinInfo != NULL)
	{
		Container->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		Container->OrigMesh = Mesh;
		Mesh->AddRef();

		NumBones = pSkinInfo->GetNumBones();
		Container->BoneOffsetMatrices = new D3DXMATRIX[NumBones];
		for(i = 0; i < NumBones; i++)
		{
			Container->BoneOffsetMatrices[i] = *(Container->pSkinInfo->GetBoneOffsetMatrix(i));
		}

		//Container->BoneCombinationBuffer = NULL;

		GenerateSkinnedMesh(Device,Container);
	}
    
	*ppNewMeshContainer = Container;
	
	return D3D_OK;
}

HRESULT DerivedAllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);
	return S_OK;
}

HRESULT DerivedAllocateHierarchy::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED *MeshContainer = (D3DXMESHCONTAINER_DERIVED *)pMeshContainerToFree;
	SAFE_DELETE_ARRAY(MeshContainer->Name);
	SAFE_DELETE_ARRAY(MeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(MeshContainer->pMaterials);
	SAFE_DELETE_ARRAY(MeshContainer->BoneOffsetMatrices);

	SAFE_DELETE_ARRAY(MeshContainer->Textures);
	SAFE_DELETE_ARRAY(MeshContainer->BoneMatrixPointers);
    SAFE_RELEASE(MeshContainer->BoneCombinationBuffer);
	SAFE_RELEASE(MeshContainer->MeshData.pMesh);
	SAFE_RELEASE(MeshContainer->pSkinInfo);
	SAFE_RELEASE(MeshContainer->OrigMesh);
	SAFE_DELETE(MeshContainer);

	return D3D_OK;
}

HRESULT DerivedAllocateHierarchy::GenerateSkinnedMesh(LPDIRECT3DDEVICE9 Device, D3DXMESHCONTAINER_DERIVED *MeshContainer)
{
	D3DCAPS9 Caps;
	HRESULT hr;

	Device->GetDeviceCaps(&Caps);

	if(MeshContainer->pSkinInfo == NULL)
		return E_FAIL;

	SAFE_RELEASE(MeshContainer->MeshData.pMesh);
	SAFE_RELEASE(MeshContainer->BoneCombinationBuffer);

	// First 9 constants are used for other data.  Each 4x3 matrix takes up 3 constants.
    // (96 - 9) /3 i.e. Maximum constant count - used constants 
    UINT MaxMatrices = 26;
	if(MeshContainer->pSkinInfo->GetNumBones() > MaxMatrices)
	{
		gLog->Stream << "Mesh " << MeshContainer->Name << " PERFORMANCE WARNING: The bones in this model exceed the maximum registers in a Vertex Shader 1.1 profile.";
		gLog->Print();
	}
	MeshContainer->NumPaletteEntries = MeshContainer->pSkinInfo->GetNumBones();
	DWORD Flags = D3DXMESHOPT_VERTEXCACHE | D3DXMESH_MANAGED;

	hr = MeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh (
												MeshContainer->OrigMesh,
												Flags,
												MeshContainer->NumPaletteEntries,
												MeshContainer->pAdjacency,
												NULL, NULL, NULL,
												&MeshContainer->NumInfl,
												&MeshContainer->NumAttributeGroups,
												&MeshContainer->BoneCombinationBuffer,
												&MeshContainer->MeshData.pMesh);

	if(FAILED(hr))
		return hr;

	DWORD NewFVF = (MeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
	if(NewFVF != MeshContainer->MeshData.pMesh->GetFVF())
	{
		LPD3DXMESH Mesh;
		hr = MeshContainer->MeshData.pMesh->CloneMeshFVF(MeshContainer->MeshData.pMesh->GetOptions(),NewFVF,Device,&Mesh);
		if(FAILED(hr))
		{
			MeshContainer->MeshData.pMesh->Release();
			MeshContainer->MeshData.pMesh = Mesh;
			Mesh = NULL;
		}
	}

	D3DVERTEXELEMENT9 Decl[MAX_FVF_DECL_SIZE];
    LPD3DVERTEXELEMENT9 DeclCur;

	hr = MeshContainer->MeshData.pMesh->GetDeclaration(Decl);
	if(FAILED(hr))
		return hr;
	DeclCur = Decl;
	while(DeclCur->Stream != 0xff)
	{
		if((DeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) && (DeclCur->UsageIndex == 0))
			DeclCur->Type = D3DDECLTYPE_D3DCOLOR;
		DeclCur++;
	}

	hr = MeshContainer->MeshData.pMesh->UpdateSemantics(Decl);
	if(FAILED(hr))
		return hr;

	return D3D_OK;
}
														

bool DerivedAllocateHierarchy::AllocateName(LPCSTR Name, LPSTR *NewName)
{
	if(Name == NULL) {
		NewName = NULL;
		return false;
	}

	size_t Len = strlen(Name)+1;
	*NewName = new CHAR[Len];
	memcpy(*NewName,Name,Len*sizeof(CHAR));
	
	return true;
}


SkinnedMesh::SkinnedMesh()
{
	mHierarchy = NULL;
	mAnimation = NULL;
}

SkinnedMesh::~SkinnedMesh()
{
	DerivedAllocateHierarchy Alloc;
	D3DXFrameDestroy(mHierarchy,&Alloc);
	SAFE_RELEASE(mAnimation);
}

bool SkinnedMesh::Load(LPDIRECT3DDEVICE9 Device, const string &Filename)
{
	wstring Path = StringToWideString(gParsing->GetPath(Filename));

	DerivedAllocateHierarchy mAllocateHierarchy;

	HRESULT hr = D3DXLoadMeshHierarchyFromX(Path.c_str(),D3DXMESH_MANAGED, Device,&mAllocateHierarchy,NULL,&mHierarchy,&mAnimation);
	if(hr != D3D_OK)
	{
		gLog->Stream << "Mesh " << Filename << ": Could not load.  Return value " << hr;
		gLog->Print();
		return false;
	}

	gLog->Stream << "Mesh " << Filename << ": Loaded.";
	gLog->Print();

	return true;	
}

bool SkinnedMesh::DrawMeshContainer(LPDIRECT3DDEVICE9 Device, LPD3DXMESHCONTAINER MeshContainer)
{
	if(!MeshContainer)
		return false;

	D3DXMESHCONTAINER_DERIVED *Container = (D3DXMESHCONTAINER_DERIVED*)MeshContainer;

//	UINT i = 0;
//	while(Container != NULL)
//	{
	for(UINT i = 0; i < Container->NumAttributeGroups; i++)
	{
		Device->SetTexture(0,Container->Textures[i].GetDecal());

		Container->MeshData.pMesh->DrawSubset(i);
	}
//		Container = (D3DXMESHCONTAINER_DERIVED*)Container->pNextMeshContainer;
//		i++;
//	}

	return true;
}

bool SkinnedMesh::DrawFrame(LPDIRECT3DDEVICE9 Device, LPD3DXFRAME Frame)
{
	LPD3DXMESHCONTAINER MeshContainer;

	MeshContainer = Frame->pMeshContainer;
	while(MeshContainer != NULL)
	{
		DrawMeshContainer(Device,MeshContainer);

		MeshContainer = MeshContainer->pNextMeshContainer;
	}

	if(Frame->pFrameSibling != NULL)
	{
		DrawFrame(Device,Frame->pFrameSibling);
	}
	if(Frame->pFrameFirstChild != NULL)
	{
		DrawFrame(Device,Frame->pFrameFirstChild);
	}
	return true;
}

bool SkinnedMesh::Render(LPDIRECT3DDEVICE9 Device)
{
	if(!DrawFrame(Device,mHierarchy))
	{
		gLog->Print("Failed.");
	}
	return true;
}