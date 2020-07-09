#include "dxstdafx.h"
#include "Quake3BSP.h"
#include "LoggingInterface.h"
#include "MaterialManager.h"
#include "ConfigurationInterface.h"

Quake3BSP::Quake3BSP()
{
	mUseLightmaps = false;
}

Quake3BSP::~Quake3BSP()
{
	DestroyBSP();
}

void Quake3BSP::ChangeGamma(byte *pImage, int size, float factor)
{
	for(int i = 0; i < size / 3; i++, pImage += 3)
	{
		float scale = 1.0f;
		float temp = 0.0f;
		float r = 0;
		float g = 0;
		float b = 0;

		r = (float)pImage[0];
		g = (float)pImage[1];
		b = (float)pImage[2];

		r = r * factor / 255.0f;
		g = g * factor / 255.0f;
		b = b * factor / 255.0f;
		
		if(r > 1.0f && (temp = (1.0f/r)) < scale) scale=temp;
		if(g > 1.0f && (temp = (1.0f/g)) < scale) scale=temp;
		if(b > 1.0f && (temp = (1.0f/b)) < scale) scale=temp;
		scale*=255.0f;		
		r*=scale;	g*=scale;	b*=scale;

		pImage[0] = (byte)r;
		pImage[1] = (byte)g;
		pImage[2] = (byte)b;
	}
}

