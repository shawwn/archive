#ifndef _QUAKE3BSP_H
#define _QUAKE3BSP_H

#include <d3dx9.h>
#include <vector>
#include <string>
#include "Mesh.h"
#include "Vertex.h"
#include "Bitset.h"
using namespace std;

class Vector3i
{
public:
	Vector3i() : x(0), y(0), z(0)
	{ }
	int x;
	int y;
	int z;
};

class bspHeader
{
public:
	bspHeader() { memset((void*)&ID,0,4); Version = 0; }
	char ID[4];  // "IBSP"
	int Version; // 0x2e
	operator bool() { if(strcmp(ID,"IBSP") != 0 || Version != 0x2e) { return false; } return true; }
};

class bspLump
{
public:
	bspLump() { Offset = -1; Length = -1; }
	int Offset;
	int Length;
	operator bool() { if(Length == -1) { return false; } return true; }
};

class bspVertex
{
public:
	bspVertex() { memset((void *)&Color, 0, 4); }
	D3DXVECTOR3 Position;
	TexCoord TextureCoord;
	TexCoord LightmapCoord;
	D3DXVECTOR3 Normal;
	BYTE Color[4];


};

class bspFace
{
public:
	int TextureID;		// Index into the texture array
	int Effect;			// Index for effect (-1 for N/A)
	int Type;			// 1=polygon, 2=patch, 3=mesh, 4=billboard
	int StartVertIndex;	// The starting index into this face's first vertex
	int NumOfVerts;		// Number of vertices for this face
	int MeshVertIndex;	// The index into the first mesh vertex
	int NumMeshVerts;	// The number of mesh vertices
	int LightmapID;		// The texture index for the lightmap
	int LMapCorner[2];	// The face's lightmap corner in the image
	int LMapSize[2];	// Size of the lightmap section
	D3DXVECTOR3 LMapPos;	// The 3D origin of the lightmap
	D3DXVECTOR3 LMapVecs[2];// The 3D space for s and t unit vectors
	D3DXVECTOR3 Normal;	// The face normal
	int Size[2];		// The brezier pach dimensions
};

class bspTexture
{
public:
	char Name[64];
	int Flags;
	int Contents;
};

class bspLightmap
{
public:
	Byte ImageBits[128][128][3];
};

class bspNode
{
public:
	int Plane;
	int Front;
	int Back;
	Vector3i Min;
	Vector3i Max;
};

class bspLeaf
{
public:
	int Cluster;
	int Area;
	Vector3i Min;
	Vector3i Max;
	int LeafFace;
	int NumLeafFaces;
	int LeafBrush;
	int NumLeafBrushes;
};

class bspPlane
{
public:
	D3DXVECTOR3 Normal;
	float Distance;
};

class bspVisData
{
public:
	bspVisData()
	{ pBitsets = NULL; }
	int NumClusters;
	int BytesPerCluster;
	byte *pBitsets;
};

enum eLumps
{
	kEntities = 0,
	kTextures,
	kPlanes,
	kNodes,
	kLeafs,
	kLeafFaces,
	kLeafBrushes,
	kModels,
	kBrushes,
	kBrushSides,
	kVertices,
	kMeshVerts,
	kShaders,
	kFaces,
	kLightmaps,
	kLightVolumes,
	kVisData,
	kMaxLumps
};

#define BSP_SWAPXY  (1 << 0)
#define BSP_SWAPYZ  (1 << 1)
#define BSP_SWAPZX  (1 << 2)
#define BSP_NEGATEX (1 << 3)
#define BSP_NEGATEY (1 << 4)
#define BSP_NEGATEZ (1 << 5)

class Quake3BSP : public Mesh
{
private:
	bspLump mLumps[kMaxLumps];

	std::vector<bspFace *> mFaces;
	std::vector<bspNode *> mNodes;
	std::vector<bspLeaf *> mLeafs;
	std::vector<int> mLeafFaces;
	std::vector<bspPlane *> mPlanes;
	std::vector<Word> mTextures;
	std::vector<Word> mLightmaps;
	bspVisData mClusters;

	Bitset mFacesDrawn;

	bool mUseLightmaps;

	void DestroyBSP();
	void ChangeGamma(byte *pImage, int size, float factor);

	int FaceCount() { return (int)mFaces.size(); }
	int NodeCount() { return (int)mNodes.size(); }
	int LeafCount() { return (int)mLeafs.size(); }
	int LeafFaceCount() { return (int)mLeafFaces.size(); }
	int PlaneCount() { return (int)mPlanes.size(); }
	int TextureCount() { return (int)mTextures.size(); }
	int LightmapCount() { return (int)mLightmaps.size(); }

	bool CreateIndexBuffer(LPDIRECT3DDEVICE9 Device);
	bool LoadLightmaps(LPDIRECT3DDEVICE9 Device, float Factor);
public:
	Quake3BSP();
	~Quake3BSP();
	bool Load(LPDIRECT3DDEVICE9 Device, const string &Filename, DWORD TransformFlags, bool UseLightmaps, float LightmapIntensity);
	bool Render(LPDIRECT3DDEVICE9 Device);
	bool Rasterize(LPDIRECT3DDEVICE9 Device);

	void ToggleUseLightmaps() { mUseLightmaps = !mUseLightmaps; }
	void SetUseLightmaps(const bool &Set) { mUseLightmaps = Set; }
	bool GetUseLightmaps() { return mUseLightmaps; }
};

#endif