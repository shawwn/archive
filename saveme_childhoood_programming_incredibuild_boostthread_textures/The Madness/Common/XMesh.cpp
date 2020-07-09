#include "Player.h"
#include "XMesh.h"
#include "LoggingInterface.h"
#include "Direct3DInterface.h"
#include "LoggingInterface.h"
#include "TextureInterface.h"
#include "EffectInterface.h"
#include "TextureInterface.h"
#include "GameInterface.h"
#include "TimeInterface.h"
#include "Types.h"
using namespace std;

bool XMeshAllocateHierarchy::AllocateName(LPCSTR Name, LPSTR *NewName)
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

HRESULT XMeshAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	XMESHFRAME *fr = new XMESHFRAME;
	AllocateName(Name,&fr->Name);

	D3DXMatrixIdentity(&fr->TransformationMatrix);
	D3DXMatrixIdentity(&fr->CombinedTransform);

	fr->pMeshContainer = NULL;
	fr->pFrameSibling = NULL;
	fr->pFrameFirstChild = NULL;

	*ppNewFrame = fr;

	return D3D_OK;
}
HRESULT XMeshAllocateHierarchy::CreateMeshContainer(THIS_ LPCSTR Name, CONST D3DXMESHDATA *pMeshData, 
													  CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials,
													  CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	XMESHCONTAINER *Container = NULL;
	UINT NumFaces;
	UINT NumBones;
	UINT i;
	LPD3DXMESH Mesh = NULL;
	LPDIRECT3DDEVICE9 Device = gD3D.GetDevice();
	if(!Device)
		return E_FAIL;
	/*
	D3DCAPS9 Caps;
	pMeshData->pMesh->GetDevice(&Device);
	Device->GetDeviceCaps(&Caps);

	if(Caps.VertexShaderVersion < D3DVS_VERSION(1,1))
	{
		gLog->Stream << "Mesh " << Name << ", System Requrement Error: Your video card does not support Vertex Shader 1.1.";
		gLog->Print();

		return E_FAIL;
	}*/

	if(pMeshData->Type != D3DXMESHTYPE_MESH)
	{
		return E_FAIL;
	}

	Mesh = pMeshData->pMesh;
	if(Mesh->GetFVF() == 0)
	{
		return E_FAIL;
	}

	Container = new XMESHCONTAINER;

	AllocateName(Name,&Container->Name);

	NumFaces = Mesh->GetNumFaces();
	if(!(Mesh->GetFVF() & D3DFVF_NORMAL))
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

	Container->NumMaterials = (NumMaterials > 1 ? NumMaterials : 1);
	Container->pMaterials = new D3DXMATERIAL[Container->NumMaterials];
	Container->Textures = new UINT[Container->NumMaterials];
	Container->pAdjacency = new DWORD[NumFaces * 3];

	memcpy(Container->pAdjacency, pAdjacency, sizeof(DWORD) * NumFaces * 3);
	if(NumMaterials > 0)
	{
		memcpy(Container->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for(i = 0; i < NumMaterials; i++)
		{
			if(!Container->pMaterials[i].pTextureFilename)
				continue;
			string TexturePath = Container->pMaterials[i].pTextureFilename;
			//TexturePath.append(Container->pMaterials[i].pTextureFilename);
			if(TexturePath.find_first_of('\\') != string::npos)
				TexturePath = TexturePath.substr(TexturePath.find_first_of('\\')+1);
			TexturePath = "Data/Textures/" + TexturePath;
			
			Container->Textures[i] = gTextures.Load(TexturePath);
			if(Container->Textures[i] == -1)
			{
				gLog << "Mesh Texture " << TexturePath << ": Couldn't load.\n";
			}
		}
	}
	else
	{
		gLog << "WARNING: Mesh " << Container->Name << " has no textures.\n";

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
		Container->BoneOffsetMatrices = new D3DXMATRIXA16[NumBones];
		for(i = 0; i < NumBones; i++)
		{
			Container->BoneOffsetMatrices[i] = *(Container->pSkinInfo->GetBoneOffsetMatrix(i));
		}

		//Container->BoneCombinationBuffer = NULL;

		GenerateSkinnedMesh(Device,Container);
	}
	else
		Container->pSkinInfo = NULL;
    
	*ppNewMeshContainer = Container;
	
	return D3D_OK;
}

HRESULT XMeshAllocateHierarchy::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);
	return S_OK;
}