bool Quake3BSP::Load(LPDIRECT3DDEVICE9 Device, const string &Filename, DWORD TransformFlags, bool UseLightmaps, float Factor)
{
	gLog->Stream << "BSP: Loading " << Filename;
	gLog->Print();

	if(!Device)
	{
		gLog->Stream << "BSP " << Filename << ": Invalid device.";
		gLog->Print();
		return false;
	}

	if(mMeshPath != "")
		DestroyBSP();

	mMeshPath = Filename;
	int Len = (int)mMeshPath.length();
	(mMeshPath.find_last_of('.') != std::string::npos ?
		mMeshPath = mMeshPath.substr(0,mMeshPath.find_last_of('.')) : mMeshPath );
	mMeshPath = mMeshPath + ".bsp";

	mName = mMeshPath.substr(0,mMeshPath.find_last_of('.'));
	mName = mName.substr(mName.find_last_of('/')+1,mName.size());

	FILE *fp = NULL;
	if((fp = fopen(mMeshPath.c_str(),"rb")) == NULL)
	{
		gLog->Stream << "BSP " << mMeshPath << ": Could not open file.";
		gLog->Print();
		DestroyBSP();
		return false;
	}

	bspHeader header;
	fread(&header,1,sizeof(bspHeader),fp);
	fread(&mLumps,kMaxLumps,sizeof(bspLump),fp);

	int NumVertices			= mLumps[kVertices].Length / sizeof(bspVertex);
	int NumFaces			= mLumps[kFaces].Length / sizeof(bspFace);
	int NumLeafs			= mLumps[kLeafs].Length / sizeof(bspLeaf);
	int NumLeafFaces		= mLumps[kLeafFaces].Length / sizeof(int);
	int NumNodes			= mLumps[kNodes].Length / sizeof(bspNode);
	int NumPlanes			= mLumps[kPlanes].Length / sizeof(bspPlane);
	int NumMaterials		= mLumps[kTextures].Length / sizeof(bspTexture);

	int i;

	fseek(fp,mLumps[kVertices].Offset,SEEK_SET);
	bspVertex *Vertices = new bspVertex[NumVertices];
	fread(Vertices,NumVertices,sizeof(bspVertex),fp);
	for(i = 0; i < NumVertices; i++) 
	{ 
		if(TransformFlags & BSP_SWAPXY)
		{
			float Temp = Vertices[i].Position.x;
			Vertices[i].Position.x = Vertices[i].Position.y;
			Vertices[i].Position.y = Temp;

			Temp = Vertices[i].Normal.x;
			Vertices[i].Normal.x = Vertices[i].Normal.y;
			Vertices[i].Normal.y = Temp;
		}
		if(TransformFlags & BSP_SWAPYZ)
		{
			float Temp = Vertices[i].Position.y;
			Vertices[i].Position.y = Vertices[i].Position.z;
			Vertices[i].Position.z = Temp;

			Temp = Vertices[i].Normal.y;
			Vertices[i].Normal.y = Vertices[i].Normal.z;
			Vertices[i].Normal.z = Temp;
		}
		if(TransformFlags & BSP_SWAPZX)
		{
			float Temp = Vertices[i].Position.z;
			Vertices[i].Position.z = Vertices[i].Position.x;
			Vertices[i].Position.x = Temp;

			Temp = Vertices[i].Normal.z;
			Vertices[i].Normal.z = Vertices[i].Normal.x;
			Vertices[i].Normal.x = Temp;
		}

		Vertex V;
		V.Position = Vertices[i].Position;
		V.Normal = Vertices[i].Normal;
		V.AddCoord(Vertices[i].TextureCoord);
		if(UseLightmaps)
			V.AddCoord(Vertices[i].LightmapCoord);
        memcpy(V.Color,Vertices[i].Color,sizeof(BYTE)*4);

		mVertices.push_back(V); 
	}
	delete Vertices;
	Vertices = NULL;

	fseek(fp,mLumps[kFaces].Offset,SEEK_SET);
	for(i = 0; i < NumFaces; i++) 
	{ 
		bspFace *Face = new bspFace;
		fread(Face,1,sizeof(bspFace),fp);
		if(TransformFlags & BSP_SWAPXY)
		{
			float Temp = Face->LMapPos.x;
			Face->LMapPos.x = Face->LMapPos.y;
			Face->LMapPos.y = Temp;

			Temp = Face->Normal.x;
			Face->Normal.x = Face->Normal.y;
			Face->Normal.y = Temp;
		}
		if(TransformFlags & BSP_SWAPYZ)
		{
			float Temp = Face->LMapPos.y;
			Face->LMapPos.y = Face->LMapPos.z;
			Face->LMapPos.z = Temp;

			Temp = Face->Normal.y;
			Face->Normal.y = Face->Normal.z;
			Face->Normal.z = Temp;
		}
		if(TransformFlags & BSP_SWAPZX)
		{
			float Temp = Face->LMapPos.z;
			Face->LMapPos.z = Face->LMapPos.x;
			Face->LMapPos.x = Temp;

			Temp = Face->Normal.z;
			Face->Normal.z = Face->Normal.x;
			Face->Normal.x = Temp;
		}
		mFaces.push_back(Face); 
	}

	fseek(fp,mLumps[kLeafs].Offset,SEEK_SET);
	
	for(i = 0; i < NumLeafs; i++)
	{
		bspLeaf *Leaf = new bspLeaf;
		fread(Leaf,1,sizeof(bspLeaf),fp);

		if(TransformFlags & BSP_SWAPXY)
		{
			int Temp = Leaf->Min.x;
			Leaf->Min.x = Leaf->Min.y;
			Leaf->Min.y = Temp;

			Temp = Leaf->Max.x;
			Leaf->Max.x = Leaf->Max.y;
			Leaf->Max.y = Temp;
		}
		if(TransformFlags & BSP_SWAPYZ)
		{
			int Temp = Leaf->Min.y;
			Leaf->Min.y = Leaf->Min.z;
			Leaf->Min.z = Temp;

			Temp = Leaf->Max.y;
			Leaf->Max.y = Leaf->Max.z;
			Leaf->Max.z = Temp;
		}
		if(TransformFlags & BSP_SWAPZX)
		{
			int Temp = Leaf->Min.z;
			Leaf->Min.z = Leaf->Min.x;
			Leaf->Min.x = Temp;

			Temp = Leaf->Max.z;
			Leaf->Max.z = Leaf->Max.x;
			Leaf->Max.x = Temp;
		}
		
		mLeafs.push_back(Leaf);
	}

	fseek(fp,mLumps[kLeafFaces].Offset,SEEK_SET);
	for(i = 0; i < NumLeafFaces; i++)
	{
		int LeafFace;
		fread(&LeafFace,1,sizeof(int),fp);
		mLeafFaces.push_back(LeafFace);
	}


	fseek(fp,mLumps[kNodes].Offset,SEEK_SET);
	for(i = 0; i < NumNodes; i++)
	{
		bspNode *Node = new bspNode;
		fread(Node,1,sizeof(bspNode),fp);
		if(TransformFlags & BSP_SWAPXY)
		{
			int Temp = Node->Min.x;
			Node->Min.x = Node->Min.y;
			Node->Min.y = Temp;

			Temp = Node->Max.x;
			Node->Max.x = Node->Max.y;
			Node->Max.y = Temp;
		}
		if(TransformFlags & BSP_SWAPYZ)
		{
			int Temp = Node->Min.y;
			Node->Min.y = Node->Min.z;
			Node->Min.z = Temp;

			Temp = Node->Max.y;
			Node->Max.y = Node->Max.z;
			Node->Max.z = Temp;
		}
		if(TransformFlags & BSP_SWAPZX)
		{
			int Temp = Node->Min.z;
			Node->Min.z = Node->Min.x;
			Node->Min.x = Temp;

			Temp = Node->Max.z;
			Node->Max.z = Node->Max.x;
			Node->Max.x = Temp;
		}

		mNodes.push_back(Node);
	}
	
	fseek(fp,mLumps[kPlanes].Offset,SEEK_SET);
	for(i = 0; i < NumPlanes; i++)
	{
		bspPlane *Plane = new bspPlane;
		fread(Plane,1,sizeof(bspPlane),fp);
		if(TransformFlags & BSP_SWAPXY)
		{
			float Temp = Plane->Normal.x;
			Plane->Normal.x = Plane->Normal.y;
			Plane->Normal.y = Temp;
		}
		if(TransformFlags & BSP_SWAPYZ)
		{
			float Temp = Plane->Normal.y;
			Plane->Normal.y = Plane->Normal.z;
			Plane->Normal.z = Temp;
		}
		if(TransformFlags & BSP_SWAPZX)
		{
			float Temp = Plane->Normal.z;
			Plane->Normal.z = Plane->Normal.x;
			Plane->Normal.x = Temp;
		}
		mPlanes.push_back(Plane);
	}

	gLog->Stream << "BSP " << mName << ": Loaded " << VertexCount() << " vertices, " << FaceCount() << " faces, " << NodeCount() << " nodes, " << PlaneCount() << " planes, " << LeafCount() << " leafs, " << LeafFaceCount() << " leaf faces.";
	gLog->Print();
	gLog->Stream << "BSP " << mName << ": Loading materials...";
	gLog->Print();

	mTextures.clear();
	fseek(fp,mLumps[kTextures].Offset,SEEK_SET);
	for(i = 0; i < NumMaterials; i++)
	{
		bspTexture T;
		fread(&T,1,sizeof(bspTexture),fp);

		Word GUID = gMaterials->Load(Device,T.Name);
		if(GUID == -1) {
			gLog->Stream << "Material " << T.Name << ": Could not load.";
			gLog->Print();
			continue;
		}
		/*for(int j = 0; j < FaceCount(); j++)
		{
			if(mFaces[j]->TextureID == i)
				mFaces[j]->TextureID = (int)GUID;
		}*/
		mTextures.push_back(GUID);
	}

	mFacesDrawn.Resize(FaceCount());

	if(UseLightmaps) {
		if(!LoadLightmaps(Device,Factor))
			return false;
	}
	if(!Rasterize(Device))
		return false;

	SetType("building");

	return true;
}

