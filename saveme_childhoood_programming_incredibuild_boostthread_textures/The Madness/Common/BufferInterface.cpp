#include "BufferInterface.h"
#include "LoggingInterface.h"
#include "Direct3DInterface.h"
using namespace std;

BufferInterface gBuffers;

void BufferInterface::OnLostDevice()
{
	UINT i;
	for(i = 0; i < NumVertexBuffers(); i++) {
		LPVBDATA Data = mVertexBuffers[i];
		if(Data->Usage & D3DUSAGE_DYNAMIC || Data->Pool == D3DPOOL_DEFAULT) {
			SAFE_RELEASE(Data->Buffer);
		}
	}
	for(i = 0; i < NumIndexBuffers(); i++) {
		LPIBDATA Data = mIndexBuffers[i];
		if(Data->Usage & D3DUSAGE_DYNAMIC || Data->Pool == D3DPOOL_DEFAULT) {
			SAFE_RELEASE(Data->Buffer);
		}
	}
}

void BufferInterface::OnResetDevice(LPDIRECT3DDEVICE9 Device)
{
	mUseDevice = Device;
	HRESULT hr;
	UINT i;
	for(i = 0; i < NumVertexBuffers(); i++) {
		LPVBDATA Data = mVertexBuffers[i];
		if(!Data->Buffer)
		{
			if(FAILED(hr = Device->CreateVertexBuffer(Data->Size,Data->Usage,0,Data->Pool,&Data->Buffer,NULL))) {
				LogDxErr(hr,"BufferInterface::OnResetDevice(CreateVertexBuffer)");
			}
		}
	}
	for(i = 0; i < NumIndexBuffers(); i++) {
		LPIBDATA Data = mIndexBuffers[i];
		if(!Data->Buffer)
		{
			if(FAILED(hr = Device->CreateIndexBuffer(Data->Size,Data->Usage,Data->Format,Data->Pool,&Data->Buffer,NULL))) {
				LogDxErr(hr,"BufferInterface::OnResetDevice(CreateIndexBuffer)");
			}
		}
	}
}

void BufferInterface::OnDestroyDevice()
{
	UINT i;
	for(i = 0; i < NumVertexBuffers(); i++) {
		LPVBDATA Data = mVertexBuffers[i];
		SAFE_RELEASE(Data->Buffer);
	}	
	for(i = 0; i < NumIndexBuffers(); i++) {
		LPIBDATA Data = mIndexBuffers[i];
		SAFE_RELEASE(Data->Buffer);
	}	
}

UINT BufferInterface::CreateVertexBuffer(UINT SizeInBytes, UINT Stride, D3DPOOL Pool, DWORD Usage)
{
	if(!mUseDevice)
		return -1;

	LPVBDATA Data = new VBDATA;
	HRESULT hr;

	if(FAILED(hr = mUseDevice->CreateVertexBuffer(SizeInBytes,Usage,0,Pool,&Data->Buffer,NULL))) {
		LogDxErr(hr,"BufferInterface::CreateVertexBuffer(CreateBuffer)");
		SAFE_DELETE(Data);
		return -1;
	}

	Data->Stride = Stride;

	Data->Pool = Pool;
	Data->Usage = Usage;
	Data->Size = SizeInBytes;

	for(UINT i = 0; i < NumVertexBuffers(); i++) {
		if(!mVertexBuffers[i]) {
			mVertexBuffers[i] = Data;
			return i;
		}
	}

	mVertexBuffers.push_back(Data);
	return NumVertexBuffers()-1;
}

LPDIRECT3DVERTEXBUFFER9 BufferInterface::AccessVertexBuffer(UINT Index)
{
	if(!ValidVB(Index))
		return NULL;
	return mVertexBuffers[Index]->Buffer;
}

bool BufferInterface::ReleaseVertexBuffer(UINT Index)
{
	if(!ValidVB(Index))
		return false;

	SAFE_DELETE(mVertexBuffers[Index]);
	return true;
}

bool BufferInterface::SetStream(UINT Index, UINT Stream, UINT Offset)
{
	if(!mUseDevice || !ValidVB(Index))
		return false;
	if(!mVertexBuffers[Index]->Buffer)
		return false;

	HRESULT hr;
	if(FAILED(hr = mUseDevice->SetStreamSource(Stream,mVertexBuffers[Index]->Buffer,Offset,mVertexBuffers[Index]->Stride)))
	{
		LogDxErr(hr,"BufferInterface::SetStream(SetStreamSource)");
		return false;
	}
	return true;
}

UINT BufferInterface::CreateIndexBuffer(UINT SizeInBytes, D3DFORMAT Format, D3DPOOL Pool, DWORD Usage)
{
	if(!mUseDevice)
		return -1;

	if(Format != D3DFMT_INDEX16 && Format != D3DFMT_INDEX32)
		Format = D3DFMT_INDEX16;

	LPIBDATA Data = new IBDATA;
	HRESULT hr;
	if(FAILED(hr = mUseDevice->CreateIndexBuffer(SizeInBytes,Usage,Format,Pool,&Data->Buffer,NULL))) {
		LogDxErr(hr,"BufferInterface::CreateIndexBuffer");
		SAFE_DELETE(Data);
		return -1;
	}

	Data->Format = Format;
	Data->Size = SizeInBytes;
	Data->Pool = Pool;
	Data->Usage = Usage;

	for(UINT i = 0; i < NumIndexBuffers(); i++) {
		if(!mIndexBuffers[i]) {
			mIndexBuffers[i] = Data;
			return i;
		}
	}

	mIndexBuffers.push_back(Data);
	return NumIndexBuffers()-1;
}

LPDIRECT3DINDEXBUFFER9 BufferInterface::AccessIndexBuffer(UINT Index)
{
	if(!ValidIB(Index))
		return NULL;
	return mIndexBuffers[Index]->Buffer;
}

bool BufferInterface::ReleaseIndexBuffer(UINT Index)
{
	if(!ValidIB(Index))
		return false;

	SAFE_DELETE(mIndexBuffers[Index]);
	return true;
}

bool BufferInterface::SetIndices(UINT Index)
{
	if(!mUseDevice || !ValidIB(Index))
		return false;

	HRESULT hr;
	if(FAILED(hr = mUseDevice->SetIndices(mIndexBuffers[Index]->Buffer))) {
		LogDxErr(hr,"BufferInterface::SetIndices");
		return false;
	}

	return true;
}