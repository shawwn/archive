#ifndef BILLBOARDINTERFACE_H
#define BILLBOARDINTERFACE_H

#include "NxPhysics.h"
#include <d3dx9.h>
#include <vector>
#include "Macros.h"
#include "Types.h"


class BillboardVertex
{
public:
	BillboardVertex() { }
	BillboardVertex(const NxVec3 &p, VTEXCOORD2 &t = VTEXCOORD2()) : mPosition(p), mCoord(t) { }
	NxVec3 mPosition;
	VTEXCOORD2 mCoord;
};

class Billboard
{
public:
	BillboardVertex mQuad[6];

	Billboard() { }

	void Make(const NxVec3 &Position, const NxVec3 &ViewUp, const NxVec3 &ViewRight, short Width = 10, short Height = 10) { 
		mQuad[0].mPosition = Position - ViewRight*Width;
		mQuad[1].mPosition = Position + ViewUp*Height;
		mQuad[2].mPosition = Position - ViewUp*Height;

		mQuad[3].mPosition = Position + ViewUp*Height;
		mQuad[4].mPosition = Position - ViewUp*Height;
		mQuad[5].mPosition = Position + ViewRight*Width;

		mQuad[0].mCoord.U = 0; mQuad[0].mCoord.V = 1;
		mQuad[1].mCoord.U = 1; mQuad[1].mCoord.V = 1;
		mQuad[2].mCoord.U = 0; mQuad[2].mCoord.V = 0;

		mQuad[3].mCoord.U = 1; mQuad[3].mCoord.V = 1;
		mQuad[4].mCoord.U = 0; mQuad[4].mCoord.V = 0;
		mQuad[5].mCoord.U = 1; mQuad[5].mCoord.V = 0;
	}
};


class BillboardInterface
{
private:
	std::vector<LPDIRECT3DVERTEXBUFFER9> mVB;
	UINT mBillboardsPerBuffer;
	UINT mBillboardCount;

	LPDIRECT3DVERTEXBUFFER9 AddVB();
public:
	BillboardInterface() : mBillboardsPerBuffer(1024), mBillboardCount(0) { }
	~BillboardInterface() { for(size_t i = 0; i < mVB.size(); i++) { SAFE_RELEASE(mVB[i]); } }

    bool Make(LPDIRECT3DDEVICE9 Device, const char *VertexPositionData, UINT Count, UINT Stride, const NxVec3 &ViewUp, const NxVec3 &ViewRight, short Width = 10, short Height = 10);
	bool Render(LPDIRECT3DDEVICE9 Device);
	void Destroy();

	inline UINT BufferCount() { return (UINT)mVB.size(); }
};

#endif