HRESULT XMeshAllocateHierarchy::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	XMESHCONTAINER *MeshContainer = (XMESHCONTAINER *)pMeshContainerToFree;
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

HRESULT XMeshAllocateHierarchy::GenerateSkinnedMesh(LPDIRECT3DDEVICE9 Device, XMESHCONTAINER *MeshContainer)
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
		gLog << "Mesh " << MeshContainer->Name << " PERFORMANCE WARNING: The bones in this model exceed the maximum registers in a Vertex Shader 1.1 profile.\n";
	}
	MeshContainer->NumPaletteEntries = MeshContainer->pSkinInfo->GetNumBones();
	DWORD Flags = D3DXMESHOPT_VERTEXCACHE | D3DXMESH_MANAGED;

	hr = MeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh (
												MeshContainer->OrigMesh,
												Flags,
												MeshContainer->NumPaletteEntries,
												MeshContainer->pAdjacency,
												NULL, NULL, NULL,
												&MeshContainer->NumInfluences,
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

	NumBones = MeshContainer->pSkinInfo->GetNumBones();

	return D3D_OK;
}


void XBaseMesh::CombineTransforms(XMESHFRAME *Frame, D3DXMATRIX &ParentCombined)
{
	D3DXMATRIX &Local = Frame->TransformationMatrix;
	D3DXMATRIX &Combined = Frame->CombinedTransform;
	Combined = Local * ParentCombined;

	XMESHFRAME *Sibling = (XMESHFRAME *)Frame->pFrameSibling;
	XMESHFRAME *Child = (XMESHFRAME *)Frame->pFrameFirstChild;
	
	if(Sibling)
		CombineTransforms(Sibling,ParentCombined);
	if(Child)
		CombineTransforms(Child,Combined);
}

void XBaseMesh::InterpolateBone(XMESHKEYFRAME &K0, XMESHKEYFRAME &K1, D3DXMATRIX &LocalTransform, float Time)
{
	float t0 = K0.Time;
	float t1 = K1.Time;
	float lerpTime = (Time - t0) / (t1 - t0);

	D3DXVECTOR3 LerpedTranslation;
	D3DXVECTOR3 LerpedScale;
	D3DXQUATERNION LerpedRotation;
	D3DXVec3Lerp(&LerpedTranslation,&K0.Translation,&K1.Translation,lerpTime);
	D3DXVec3Lerp(&LerpedScale,&K0.Scaling,&K1.Scaling,lerpTime);
	D3DXQuaternionSlerp(&LerpedRotation,&K0.Rotation,&K1.Rotation,lerpTime);

	D3DXMATRIX T, S, R;
	D3DXMatrixTranslation(&T,LerpedTranslation.x,LerpedTranslation.y,LerpedTranslation.z);
	D3DXMatrixScaling(&S,LerpedScale.x,LerpedScale.y,LerpedScale.z);
	D3DXMatrixRotationQuaternion(&R,&LerpedRotation);

}

void XBaseMesh::RenderHierarchy(LPDIRECT3DDEVICE9 Device, D3DXFRAME *node, D3DXMATRIX &ParentTransform)
{
	D3DXMATRIX Combined = node->TransformationMatrix * ParentTransform;
	Device->SetTransform(D3DTS_WORLD,&Combined);
	for(UINT i = 0; i < node->pMeshContainer->NumMaterials; i++) {
		node->pMeshContainer->MeshData.pMesh->DrawSubset(i);
	}
	if(node->pFrameSibling)
		RenderHierarchy(Device, node->pFrameSibling,ParentTransform);
	if(node->pFrameFirstChild)
		RenderHierarchy(Device, node->pFrameFirstChild,Combined);
	
}