bool Quake3BSP::LoadLightmaps(LPDIRECT3DDEVICE9 Device, float Factor)
{
	if(mMeshPath == "") {
		gLog->Stream << "BSP: Could not load lightmaps.  Not allocated.";
		gLog->Print();
		return false;
	}
	int i;
	if(mUseLightmaps)
	{
		for(i = 0; i < LightmapCount(); i++)
		{
			gMaterials->Remove(mLightmaps[i]);
		}
	}
	mUseLightmaps = true;

	int NumLightmaps		= mLumps[kLightmaps].Length / sizeof(bspLightmap);

	FILE *fp = NULL;
	if((fp = fopen(mMeshPath.c_str(),"rb")) == NULL) {
		gLog->Stream << "BSP " << mName << ": Could not open file " << mMeshPath << ".";
		gLog->Print();
		return false;
	}
	RECT rect;
	mLightmaps.clear();
	fseek(fp,mLumps[kLightmaps].Offset,SEEK_SET);
	for(i = 0; i < NumLightmaps; i++)
	{
		bspLightmap L;
		LPDIRECT3DTEXTURE9 Lightmap;
		LPDIRECT3DSURFACE9 Surface;
		fread(&L,1,sizeof(bspLightmap),fp);
		ChangeGamma((byte *)&L,128*128*3,3);
		D3DPOOL Pool = gConfig->GetPool();
		D3DXCreateTexture(Device,128,128,0,0,D3DFMT_X8R8G8B8,Pool,&Lightmap);
		SetRect(&rect,0,0,128,128);
		Lightmap->GetSurfaceLevel(0,&Surface);
		D3DXLoadSurfaceFromMemory(Surface,NULL,NULL,L.ImageBits,D3DFMT_R8G8B8,128*3,NULL,&rect,D3DX_FILTER_LINEAR,0);
		Word GUID = gMaterials->Add(Lightmap);

		if(GUID == -1)
			continue;

		/*for(int j = 0; j < FaceCount(); j++)
		{
			if(mFaces[j]->LightmapID == i) {
					mFaces[j]->LightmapID = (int)GUID;
			}
		}*/
		mLightmaps.push_back(GUID);
	}

	gLog->Stream << "BSP " << mName << " : Lightmaps loaded.";
	gLog->Print();
	return true;
}

