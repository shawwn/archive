#ifndef DIRECT3DINTERFACE_H
#define DIRECT3DINTERFACE_H

#define D3D_DEBUG_INFO

#define NOMINMAX

#include <d3dx9.h>
#include "Window.h"
#include "Vertex.h"
#include "Macros.h"
#include "LoggingInterface.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

class Direct3DInterface
{
private:
	LPDIRECT3D9 mDirect3D;
	LPDIRECT3DDEVICE9 mDevice;

	D3DDEVTYPE mDeviceType;

	UINT mNumAdapters;
	UINT mAdapter;
	D3DADAPTER_IDENTIFIER9 *mAdapterIdentifier;
	UINT *mNumAdapterModes;
	D3DFORMAT *mMaxSupportedDisplayFormat;
	D3DFORMAT *mMaxSupportedDepthBufferFormat;
	D3DFORMAT *mMaxSupportedTextureFormat;
	D3DDISPLAYMODE **mAdapterModes;
	D3DCAPS9 *mDeviceCaps;

	Window mWindow;
    void SetStates();
public:
	Direct3DInterface() : mNumAdapters(0), mAdapter(0) { }
	~Direct3DInterface() { Deinitialize(); }
	bool Initialize(UINT Adapter = D3DADAPTER_DEFAULT, D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL);
	void Deinitialize();
	bool Reset();
	bool Recover();

	bool FillVertexBuffer(LPDIRECT3DVERTEXBUFFER9 *VertexBuffer, Vertex *StreamData, int NumVertices);
	bool SaveScreenshot(const std::string &Filename);

	inline HWND GethWnd() { return mWindow.GethWnd(); }
	inline LPDIRECT3DDEVICE9 GetDevice() { return mDevice; }

	inline D3DFORMAT GetMaxSupportedDisplayFormat() { return mMaxSupportedDisplayFormat[mAdapter]; }
	inline D3DFORMAT GetMaxSupportedDepthBufferFormat() { return mMaxSupportedDepthBufferFormat[mAdapter]; }
	inline D3DFORMAT GetMaxSupportedTextureFormat() { return mMaxSupportedTextureFormat[mAdapter]; }
	inline const D3DCAPS9 &GetCaps() { return mDeviceCaps[mAdapter]; }

	bool SupportsTextureFormat(D3DFORMAT Format, DWORD Usage, D3DRESOURCETYPE Type);


};

extern Direct3DInterface gD3D;
extern D3DXMATRIX gMatIdent;

extern std::string DxErrStr(HRESULT hr);
extern void LogDxErr(HRESULT hr, const std::string &str = "");

#endif