#include "Heightmap.h"
#include "LoggingInterface.h"
#include "Macros.h"
#include "NovodexInterface.h"
#include "BufferInterface.h"
#include "VertexDeclarationInterface.h"
using namespace std;

BYTE *Heightmap::Load(LPDIRECT3DDEVICE9 Device, const std::string &Filename, int &Width, int &Height)
{
	if(!Device)
		return NULL;
	LPDIRECT3DTEXTURE9 Tex = NULL;
	D3DXIMAGE_INFO Info;
	if(FAILED(D3DXCreateTextureFromFileEx(Device,Filename.c_str(),-1,-1,1,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,-1,-1,0,&Info,NULL,&Tex))) {
		gLog << "WARNING: Could not open heightmap [" << Filename << "]\n";
		return NULL;
	}


	D3DLOCKED_RECT Rect;
	if(FAILED(Tex->LockRect(0,&Rect,NULL,0))) {
		gLog << "WARNING: Could not retrieve data from heightmap [" << Filename << "]\n";
		return NULL;
	}

	int BitsPerPixel;
	switch(Info.Format)
	{
	case D3DFMT_A8R8G8B8:
	case D3DFMT_X8R8G8B8:
		BitsPerPixel = 32;
		break;
	case D3DFMT_R8G8B8:
		BitsPerPixel = 24;
		break;
	case D3DFMT_R5G6B5:
		BitsPerPixel = 16;
		break;
	case D3DFMT_L8:
		BitsPerPixel = 8;
		break;
	default:
		BitsPerPixel = 8;
		break;
	}

	char *Bits = reinterpret_cast<char *>(Rect.pBits);
	char *At = Bits;

	BYTE *Map = new BYTE[Info.Width*Info.Height];
		
	for(UINT i = 0; i < Info.Height; i++) {
		At = Bits + i*Rect.Pitch;
		for(UINT j = 0; j < Info.Width; j++) {
			switch(BitsPerPixel)
			{
			case 32:
				memcpy(Map+i*Info.Width+j,At+1,1);
				At += 4;
				break;
			case 24:
				memcpy(Map+i*Info.Width+j,At,1);
				At += 3;
				break;
			case 8:
//				if(Rect.Pitch == Info.Width) {
					memcpy(Map,At,Info.Width*Info.Height);
					i = Info.Height;
					j = Info.Width;
			/*	}
				else
				{
					memcpy(Map+i*Info.Width+j,At,Info.Width);
					Map += Info.Width;
					j = Info.Width;
				}*/
				break;
			case 16:
				memcpy(Map+i*Info.Width+j,At,1);
				Map[+i*Info.Width+j] = (Map[+i*Info.Width+j] >> 3);
				At += 2;
				break;
			}
		}
	}


	Width = Info.Width;
	Height = Info.Height;

	gLog << "Heightmap [" << Filename << "]: Loaded.\n";

	return Map;	
}

