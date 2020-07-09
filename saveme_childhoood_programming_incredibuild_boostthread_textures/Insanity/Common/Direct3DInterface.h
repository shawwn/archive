#ifndef DIRECT3DINTERFACE_H
#define DIRECT3DINTERFACE_H

#define D3D_DEBUG_INFO

#include <string>
#include <windows.h>
#include <d3dx9.h>
#include "SmartPointer.h"
#include "Interface.h"
#include "Macros.h"

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
/*
class Window : public Resource
{
private:
	int mXOffset;
	int mYOffset;
	UINT mWidth;
	UINT mHeight;
	HWND mhWnd;
	std::string mName;
	void Create();
public:
	Window(UINT Width, UINT Height, const std::string &Name = "Default Window", int XOffset = 0, int YOffset = 0) : 
	  Resource("Window",OBJECT_WINDOW), mWidth(Width), mHeight(Height), mName(Name), mXOffset(XOffset), mYOffset(YOffset), mhWnd(NULL)
	  { Create(); }

	  LoggingInterface &PrintProperties();

	inline HWND GethWnd() const { return mhWnd; }
	inline const std::string &GetName() const { return mName; }
	inline UINT GetWidth() const { return mWidth; }
	inline UINT GetHeight() const { return mHeight; }
	inline UINT GetXOffset() const { return mXOffset; }
	inline UINT GetYOffset() const { return mYOffset; }
};

class Direct3DInterface : public Interface
{
private:
	LPDIRECT3D9 mD3D;
	LPDIRECT3DDEVICE9 mDevice;

	bool Reset();
public:
	Direct3DInterface();

	bool Initialize(HWND hWnd);

};*/

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

	void SetStates();
public:
	Direct3DInterface() : mNumAdapters(0), mAdapter(0) { }
	~Direct3DInterface() { Deinitialize(); }
	bool Initialize(UINT Adapter = D3DADAPTER_DEFAULT, D3DDEVTYPE DeviceType = D3DDEVTYPE_HAL);
	void Deinitialize();
	bool Reset();
	bool Recover();

	bool SaveScreenshot(const std::string &Filename);

	inline HWND GethWnd() { return ghWnd; }
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


