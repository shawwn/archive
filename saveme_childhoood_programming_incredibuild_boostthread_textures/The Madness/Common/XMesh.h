#ifndef XMESH_H
#define XMESH_H

#include "Player.h"
#include <d3dx9.h>
#include <string>
#include "Macros.h"
#include <windows.h>

class XMesh;

struct XMESHFRAME : public D3DXFRAME
{
	D3DXMATRIX CombinedTransform;
};

struct XMESHKEYFRAME
{
	float Time;
	D3DXQUATERNION Rotation;
	D3DXVECTOR3 Scaling;
	D3DXVECTOR3 Translation;
};

struct XMESHCONTAINER : public D3DXMESHCONTAINER
{	
	XMESHCONTAINER() : Textures(NULL), BoneMatrixPointers(0), BoneOffsetMatrices(0), BoneCombinationBuffer(0),
								OrigMesh(0), AttributeTable(0), NumAttributeGroups(0), NumInfluences(0), NumPaletteEntries(0) { }
	~XMESHCONTAINER() { SAFE_DELETE(Textures); }
	UINT*					Textures;
	LPD3DXMESH				OrigMesh;
	LPD3DXATTRIBUTERANGE	AttributeTable;
	DWORD					NumAttributeGroups;
	DWORD					NumInfluences;
	LPD3DXBUFFER			BoneCombinationBuffer;
	D3DXMATRIXA16**			BoneMatrixPointers;
	D3DXMATRIXA16*			BoneOffsetMatrices;
	DWORD					NumPaletteEntries;
};

class XMeshAllocateHierarchy : public ID3DXAllocateHierarchy
{
private:
	bool AllocateName(LPCSTR Name, LPSTR *NewName);
	HRESULT GenerateSkinnedMesh(LPDIRECT3DDEVICE9 Device, XMESHCONTAINER *MeshContainer);
public:
	UINT NumBones;

	XMeshAllocateHierarchy() : NumBones(-1) { }

	STDMETHOD(CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame));
	STDMETHOD(CreateMeshContainer(THIS_ LPCSTR Name, 
				CONST D3DXMESHDATA *pMeshData, 
				CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, 
				CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer));
	STDMETHOD(DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree));
	STDMETHOD(DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree));
};

class XBaseMesh
{
private:
	LPD3DXFRAME mHierarchy;
	LPD3DXANIMATIONCONTROLLER mAnimation;
	LPD3DXMATRIX mBoneMatrices;
	UINT mSkinnedMeshEffect;
	XMeshAllocateHierarchy mAllocateHierarchy;
	std::string mFilename;
	NxVec3 mPosition;
	NxVec3 mView;

	void CombineTransforms(XMESHFRAME *Frame, D3DXMATRIX &ParentCombined);
	void InterpolateBone(XMESHKEYFRAME &K0, XMESHKEYFRAME &K1, D3DXMATRIX &LocalTransform, float Time);
	void RenderHierarchy(LPDIRECT3DDEVICE9 Device, D3DXFRAME *node, D3DXMATRIX &ParentTransform);

	
	bool DrawMeshContainer(LPDIRECT3DDEVICE9 Device, LPD3DXMESHCONTAINER MeshContainerBase);
	bool DrawFrame(LPDIRECT3DDEVICE9 Device, LPD3DXFRAME Frame);
	HRESULT SetupBoneMatrixPointers(LPD3DXFRAME Frame);
	HRESULT SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER MeshContainerBase);

	void UpdateFrameMatrices(LPD3DXFRAME FrameBase, LPD3DXMATRIX ParentMatrix);
public:
	XBaseMesh() : mBoneMatrices(NULL), mAnimation(NULL), mPosition(NxVec3(0,0,0)), mView(NxVec3(0,0,0)) { }

	bool Load(const std::string &Filename);
	bool Render(LPDIRECT3DDEVICE9 Device);
	bool Update(const NxVec3 &Position, const NxVec3 &View);
	bool CreateNewInstance(XMesh **Instance);
	
	const std::string &GetFilename() { return mFilename; }
};

class XMesh : public Player
{
	friend class XBaseMesh;
private:
	bool Setup(LPD3DXANIMATIONCONTROLLER AC);
	void UpdateFrames(XMESHFRAME *Frame, D3DXMATRIX *BaseFrame);
	void DrawFrames(XMESHFRAME *Frame);
	void DrawMeshFrame(XMESHFRAME *Frame);

	XBaseMesh *mBaseMesh;
	LPD3DXANIMATIONCONTROLLER mAC;
	std::vector<ID3DXAnimationSet*> mAnimationSets;
	float mAnimationLerp;
	UINT mPrevSet;

public:
	XMesh(XBaseMesh *Base);

	XBaseMesh *GetBaseMesh() { return mBaseMesh; }   

	HRESULT ResetTime();

	bool Render(void *Param);
	bool CustomUpdate();
	LPD3DXANIMATIONCONTROLLER GetAnimationController() { return mAC; }

	void PlaySet(UINT Which, UINT Track, float Weight = 1.0f, float Speed = 1.0f);

	UINT GetNumAnimationSets() const { return mAC->GetNumAnimationSets(); }
};

#endif