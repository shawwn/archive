#ifndef SKYDOME_H
#define SKYDOME_H

#include <d3dx9.h>
#include "Macros.h"
#include "BufferInterface.h"
#include "VertexDeclarationInterface.h"

class SkydomeVertex
{
public:
	SkydomeVertex() : x(0), y(0), z(0), u(0), v(0) { }
	float x, y, z, u, v;
};

class Skydome
{
private:
	UINT mVertexBufferIndex;
	UINT mVertexDeclIndex;
	UINT mNumVertices;
	UINT mEffectIndex;
	UINT mTextureIndex;
public:
	Skydome() : mVertexBufferIndex(-1), mVertexDeclIndex(-1), mNumVertices(0), mEffectIndex(-1), mTextureIndex(-1) { }
	~Skydome() { gBuffers.ReleaseVertexBuffer(mVertexBufferIndex); gDeclarations.ReleaseDeclaration(mVertexDeclIndex);  }
	bool GenerateDome(float radius, float dtheta, float dphi, float hTile, float vTile);
	bool Render(LPDIRECT3DDEVICE9 Device);
};

#endif