bool XBaseMesh::DrawMeshContainer(LPDIRECT3DDEVICE9 Device, LPD3DXMESHCONTAINER MeshContainer)
{
	if(!MeshContainer)
		return false;

	XMESHCONTAINER *Container = (XMESHCONTAINER*)MeshContainer;

	if(Container->pSkinInfo)
	{
		UINT MatrixIndex;
		LPD3DXBONECOMBINATION BoneComb;
		D3DXMATRIXA16 matTemp;

		LPD3DXEFFECT eff = gEffects.FindEffect(mSkinnedMeshEffect);
		if(!eff)
			return false;

		BoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(Container->BoneCombinationBuffer->GetBufferPointer());
		D3DXMATRIXA16 matView;
		for(UINT i = 0; i < Container->NumAttributeGroups; i++)
		{
			for(UINT j = 0; j < Container->NumPaletteEntries; j++)
			{
				MatrixIndex = BoneComb[i].BoneId[j];
				if(MatrixIndex != UINT_MAX)
				{
					D3DXMatrixMultiply(&mBoneMatrices[j],&Container->BoneOffsetMatrices[MatrixIndex],Container->BoneMatrixPointers[MatrixIndex]);
				}
			}
			LPTDATA Tex = gTextures.Find(Container->Textures[i]);
			if(Tex)
				Device->SetTexture(0,Tex->Texture);

			D3DXMATRIX Translate, Scale, Rot, Mat;
			D3DXMatrixIdentity(&Translate);
			D3DXMatrixIdentity(&Scale);
			D3DXMatrixIdentity(&Rot);
			D3DXMatrixTranslation(&Translate,mPosition.x,mPosition.y - 5.0f,mPosition.z);
			D3DXMatrixScaling(&Scale,0.02f,0.02f,0.02f);
			D3DXQUATERNION Quat;
	
			NxVec3 View = mView;
			View.y = 0;
			View.normalize();
			float Angle;
			if(View.x > 0)
				Angle = -acos(View.dot(NxVec3(0,0,-1)));
			else
				Angle = acos(View.dot(NxVec3(0,0,-1)));
			D3DXMatrixRotationY(&Rot,Angle);

			Mat = Rot * Scale * Translate;

			eff->SetMatrix("mWorld",&Mat);

			UINT numPasses;
			if(FAILED(eff->SetMatrixArray("mWorldMatrixArray",mBoneMatrices,Container->NumPaletteEntries)))
				return false;
			if(FAILED(eff->SetVector("MaterialDiffuse",&D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f))))
				return false;
			if(FAILED(eff->SetVector("MaterialAmbient",&D3DXVECTOR4(0.2f,0.2f,0.2f,0.2f))))
				return false;
			if(FAILED(eff->SetInt("CurNumBones",Container->NumInfluences-1)))
				return false;


			if(FAILED(eff->SetMatrix("mViewProj",&(gEngine.GetCamera().GetMatView() * gEngine.GetCamera().GetMatProj() ))))
				return false;

			D3DXVECTOR4 vLightDir( 0.0f, 1.0f, -1.0f, 0.0f );
			D3DXVec4Normalize( &vLightDir, &vLightDir );
			if(FAILED(eff->SetVector("lhtDir",&vLightDir)))
				return false;

			if(FAILED(eff->Begin(&numPasses,0)))
				return false;
			for(UINT i = 0; i < numPasses; i++)
			{
				if(FAILED(eff->BeginPass(i)))
					return false;
				Container->MeshData.pMesh->DrawSubset(i);
				eff->EndPass();
			}
			eff->End();
		}
	}
	else if(Container->MeshData.pMesh)
	{
		/*
		LPTDATA Tex = gTextures.Find(Container->Textures[0]);
		if(Tex)
			Device->SetTexture(0,Tex->Texture);
	
		HRESULT hr;
		if(FAILED(hr = Container->MeshData.pMesh->DrawSubset(0))) {
			return false;
		}*/
	}

	return true;

}

bool XBaseMesh::DrawFrame(LPDIRECT3DDEVICE9 Device, LPD3DXFRAME Frame)
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

HRESULT XBaseMesh::SetupBoneMatrixPointers(LPD3DXFRAME Frame)
{	
	HRESULT hr;

	if(Frame->pMeshContainer != NULL)
	{
		hr = SetupBoneMatrixPointersOnMesh(Frame->pMeshContainer);
		if(FAILED(hr))
			return hr;
	}
	if(Frame->pFrameSibling != NULL)
	{
		hr = SetupBoneMatrixPointers(Frame->pFrameSibling);
		if(FAILED(hr))
			return hr;
	}
	if(Frame->pFrameFirstChild != NULL)
	{
		hr = SetupBoneMatrixPointers(Frame->pFrameFirstChild);
		if(FAILED(hr))
			return hr;
	}

	return D3D_OK;
}

