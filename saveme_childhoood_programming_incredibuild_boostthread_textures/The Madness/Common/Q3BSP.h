#ifndef Q3BSP_H
#define Q3BSP_H

#include <fstream>
#include <string>
#include <d3dx9.h>
#include "Types.h"
#include "Macros.h"

struct Q3DirEntry
{
	int Offset;
	int Length;
};

struct Q3Header
{
	char Magic[4];	// "IBSP"
	int Version;    // 0x2E
	Q3DirEntry Entries[17];
};

struct Q3Entities
{
};

struct Q3Textures
{
	char Name[64];
	int Flags;
	int Contents;
};

struct Q3Planes
{
	float Normal[3];
	float Distance;
};

struct Q3Nodes
{
	int Plane;
	int Children[2];
	int Mins[3];
	int Maxs[3];
};

struct Q3Leafs
{
	int Cluster;
	int Area;
	int Mins[3];
	int Maxs[3];
	int IndexLeafface;
	int NumLeafFaces;
	int IndexLeafBrush;
	int NumLeafBrushes;
};

struct Q3LeafFaces
{
	int LeafFaceIndex;
};

struct Q3LeafBrushes
{
	int LeafBrushIndex;
};

struct Q3Models
{
	float Mins[3];
	float Maxs[3];
	int IndexFace;
	int NumFaces;
	int IndexBrush;
	int NumBrushes;
};

struct Q3Brushes
{
	int IndexBrushSide;
	int NumBrushSides;
	int IndexTexture;
};

struct Q3BrushSides
{
	int Plane;
	int	IndexTexture;
};

struct Q3Vertices
{
	float Position[3];
	float Texcoords[2][2];
	float Normal[3];
	BYTE Color[4];
};

struct Q3MeshVertices
{
	int Index;
};

struct Q3Effects
{
	char Name[64];
	int Brush;
	int Unknown; // Always 5
};

struct Q3Faces
{
	int IndexTexture;
	int IndexEffect;
	int Type;			// 1 = Polygon, 2 = Patch, 3 = Mesh, 4 = Billboard
	int IndexVertex;
	int NumVertices;
	int IndexMeshVertex;
	int NumMeshVertices;
	int IndexLightmap;
	int LightmapStart[2];
	int LightmapSize[2];
	float LightmapOrigin[3];
	float LightmapVectors[2][3];
	float SurfaceNormal[3];
	int Size[2];
};

struct Q3Lightmaps
{
	BYTE Data[128][128][3];
};

struct Q3LightVolumes
{
	BYTE Ambient[3];
	BYTE Directional[3];
	BYTE Dir[2];
};

struct Q3VisData
{
	int NumVectors;
	int VectorSize;
	BYTE *Vecs;
};

class Q3BSP
{
private:
	std::string mFilename;

	Q3Header mHeader;

	Q3Textures *mTextures;
	Q3Planes *mPlanes;
	Q3Nodes *mNodes;
	Q3Leafs *mLeafs;
	Q3LeafFaces *mLeafFaces;
	Q3LeafBrushes *mLeafBrushes;
	Q3Models *mModels;
	Q3Brushes *mBrushes;
	Q3BrushSides *mBrushSides;
	Q3Vertices *mVertices;
	Q3MeshVertices *mMeshVertices;
	Q3Effects *mEffects;
	Q3Faces *mFaces;
	Q3Lightmaps *mLightmaps;

    UINT mNumTextures;
	UINT mNumPlanes;
	UINT mNumNodes;
	UINT mNumLeafs;
	UINT mNumLeafFaces;
	UINT mNumLeafBrushes;
	UINT mNumModels;
	UINT mNumBrushes;
	UINT mNumBrushSides;
	UINT mNumVertices;
	UINT mNumMeshVertices;
	UINT mNumEffects;
	UINT mNumFaces;
	UINT mNumLightmaps;

	UINT *mTextureIndices;
	UINT mCaulkIndex;

	unsigned short *mVertexIndices;

	template<class Q3>
		UINT InitPart(std::ifstream &opened, Q3 *&Dest, UINT &Number, UINT Index) { 
			Number = mHeader.Entries[Index].Length / sizeof(Q3); 
			SAFE_DELETE_ARRAY(Dest);
			Dest = new Q3[Number];
			opened.seekg(mHeader.Entries[Index].Offset);
			opened.read((char *)Dest,mHeader.Entries[Index].Length); 
			return Number; }

public:
	Q3BSP() : mTextures(NULL), mPlanes(NULL), mNodes(NULL), mLeafs(NULL), mLeafFaces(NULL), mLeafBrushes(NULL), mModels(NULL), mBrushes(NULL), mBrushSides(NULL),
				mVertices(NULL), mMeshVertices(NULL), mEffects(NULL), mFaces(NULL), mLightmaps(NULL),
				mNumTextures(0), mNumPlanes(0), mNumNodes(0), mNumLeafs(0), mNumLeafFaces(0), mNumLeafBrushes(0), mNumModels(0), mNumBrushes(0), mNumBrushSides(0),
				mNumVertices(0), mNumMeshVertices(0), mNumEffects(0), mNumFaces(0), mNumLightmaps(0),
				mTextureIndices(NULL), mCaulkIndex(0), mVertexIndices(NULL)
	{ }
	~Q3BSP() {
		SAFE_DELETE_ARRAY(mTextures);
		SAFE_DELETE_ARRAY(mPlanes);
		SAFE_DELETE_ARRAY(mNodes);
		SAFE_DELETE_ARRAY(mLeafs);
		SAFE_DELETE_ARRAY(mLeafFaces);
		SAFE_DELETE_ARRAY(mLeafBrushes);
		SAFE_DELETE_ARRAY(mModels);
		SAFE_DELETE_ARRAY(mBrushes);
		SAFE_DELETE_ARRAY(mBrushSides);
		SAFE_DELETE_ARRAY(mVertices);
		SAFE_DELETE_ARRAY(mMeshVertices);
		SAFE_DELETE_ARRAY(mEffects);
		SAFE_DELETE_ARRAY(mFaces);
		SAFE_DELETE_ARRAY(mLightmaps);

		SAFE_DELETE_ARRAY(mTextureIndices);
		SAFE_DELETE_ARRAY(mVertexIndices);
	}

	inline UINT GetNumTextures()		const { return mNumTextures; }
	inline UINT GetNumPlanes()			const { return mNumPlanes; }
	inline UINT GetNumNodes()			const { return mNumNodes; }
	inline UINT GetNumLeafs()			const { return mNumLeafs; }
	inline UINT GetNumLeafFaces()		const { return mNumLeafFaces; }
	inline UINT GetNumLeafBrushes()		const { return mNumLeafBrushes; }
	inline UINT GetNumModels()			const { return mNumModels; }
	inline UINT GetNumBrushes()			const { return mNumBrushes; }
	inline UINT GetNumBrushSides()		const { return mNumBrushSides; }
	inline UINT GetNumVertices()		const { return mNumVertices; }
	inline UINT GetNumMeshVertices()	const { return mNumMeshVertices; }
	inline UINT GetNumEffects()			const { return mNumEffects; }
	inline UINT GetNumFaces()			const { return mNumFaces; }
	inline UINT GetNumLightmaps()		const { return mNumLightmaps; }

	bool Load(const std::string &Filename);
	bool Render(LPDIRECT3DDEVICE9 Device);
};

#endif