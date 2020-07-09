#ifndef _BATCHINGINTERFACE_H
#define _BATCHINGINTERFACE_H

#include "Vertex.h"
using namespace std;

class TriangleBatch
{
private:

	std::vector< int > mMin;
	std::vector< int > mMax;
	std::vector< std::vector<IndexedFace *> > mGroup;
	std::vector< LPDIRECT3DINDEXBUFFER9 > mBuffer;
public:
	TriangleBatch() { Destroy(); }
	~TriangleBatch() { DestroyTriangles(); ReleaseBuffers(); }

	void Add(IndexedFace *F);
	bool Rasterize(LPDIRECT3DDEVICE9 Device);
	bool Render(LPDIRECT3DDEVICE9 Device, LPDIRECT3DVERTEXBUFFER9 VertexBuffer, DWORD FVF, int VertexSize);
	void Destroy() { DestroyTriangles(); ReleaseBuffers(); }
	void DestroyTriangles();
	void ReleaseBuffers();

	int GroupCount() { return (int)mGroup.size(); }
	int TriangleCount() { int count = 0; for(int i = 0; i < GroupCount(); i++) { count += (int)mGroup[i].size(); } return count; }
	int BufferCount() { return (int)mBuffer.size(); }
};

#endif