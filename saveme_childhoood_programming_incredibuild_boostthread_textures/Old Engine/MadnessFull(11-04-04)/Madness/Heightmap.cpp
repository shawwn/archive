#include "dxstdafx.h"
#include "Heightmap.h"
#include "ConfigurationInterface.h"
#include "LoggingInterface.h"



bool Heightmap::Load(LPDIRECT3DDEVICE9 Device, const string &Filename, const float &Scale, const float &Height)
{
	if(!Device) {
		gLog->Stream << "Heightmap " << Filename << ": Load error, invalid device.";
		gLog->Print();
		return false;
	}
	if(Scale < 1)
	{
		gLog->Stream << "Heightmap " << Filename << ": Load error, scale value must be greater than or equal to 1.";
		gLog->Print();
		return false;
	}

	DestroyMesh();

	if(!mData.Load(Device,Filename))
	{
		gLog->Stream << "Heightmap " << Filename << ": Couldn't load file.";
		gLog->Print();
		return false;
	}
	mName = Filename;
	mName = mName.substr((mName.find_last_of('/')!=std::string::npos)?mName.find_last_of('/')+1:0,mName.length());
	mName = mName.substr(0,(mName.find_last_of('.')!=std::string::npos)?mName.find_last_of('.'):mName.length());

	//mFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;

	int i, j;

	// Create vertices
    for(i = 0; i < mData.Height(); i++)
	{
		for(j = 0; j < mData.Width(); j++)
		{
			Vertex V;
			V.Position = D3DXVECTOR3(j*Scale,Height*(mData.mData[i][j][0]/255.0f),(mData.Height()-i)*Scale);
			V.Color[0] = mData.mData[i][j][0];
			V.Color[1] = mData.mData[i][j][0];
			V.Color[2] = mData.mData[i][j][0];
			V.Color[3] = mData.mData[i][j][0];
			mVertices.push_back(V);
		}
	}


	// Calculate normals


	// Create Triangles

	for(i = 0; i < VertexCount() / mData.Height()-1; i++)
	{
		for(j = 0; j < VertexCount() / mData.Width()-1; j++)
		{
			IndexedFace *F = new IndexedFace;
			F->Indices[0] = i*VertexCount()/mData.Width()+j;
			F->Indices[1] = i*VertexCount()/mData.Width()+j+1;
			F->Indices[2] = i*VertexCount()/mData.Width()+j+VertexCount()/mData.Width();

			mBatch.Add(F);

			F = new IndexedFace;
			F->Indices[0] = i*VertexCount()/mData.Width()+j+1;
			F->Indices[1] = i*VertexCount()/mData.Width()+j+VertexCount() / mData.Width()+1;
			F->Indices[2] = i*VertexCount()/mData.Width()+j+VertexCount() / mData.Width();

			mBatch.Add(F);
		}
	}

	if(!CreateVertexBuffer(Device))
		return false;

	mBatch.Rasterize(Device);

	SetType("landscape");

	gLog->Stream << "Heightmap " << mName << ": Loaded " << VertexCount() << " vertices and " << mBatch.TriangleCount() << " faces.";
	gLog->Print();

	return true;
}