HRESULT XBaseMesh::SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER MeshContainerBase)
{
	UINT i, NumBones;
	XMESHFRAME *Frame;

	XMESHCONTAINER *MeshContainer = (XMESHCONTAINER*)MeshContainerBase;
	if(MeshContainer->pSkinInfo == NULL)
		return S_OK;

    NumBones = MeshContainer->pSkinInfo->GetNumBones();
	MeshContainer->BoneMatrixPointers = new D3DXMATRIXA16*[NumBones];
	for(i = 0; i < NumBones; i++)
	{
		Frame = (XMESHFRAME*)D3DXFrameFind(mHierarchy,MeshContainer->pSkinInfo->GetBoneName(i));
		if(Frame == NULL)
			return S_OK;

		MeshContainer->BoneMatrixPointers[i] = (D3DXMATRIXA16 *)&Frame->CombinedTransform;
	}

	return D3D_OK;
}

void XBaseMesh::UpdateFrameMatrices(LPD3DXFRAME FrameBase, LPD3DXMATRIX ParentMatrix)
{
	XMESHFRAME *Frame = (XMESHFRAME*)FrameBase;

	if(ParentMatrix != NULL)
		D3DXMatrixMultiply(&Frame->CombinedTransform,&Frame->TransformationMatrix,ParentMatrix);
	else
		Frame->CombinedTransform = Frame->TransformationMatrix;

	if(Frame->pFrameSibling)
		UpdateFrameMatrices(Frame->pFrameSibling, ParentMatrix);
	if(Frame->pFrameFirstChild)
		UpdateFrameMatrices(Frame->pFrameFirstChild,&Frame->CombinedTransform);
}

bool XBaseMesh::Load(const std::string &Filename)
{
	mSkinnedMeshEffect = gEffects.Load("Data/Shaders/SkinnedMesh.fx");
	if(mSkinnedMeshEffect == -1)
		return false;
	
	HRESULT hr;
	LPDIRECT3DDEVICE9 Device = gD3D.GetDevice();

	hr = D3DXLoadMeshHierarchyFromX(Filename.c_str(),D3DXMESH_MANAGED, Device,&mAllocateHierarchy,NULL,&mHierarchy,&mAnimation);
	if(hr != D3D_OK)
	{
		//gLog << "Mesh " << Filename << ": Could not load.  Return value " << hr;
		LogDxErr(hr,"Could not load mesh");
		return false;
	}

	if(mAnimation)
	{
		hr = SetupBoneMatrixPointers(mHierarchy);
		if(hr != D3D_OK)
		{
			LogDxErr(hr,"Could not load mesh");
			return false;
		}
		LPD3DXANIMATIONSET Jog;
		LPD3DXANIMATIONSET Wave;

		mAnimation->GetAnimationSet(1,&Jog);
		mAnimation->GetAnimationSet(0,&Wave);

		mAnimation->SetTrackAnimationSet(0,Jog);
		mAnimation->SetTrackAnimationSet(1,Wave);

		mAnimation->SetTrackWeight(0,0.01f);
		mAnimation->SetTrackWeight(1,0.99f);

		mAnimation->SetTrackEnable(0,false);
		mAnimation->SetTrackEnable(1,true);

		mAnimation->SetTrackSpeed(0,1.0f);
		mAnimation->SetTrackSpeed(1,1.0f);

		mAnimation->SetTrackPriority(0,D3DXPRIORITY_HIGH);
		mAnimation->SetTrackPriority(1,D3DXPRIORITY_HIGH);
	}

	SAFE_DELETE(mBoneMatrices);
	if(mAllocateHierarchy.NumBones != -1)
		mBoneMatrices = new D3DXMATRIXA16[mAllocateHierarchy.NumBones];

	gLog << "Mesh " << Filename << ": Loaded.\n";
	mFilename = Filename;

	return true;
}

bool XBaseMesh::Render(LPDIRECT3DDEVICE9 Device)
{

	if(!DrawFrame(Device,mHierarchy))
	{
		gLog << "Failed.\n";
	}

	//Device->SetTransform(D3DTS_WORLD,&gEngine->GetIdentityMatrix());
	return true;
}

