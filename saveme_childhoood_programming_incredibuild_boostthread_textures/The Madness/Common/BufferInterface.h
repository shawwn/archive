#ifndef VERTEXBUFFERINTERFACE_H
#define VERTEXBUFFERINTERFACE_H

#include <vector>
#include <d3dx9.h>
#include "Macros.h"

struct VBDATA
{
	VBDATA() : Buffer(NULL), Size(0), Pool(D3DPOOL_MANAGED), Usage(0), Stride(0) { }
	~VBDATA() { 
		SAFE_RELEASE(Buffer);
	}
	LPDIRECT3DVERTEXBUFFER9 Buffer;
	UINT Size;
	D3DPOOL Pool;
	DWORD Usage;
	UINT Stride;
};

struct IBDATA
{
	IBDATA() : Buffer(NULL), Size(0), Pool(D3DPOOL_MANAGED), Usage(0), Format(D3DFMT_INDEX16) { }
	~IBDATA() { 
		SAFE_RELEASE(Buffer);
	}
	LPDIRECT3DINDEXBUFFER9 Buffer;
	UINT Size;
	D3DPOOL Pool;
	DWORD Usage;
	D3DFORMAT Format;
};

typedef VBDATA* LPVBDATA;
typedef IBDATA* LPIBDATA;

class BufferInterface
{
private:
	LPDIRECT3DDEVICE9 mUseDevice;
	std::vector<LPVBDATA> mVertexBuffers;
	std::vector<LPIBDATA> mIndexBuffers;

	inline bool ValidVB(UINT Index) { return (Index >= 0 && Index < NumVertexBuffers() && mVertexBuffers[Index]); }
	inline bool ValidIB(UINT Index) { return (Index >= 0 && Index < NumIndexBuffers() && mIndexBuffers[Index]); }
public:
	BufferInterface() : mUseDevice(NULL){ }
	~BufferInterface() {
		for(UINT i = 0; i < NumVertexBuffers(); i++) 
			SAFE_DELETE(mVertexBuffers[i]); }
	
	void OnLostDevice();
	void OnResetDevice(LPDIRECT3DDEVICE9 Device);
	void OnDestroyDevice();

	UINT CreateVertexBuffer(UINT SizeInBytes, UINT Stride, D3DPOOL Pool = D3DPOOL_MANAGED, DWORD Usage = 0);
	LPDIRECT3DVERTEXBUFFER9 AccessVertexBuffer(UINT Index);
	bool ReleaseVertexBuffer(UINT Index);
	bool SetStream(UINT Index, UINT Stream = 0, UINT Offset = 0);

	UINT CreateIndexBuffer(UINT SizeInBytes, D3DFORMAT Format, D3DPOOL Pool = D3DPOOL_MANAGED, DWORD Usage = 0);
	LPDIRECT3DINDEXBUFFER9 AccessIndexBuffer(UINT Index);
	bool ReleaseIndexBuffer(UINT Index);
	bool SetIndices(UINT Index);

	inline UINT NumVertexBuffers() { return (UINT)mVertexBuffers.size(); }
	inline UINT NumIndexBuffers() { return (UINT)mIndexBuffers.size(); }
	
};

extern BufferInterface gBuffers;

#endif