bool Quake3BSP::CreateIndexBuffer(LPDIRECT3DDEVICE9 Device)
{
	int i;
	int current = 0;
	int at; 
	int ZeroNormal = 0;
	for(i = 0; i < FaceCount(); i++)
	{
		at = 2;
		mFaces[i]->TextureID = (int)mTextures[mFaces[i]->TextureID];
		mFaces[i]->LightmapID = (int)mLightmaps[mFaces[i]->LightmapID];
		while(at < mFaces[i]->NumOfVerts)
		{
			IndexedFace *F = new IndexedFace;
			F->AddCoordID(mFaces[i]->TextureID);
			F->AddCoordID(mFaces[i]->LightmapID);

			F->Indices[0] = mFaces[i]->StartVertIndex;
			F->Indices[1] = mFaces[i]->StartVertIndex+at-1;
			F->Indices[2] = mFaces[i]->StartVertIndex+at;

			if(F->CalculateNormal(mVertices[F->Indices[0]].Position,
								mVertices[F->Indices[1]].Position,
								mVertices[F->Indices[2]].Position))
			{
				mBatch.Add(F);

			}
			else
			{
				delete F;
				ZeroNormal++;
			}
			at++;
		}
	}
	gLog->Stream << "BSP " << mName << " Removed: " << ZeroNormal << " triangles with zero normals.";
	gLog->Print();

	if(!mBatch.Rasterize(Device))
		return false;

	return true;
}

bool Quake3BSP::Rasterize(LPDIRECT3DDEVICE9 Device)
{
/*	if(mMesh)
		mMesh->Release();

	D3DVERTEXELEMENT9 *VertexElement = new D3DVERTEXELEMENT9[MAX_FVF_DECL_SIZE];

	D3DXDeclaratorFromFVF(D3DFVF_XYZ | 
						//D3DFVF_NORMAL | 
						D3DFVF_DIFFUSE | 
						D3DFVF_TEX2,
						VertexElement);


	if(D3DXCreateMesh(FaceCount(),VertexCount(),D3DXMESH_MANAGED,VertexElement,Device,&mMesh) != D3D_OK)
		return false;*/

	if(!CreateIndexBuffer(Device))
		return false;

	if(!CreateVertexBuffer(Device))
		return false;

    return true;
}

void Quake3BSP::DestroyBSP()
{
	if(VertexCount() == 0)
		return;

	gLog->Stream << "BSP " << mName << ": Deallocating...";
	gLog->Print();
	int i;

	for(i = 0; i < FaceCount(); i++)
		delete mFaces[i];
	mFaces.clear();

	for(i = 0; i < NodeCount(); i++)
		delete mNodes[i];
	mNodes.clear();

	for(i = 0; i < LeafCount(); i++)
		delete mLeafs[i];
	mLeafs.clear();

	mLeafFaces.clear();

	for(i = 0; i < PlaneCount(); i++)
		delete mPlanes[i];
	mPlanes.clear();

	mTextures.clear();

	for(int i = 0; i < (int)mLightmaps.size(); i++)
		gMaterials->Remove(mLightmaps[i]);
	mLightmaps.clear();

	DestroyMesh();
}

bool Quake3BSP::Render(LPDIRECT3DDEVICE9 Device)
{
	if(!Device) 
	{
		gLog->Stream << "BSP " << mName << " Render: Invalid device.";
		gLog->Print();
		return false;
	}

	if(!mVertexBuffer)
	{
		gLog->Stream << "BSP " << mName << " Render: Invalid vertex buffer.";
		gLog->Print();
		return false;	
	}

	Device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	Device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
	if(mUseLightmaps)
	{
		Device->SetTextureStageState(1,D3DTSS_COLORARG1,D3DTA_TEXTURE);
		Device->SetTextureStageState(1,D3DTSS_COLORARG2,D3DTA_CURRENT);
		Device->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_MODULATE);
	}
	else
	{
		Device->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);
	}

	gMaterials->Reset();

	mBatch.Render(Device,VertexBuffer(),FVF(),VertexSize());

	return true;
}