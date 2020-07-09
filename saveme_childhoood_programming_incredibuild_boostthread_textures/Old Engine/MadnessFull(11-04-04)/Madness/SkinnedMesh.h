#ifndef SKINNEDMESH_H
#define SKINNEDMESH_H

#include <d3dx9.h>
#include <string>
#include <vector>
#include "Material.h"
using namespace std;

struct D3DXFRAME_DERIVED : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
};

struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
	D3DXMESHCONTAINER_DERIVED() : BoneMatrixPointers(0), BoneOffsetMatrices(0), BoneCombinationBuffer(0),
								OrigMesh(0), AttributeTable(0), NumAttributeGroups(0), NumInfl(0), NumPaletteEntries(0)
	{ }
	Material *Textures;
	
	LPD3DXMESH				OrigMesh;
	LPD3DXATTRIBUTERANGE	AttributeTable;
	DWORD					NumAttributeGroups;
	DWORD					NumInfl;
	LPD3DXBUFFER			BoneCombinationBuffer;
	D3DXMATRIX**			BoneMatrixPointers;
	D3DXMATRIX*				BoneOffsetMatrices;
	DWORD					NumPaletteEntries;
};

class DerivedAllocateHierarchy : public ID3DXAllocateHierarchy
{
private:
	bool AllocateName(LPCSTR Name, LPSTR *NewName);
	HRESULT GenerateSkinnedMesh(LPDIRECT3DDEVICE9 Device, D3DXMESHCONTAINER_DERIVED *MeshContainer);
public:
	STDMETHOD(CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame));
	STDMETHOD(CreateMeshContainer(THIS_ LPCSTR Name, 
				CONST D3DXMESHDATA *pMeshData, 
				CONST D3DXMATERIAL *pMaterials, CONST D3DXEFFECTINSTANCE *pEffectInstances, DWORD NumMaterials, 
				CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER *ppNewMeshContainer));
	STDMETHOD(DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree));
	STDMETHOD(DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree));

};

class SkinnedMesh
{
private:
	LPD3DXFRAME mHierarchy;
	LPD3DXANIMATIONCONTROLLER mAnimation;
	std::vector<D3DXMATRIXA16*> mBlendMatrices;

	bool DrawMeshContainer(LPDIRECT3DDEVICE9 Device, LPD3DXMESHCONTAINER MeshContainer);
	bool DrawFrame(LPDIRECT3DDEVICE9 Device, LPD3DXFRAME Frame);
public:
	SkinnedMesh();
	~SkinnedMesh();

	bool Load(LPDIRECT3DDEVICE9 Device, const string &Name);
	bool Render(LPDIRECT3DDEVICE9 Device);
};

#endif