bool XBaseMesh::Update(const NxVec3 &Position, const NxVec3 &View)
{	
	//if(mAnimation)
		//mAnimation->AdvanceTime(gTime.GetDeltaTimeInSeconds(),NULL);

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	mPosition = Position;
	mView = View;

	UpdateFrameMatrices(mHierarchy,&matWorld);

	return true;
}

bool XBaseMesh::CreateNewInstance(XMesh **Instance)
{
	*Instance = NULL;
	LPD3DXANIMATIONCONTROLLER AC = NULL;

    HRESULT hr;
	XMesh *XM = NULL;

	hr = mAnimation->CloneAnimationController(mAnimation->GetMaxNumAnimationOutputs(),
											mAnimation->GetMaxNumAnimationSets(),
											mAnimation->GetMaxNumTracks(),
											mAnimation->GetMaxNumEvents(),
											&AC);
	if(FAILED(hr)) {
		SAFE_RELEASE(AC);
		return false;
	}

	XM = new XMesh(this);
	if(!XM->Setup(AC)) {
		SAFE_DELETE(XM);
		return false;
	}

	*Instance = XM;
	return true;
}

XMesh::XMesh(XBaseMesh *Base) : mBaseMesh(Base), Player(0)
{

}

bool XMesh::Setup(LPD3DXANIMATIONCONTROLLER AC)
{
	mAC = AC;
	DWORD Tracks = mAC->GetMaxNumTracks();
	for(UINT i = 0; i < Tracks; i++)
		mAC->SetTrackEnable(i,false);

	DWORD NumSets = mAC->GetNumAnimationSets();
	for(UINT i = 0; i < NumSets; i++) {
		LPD3DXANIMATIONSET Set;
		mAC->GetAnimationSet(i,&Set);
		mAnimationSets.push_back(Set);
	}
	return true;
}
/*

void XMesh::UpdateFrames(XMESHFRAME *Frame, D3DXMATRIX *BaseFrame)
{
	D3DXMatrixMultiply(&Frame->CombinedTransform, &Frame->TransformationMatrix, BaseFrame);
	if(Frame->pFrameSibling)
		UpdateFrames((XMESHFRAME *)Frame->pFrameSibling,BaseFrame);
	if(Frame->pFrameFirstChild)
		UpdateFrames((XMESHFRAME *)Frame->pFrameFirstChild,&Frame->CombinedTransform);
}

void XMesh::DrawMeshFrame(XMESHFRAME *Frame)
{
	XMESHCONTAINER *MC = (XMESHCONTAINER *)Frame->pMeshContainer;
	D3DXMATRIX mx;

	if(MC->pSkinInfo == NULL)
		return;


}
*/

bool XMesh::Render(void *Param)
{
	mAC->AdvanceTime(gTime.GetDeltaTimeInSeconds(),NULL);
	mBaseMesh->Update(GetPosition(),GetView());
	return mBaseMesh->Render((LPDIRECT3DDEVICE9)Param);
}

HRESULT XMesh::ResetTime()
{
	return mAC->ResetTime();
}

bool XMesh::CustomUpdate()
{
	mAC->SetTrackWeight(0,mAnimationLerp);
	mAC->SetTrackWeight(1,1 - mAnimationLerp);
	mAnimationLerp += gTime.GetDeltaTimeInSeconds() * 4;
	if(mAnimationLerp > 1.0f)
		mAnimationLerp = 1.0f;
	return true;
}

void XMesh::PlaySet(UINT Which, UINT Track, float Weight, float Speed)
{
	if(Which >= GetNumAnimationSets())
		return;

	LPD3DXANIMATIONSET Set;
	mAC->GetTrackAnimationSet(0,&Set);
	for(UINT i = 0; i < GetNumAnimationSets(); i++) {
		if(mAnimationSets[i] == Set)
			break;
	}
	mPrevSet = i;
	mAC->SetTrackEnable(0,true);
	mAC->SetTrackEnable(1,true);
	mAC->SetTrackAnimationSet(0,mAnimationSets[Which]);
	mAC->SetTrackAnimationSet(1,mAnimationSets[mPrevSet]);
	mAC->SetTrackWeight(0,0.0f);
	mAC->SetTrackWeight(1,1.0f);
	mAC->SetTrackSpeed(0,Speed);
	mAC->SetTrackSpeed(1,Speed);

	mAnimationLerp = 0.0f;
}