bool Heightmap::BuildMesh(const std::string &Filename, LPDIRECT3DDEVICE9 Device, UINT &VBIndex, int VertexBufferOffset, UINT &IBIndex, int IndexBufferOffset, UINT &DeclIndex)
{
	BYTE *Map = Load(Device,Filename,mWidth,mHeight);

	if(!Map) 
		return false;

	LPDIRECT3DVERTEXBUFFER9 VB = gBuffers.AccessVertexBuffer(VBIndex);
	LPDIRECT3DINDEXBUFFER9 IB = gBuffers.AccessIndexBuffer(IBIndex);
	LPDIRECT3DVERTEXDECLARATION9 Decl = gDeclarations.AccessDeclaration(DeclIndex);

	if(!Decl)
	{
		D3DVERTEXELEMENT9 Elements[2];
		gDeclarations.Start(Elements).Fill(0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0).End();
		DeclIndex = gDeclarations.CreateDeclaration(Elements);
		if(DeclIndex == -1) {
			gLog ^ "Heightmap [" ^ mFilename ^ "] ERROR: Could not create declaration.\n";
		}
	}

	UINT Stride = sizeof(D3DXVECTOR3);
	if(!VB) {
		gLog << "Heightmap [" << mFilename << "] WARNING: Vertex buffer is NULL, allocating a new one...\n";
		
		

		if((VBIndex = gBuffers.CreateVertexBuffer(mWidth*mHeight*Stride,Stride)) == -1) {
			gLog << "Heightmap [" << mFilename << "] WARNING: Failed to create vertex buffer.\n";
			return false;
		}
		VertexBufferOffset = 0;
	}

	if(!IB) {
		gLog << "Heightmap [" << mFilename << "] WARNING: Index buffer is NULL, allocating a new one...\n";
		
		
		if((IBIndex = gBuffers.CreateIndexBuffer((mWidth-1)*2*(mHeight-1)*3*4,D3DFMT_INDEX32)) == -1)
		{
			gLog << "Heightmap [" << mFilename << "] WARNING: Failed to create index buffer.\n";
			return false;
		}
		IndexBufferOffset = 0;
	}
	

	D3DXVECTOR3 *Verts = NULL;
	if(FAILED(VB->Lock(VertexBufferOffset,mWidth*mHeight*Stride,(void**)&Verts,0))) {
		gLog << "Heightmap [" << mFilename << "] WARNING: Could not lock vertex buffer.\n";
		return false;
	}

	int *Indices = NULL;
	if(FAILED(IB->Lock(IndexBufferOffset,(mWidth-1)*2*(mHeight-1)*3,(void**)&Indices,0)))
	{
		gLog << "Heightmap [" << mFilename << "] WARNING: Could not lock index buffer.\n";
		return false;
	}
	
	int i, j;
	for(i = 0; i < mHeight; i++) {
		for(j = 0; j < mWidth; j++) {
			Verts[i*mWidth+j] = D3DXVECTOR3((float)j,(float)Map[i*mWidth+j]/3,(float)i);
		}
	}


	int Offset = -1;
	for(i = 0; (i+Offset) < (mWidth-1)*(mHeight-1); i++) {
		if(i % (mWidth-1) == 0) {
			Offset++;
		}
		Indices[i*6  ]	= Offset+i;
		Indices[i*6+1]	= Offset+i+1;
		Indices[i*6+2]	= Offset+i+mWidth;

		Indices[i*6+3]	= Offset+i+1;
		Indices[i*6+4]	= Offset+i+mWidth+1;
		Indices[i*6+5]	= Offset+i+mWidth;
	}

	NxTriangleMeshDesc heightDesc;
	heightDesc.numVertices = mWidth * mHeight;
	heightDesc.numTriangles = (mWidth-1)*2*(mHeight-1);
	heightDesc.pointStrideBytes = sizeof(D3DXVECTOR3);
	heightDesc.triangleStrideBytes = 3*sizeof(int);
	heightDesc.points = Verts;
	heightDesc.triangles = Indices;
	heightDesc.heightFieldVerticalAxis = NX_Y;
	heightDesc.heightFieldVerticalExtent = -1000.0f;
	heightDesc.flags = NX_MESH_SMOOTH_SPHERE_COLLISIONS;

	NxTriangleMesh *heightTriangleMesh = NULL;
	LPNXPHYSICSSDK SDK = gPhysics.GetSDK();
	heightTriangleMesh = SDK->createTriangleMesh(heightDesc);

	NxTriangleMeshShapeDesc terrainShapeDesc;
	terrainShapeDesc.meshData = heightTriangleMesh;

	NxActorDesc ActorDesc;
	ActorDesc.shapes.push_back(&terrainShapeDesc);

	gPhysics.GetScene()->createActor(ActorDesc);

	VB->Unlock();

	IB->Unlock();

	SAFE_DELETE_ARRAY(Map);


	return true;
}

void Heightmap::Deinitialize()
{
	mWidth = 0;
	mHeight = 0;
	mFilename = "";
}