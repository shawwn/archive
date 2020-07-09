#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#define NOMINMAX
#include <string>
#include <d3dx9.h>

class Heightmap
{
private:
	int mWidth;
	int mHeight;
	std::string mFilename;
public:
	Heightmap::Heightmap() : mWidth(0), mHeight(0) { }
	BYTE *Load(LPDIRECT3DDEVICE9 Device, const std::string &Filename, int &Width, int &Height);
	bool BuildMesh(const std::string &Filename, LPDIRECT3DDEVICE9 Device, UINT &VBIndex, int VertexBufferOffset, UINT &IBIndex, int IndexBufferOffset, UINT &DeclIndex);
	void Deinitialize();

	inline int GetWidth() { return mWidth; }
	inline int GetHeight() { return mHeight; }
	inline const std::string &GetName() { return mFilename; }
};

#endif