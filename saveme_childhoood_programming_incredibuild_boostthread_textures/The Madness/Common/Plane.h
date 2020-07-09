#ifndef PLANE_H
#define PLANE_H

#include <d3dx9.h>
#include "Macros.h"


class Plane
{
private:
	UINT mVBIndex;
	D3DXVECTOR3 mPositions[4];
	UINT mSize;
	float mWidth;
	float mDepth;
	float mGridSize;
	UINT mEffectIndex;
	UINT mTextureIndex;
	UINT mDeclIndex;
public:
	Plane() : mVBIndex(0), mDeclIndex(0), mSize(0), mWidth(0), mDepth(0), mGridSize(0) { }
	~Plane();

	bool Initialize(LPDIRECT3DDEVICE9 Device, float Width = 10000.0f, float Depth = 10000.0f);
	bool Render(LPDIRECT3DDEVICE9 Device);	
};

#endif