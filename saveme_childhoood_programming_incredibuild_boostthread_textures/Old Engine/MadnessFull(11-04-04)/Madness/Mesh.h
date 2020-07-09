#ifndef _MESH_H
#define _MESH_H

#include "Object.h"
#include "TriangleBatch.h"
#include <d3dx9.h>

class Mesh : public Object
{
protected:
	string mMeshPath;

	DWORD mFVF;
	int mVertexSize;

	int mMinIndex;
	int mStartIndex;

	std::vector<Vertex> mVertices;

	TriangleBatch mBatch;
	LPDIRECT3DVERTEXBUFFER9 mVertexBuffer;

	void DestroyMesh();
public:
	Mesh();
	virtual ~Mesh();

	virtual bool Render(LPDIRECT3DDEVICE9 Device);
	virtual bool CreateVertexBuffer(LPDIRECT3DDEVICE9 Device);

	int VertexCount()	{ return (int)mVertices.size(); }

	LPDIRECT3DVERTEXBUFFER9 VertexBuffer()	{ return mVertexBuffer; }
	int VertexSize()						{ return mVertexSize; }
	DWORD FVF()								{ return mFVF; }
};

#endif