
#include "Direct3DInterface.h"
#include "LoggingInterface.h"
#include "ClientSettingsInterface.h"
#include "EnumStringInterface.h"
#include "Macros.h"
#include "GameInterface.h"
#include <dxerr9.h>

#pragma comment(lib,"dxerr9.lib")

std::string DxErrStr(HRESULT hr)
{
	return DXGetErrorString9(hr);
}

void LogDxErr(HRESULT hr, const std::string &str)
{
	if(str == "") 
		gLog << "DirectX ERROR: Function returned error [" << DxErrStr(hr) << "]\n";
	else
		gLog << str << " ERROR: Function returned error [" << DxErrStr(hr) << "]\n";
}

Direct3DInterface gD3D;
D3DXMATRIX gMatIdent;

void Direct3DInterface::SetStates()
{
	if(!mDevice)
		return;
	
	mDevice->SetRenderState(D3DRS_LIGHTING,false);
	mDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
}

bool Direct3DInterface::Initialize(UINT Adapter, D3DDEVTYPE DeviceType)
{
	D3DXMatrixIdentity(&gMatIdent);
	mDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(!mDirect3D)
	{
		gLog ^ "ERROR: Could not initialize DirectX.  Please make sure you have DirectX 9.0c installed.\n";
		return false;
	}

	mNumAdapters = mDirect3D->GetAdapterCount();
	if(!mNumAdapters) {
		gLog ^ "ERROR: Number of display adapters is zero.\n";
		return false;
	}

	mAdapterIdentifier = new D3DADAPTER_IDENTIFIER9 [mNumAdapters];
	mNumAdapterModes = new UINT [mNumAdapters];
	mMaxSupportedDisplayFormat = new D3DFORMAT [mNumAdapters];
	mMaxSupportedDepthBufferFormat = new D3DFORMAT [mNumAdapters];
	mMaxSupportedTextureFormat = new D3DFORMAT [mNumAdapters];
	mAdapterModes = new D3DDISPLAYMODE *;
	mDeviceCaps = new D3DCAPS9 [mNumAdapters];

	mAdapter = (Adapter > mNumAdapters-1 ? mNumAdapters-1 : Adapter);

	for(UINT i = 0; i < mNumAdapters; i++)
	{
		// Get adapter information
		if(mDirect3D->GetAdapterIdentifier(i,0,&mAdapterIdentifier[i]) != D3D_OK) {
			gLog << "WARNING: Could not get identifier information on adapter [" << i << "]\n";
		}

		// Get maximum supported display format bit information
		mNumAdapterModes[i] = mDirect3D->GetAdapterModeCount(i,D3DFMT_X8R8G8B8);
		if(mNumAdapterModes[i]) {
			mMaxSupportedDisplayFormat[i] = D3DFMT_X8R8G8B8;
		}
		else
		{
			mMaxSupportedDisplayFormat[i] = D3DFMT_R5G6B5;
			mNumAdapterModes[i] = mDirect3D->GetAdapterModeCount(i,D3DFMT_R5G6B5);
			if(!mNumAdapterModes[i])
				gLog ^ "ERROR: No adapter modes supported for 32-bit or 16-bit! (Something's farked up)\n";
		}

        // Check DevType
		if(DeviceType == D3DDEVTYPE_HAL)
		{
			if(mDirect3D->CheckDeviceType(i,DeviceType,mMaxSupportedDisplayFormat[i],mMaxSupportedDisplayFormat[i],gSettings.Windowed) == D3DERR_NOTAVAILABLE) {
				gLog << "WARNING: Hardware acceleration not supported, using software rendering.\n";
				mDeviceType = D3DDEVTYPE_SW;
			}
			else
				mDeviceType = D3DDEVTYPE_HAL;
		}
		else
			mDeviceType = DeviceType;

		
		// Get maximum supported depth buffer bit information
		if(mDirect3D->CheckDepthStencilMatch(i,mDeviceType,mMaxSupportedDisplayFormat[i],mMaxSupportedDisplayFormat[i],D3DFMT_D24S8) == D3DERR_NOTAVAILABLE) {
			gLog << "WARNING: 24-bit depth buffering not supported, using 16-bit depth buffering.\n";
			mMaxSupportedDepthBufferFormat[i] = D3DFMT_D16;
			if(mDirect3D->CheckDepthStencilMatch(i,mDeviceType,mMaxSupportedDisplayFormat[i],mMaxSupportedDisplayFormat[i],D3DFMT_D16) == D3DERR_NOTAVAILABLE) {
				gLog << "WARNING: 16-bit depth buffering is not supported.\n";
			}
		}
		else
			mMaxSupportedDepthBufferFormat[i] = D3DFMT_D24S8;

		// Get maximum supported texture format bit information
		if(mDirect3D->CheckDeviceFormat(i,mDeviceType,mMaxSupportedDisplayFormat[i],0,D3DRTYPE_TEXTURE,D3DFMT_A8R8G8B8) == D3DERR_NOTAVAILABLE) {
			if(mDirect3D->CheckDeviceFormat(i,mDeviceType,mMaxSupportedDisplayFormat[i],0,D3DRTYPE_TEXTURE,D3DFMT_R8G8B8) == D3DERR_NOTAVAILABLE) {
				mMaxSupportedTextureFormat[i] = D3DFMT_R5G6B5;
				if(mDirect3D->CheckDeviceFormat(i,mDeviceType,mMaxSupportedDisplayFormat[i],0,D3DRTYPE_TEXTURE,D3DFMT_R5G6B5) == D3DERR_NOTAVAILABLE) {
					gLog << "WARNING: 16-bit textures are not supported.\n";
				}
			}
			else
				mMaxSupportedTextureFormat[i] = D3DFMT_R8G8B8;
		}
		else
			mMaxSupportedTextureFormat[i] = D3DFMT_A8R8G8B8;

		// Get supported display modes
		mAdapterModes[i] = new D3DDISPLAYMODE [mNumAdapterModes[i]];
		for(UINT j = 0; j < mNumAdapterModes[i]; j++) {
			mDirect3D->EnumAdapterModes(i,mMaxSupportedDisplayFormat[i],j,&mAdapterModes[i][j]);
		}

		// Get adapter capabilities
		mDirect3D->GetDeviceCaps(i,mDeviceType,&mDeviceCaps[i]);
        
		gLog << "#################################\n";
		gLog << "Adapter [" << i << "] information:\n";
		gLog << "Name:				[" << mAdapterIdentifier[i].Description << "]\n";
		gLog << "Driver:				[" << mAdapterIdentifier[i].Driver << "]\n";
		gLog << "Driver Version:			[" << (unsigned __int64)mAdapterIdentifier[i].DriverVersion.QuadPart << "]\n";
		gLog << "Maximum Resolution:		[" << mAdapterModes[i][mNumAdapterModes[i]-1].Width << "x" << mAdapterModes[i][mNumAdapterModes[i]-1].Height << "]\n";
		gLog << "Maximum Display Format:		[" << EnumToString.Find(mMaxSupportedDisplayFormat[i],"D3DFMT") << "]\n";
		gLog << "Maximum Depth Buffer Format:	[" << EnumToString.Find(mMaxSupportedDepthBufferFormat[i],"D3DFMT") << "]\n";
		gLog << "Maximum Texture Format:		[" << EnumToString.Find(mMaxSupportedTextureFormat[i],"D3DFMT") << "]\n";
		gLog << "Using Device Type:		[" << EnumToString.Find(mDeviceType,"D3DDEVTYPE") << "]\n";
	}
	gLog << "#################################\n"; 

	gLog << "Using adapter [" << mAdapter << "]\n";
	switch(gSettings.DisplayBits) {
		case 0:
			break;
		case 16:
			mMaxSupportedDisplayFormat[mAdapter] = D3DFMT_R5G6B5;
			mMaxSupportedTextureFormat[mAdapter] = D3DFMT_R5G6B5;
			gLog << "Adapter's display has been forced to 16-bit (R5G6B5) due to the settings in Settings.dat\n";
			break;
		case 32:
			mMaxSupportedDisplayFormat[mAdapter] = D3DFMT_X8R8G8B8;
			gLog << "Adapter's display format has been forced to 32-bit (X8R8G8B8) due to the settings in Settings.dat\n";
			break;
	}

	switch(gSettings.DepthBits) {
		case 0:
			break;
		case 16:
			mMaxSupportedDepthBufferFormat[mAdapter] = D3DFMT_D16;
			gLog << "Adapter's depth buffer format has been forced to 16-bit (D16) due to the settings in Settings.dat\n";
			break;
		case 32:
			mMaxSupportedDepthBufferFormat[mAdapter] = D3DFMT_D24S8;
			gLog << "Adapter's depth buffer format has been force to 24-bit (D24S8) due to the settings in Settings.dat\n";
			break;
	}
	if(gSettings.VertexProcessing != D3DCREATE_HARDWARE_VERTEXPROCESSING) {
		gLog << "Adapter is using [" << EnumToString.Find(gSettings.VertexProcessing,"D3DCREATE") << "] vertex processing due to the settings in Settings.dat\n";
	}

	mWindow.Make("Madness",gSettings.Width,gSettings.Height,gSettings.Windowed);

	if(!Reset())
		return false;
	
	return true;
}

void Direct3DInterface::Deinitialize()
{
	
	for(UINT i = 0; i < mNumAdapters; i++) {
		SAFE_DELETE_ARRAY(mAdapterModes[i]);
	}
	SAFE_DELETE_ARRAY(mDeviceCaps);
	SAFE_DELETE_ARRAY(mMaxSupportedTextureFormat);
	SAFE_DELETE_ARRAY(mMaxSupportedDepthBufferFormat);
	SAFE_DELETE_ARRAY(mMaxSupportedDisplayFormat);
	SAFE_DELETE_ARRAY(mNumAdapterModes);
	SAFE_DELETE_ARRAY(mAdapterIdentifier);

	SAFE_RELEASE(mDevice);
	SAFE_RELEASE(mDirect3D);
}

bool Direct3DInterface::Reset()
{
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(D3DPRESENT_PARAMETERS));

	//if(gSettings.Height > mAdapterModes[mAdapter][mNumAdapterModes[mAdapter]-1].Height)
	UINT i;
	for(i = 0; i < mNumAdapterModes[mAdapter]; i++) {
		if(gSettings.Height == mAdapterModes[mAdapter][i].Height)
			break;
	}
	if(i == mNumAdapterModes[mAdapter]) {
		gSettings.Height = mAdapterModes[mAdapter][mNumAdapterModes[mAdapter]-1].Height;
		gSettings.Width = mAdapterModes[mAdapter][mNumAdapterModes[mAdapter]-1].Width;
	}
	else
	{
		for(i = 0; i < mNumAdapterModes[mAdapter]; i++) {
			if(gSettings.Width == mAdapterModes[mAdapter][i].Width)
				break;
		}
		if(i == mNumAdapterModes[mAdapter]) {
			gSettings.Height = mAdapterModes[mAdapter][mNumAdapterModes[mAdapter]-1].Height;
			gSettings.Width = mAdapterModes[mAdapter][mNumAdapterModes[mAdapter]-1].Width;

		}
	}

	d3dpp.AutoDepthStencilFormat = mMaxSupportedDepthBufferFormat[mAdapter];
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = mMaxSupportedDisplayFormat[mAdapter];
	d3dpp.BackBufferHeight = gSettings.Height;
	d3dpp.BackBufferWidth = gSettings.Width;
	d3dpp.hDeviceWindow = mWindow.GethWnd();
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.Windowed = gSettings.Windowed;
	d3dpp.Flags = 0;
	//d3dpp.FullScreen_RefreshRateInHz = mAdapterModes[mAdapter][mNumAdapterModes[mAdapter]-1].RefreshRate;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	HRESULT hr;

	if(mDevice)
	{
		gEngine.OnLostDevice();
		if(FAILED(hr = mDevice->Reset(&d3dpp))) {
			LogDxErr(hr,"Device::Reset");
			SAFE_RELEASE(mDevice);
			gEngine.OnDestroyDevice();
			gSettings.DeviceDestroyed = true;
		}
		else
		{
			SetStates();
			gLog << "Device reset.\n";
			return true;
		}
	}




	if(gSettings.VertexProcessing != D3DCREATE_HARDWARE_VERTEXPROCESSING) {
		mDirect3D->CreateDevice(mAdapter,mDeviceType,mWindow.GethWnd(),gSettings.VertexProcessing,&d3dpp,&mDevice);
		if(!mDevice) { 
			gLog ^ "ERROR: Adapter vertex processing was forced to [" ^ EnumToString.Find(gSettings.VertexProcessing,"D3DCREATE") ^ "] which failed.  Attempting recovery...\n";
			gSettings.VertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			return Reset();
		}
	}
	else
	{
		mDirect3D->CreateDevice(mAdapter,mDeviceType,mWindow.GethWnd(),D3DCREATE_HARDWARE_VERTEXPROCESSING,&d3dpp,&mDevice);
		if(!mDevice) {
			mDirect3D->CreateDevice(mAdapter,mDeviceType,mWindow.GethWnd(),D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,&mDevice);
			if(!mDevice)
			{
				if(!Recover()) {
					gLog ^ "ERROR: Could not create device with hardware or software vertex processing.\n";
					return false;
				}
			}
			else
			{
				SetStates();
				gLog << "Hardware vertex processing not available, using software vertex processing.\n";
				return true;
			}
		}
	}
	gLog << "Using hardware vertex processing.\n";
	SetStates();
	return true;
}

bool Direct3DInterface::Recover()
{
	if(!mDevice)
		return false;

	gLog << "Attempting video recovery.\n";

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = mMaxSupportedDisplayFormat[mAdapter];
	d3dpp.BackBufferHeight = 768;
	d3dpp.BackBufferWidth = 1024;
	d3dpp.hDeviceWindow = mWindow.GethWnd();
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;

	if(mDevice->Reset(&d3dpp) != D3D_OK) {
		gLog << "Video recovery failed.\n";
		return false;
	}
	
	SetStates();

	gLog << "Recovered.\n";
	return true;
}

bool Direct3DInterface::FillVertexBuffer(LPDIRECT3DVERTEXBUFFER9 *VertexBuffer, Vertex *StreamData, int NumVertices)
{
	if(*VertexBuffer)
		(*VertexBuffer)->Release();
	mDevice->CreateVertexBuffer(StreamData[0].Size()*NumVertices,0,0,D3DPOOL_MANAGED,VertexBuffer,NULL);
	if(!(*VertexBuffer)) {
		gLog << "FillVertexBuffer failed to create the vertex buffer.\n";
		return false;
	}
	char *Buffer;
	(*VertexBuffer)->Lock(0,0,(void **)&Buffer,0);
	int i, j;
	for(i = 0; i < NumVertices; i++) {
		for(j = 0; j < StreamData[i].PositionDataCount(); j++) {
			memcpy(Buffer,&StreamData[i].PositionData[j],sizeof(D3DXVECTOR3));
			Buffer += sizeof(D3DXVECTOR3);
		}
		for(j = 0; j < StreamData[i].ColorDataCount(); j++) {
			memcpy(Buffer,&StreamData[i].ColorData[j],sizeof(VCOLOR4));
			Buffer += sizeof(VCOLOR4);
		}
		for(j = 0; j < StreamData[i].TextureCoordinateCount(); j++) {
			memcpy(Buffer,&StreamData[i].TextureCoordinates[j],sizeof(VTEXCOORD2));
			Buffer += sizeof(VTEXCOORD2);
		}
	}
	(*VertexBuffer)->Unlock();
	return true;
}

bool Direct3DInterface::SaveScreenshot(const std::string &Filename)
{
	if(!mDevice) {
		gLog ^ "SaveScreenshot() ERROR: Device is NULL.\n";
		return false;
	}

	LPDIRECT3DTEXTURE9 Texture = NULL;
	LPDIRECT3DSURFACE9 RenderSurface = NULL;
	LPDIRECT3DSURFACE9 BackBuffer = NULL;

	if(FAILED(mDevice->CreateTexture(gSettings.Width,gSettings.Height,1,D3DUSAGE_RENDERTARGET,mMaxSupportedTextureFormat[mAdapter],D3DPOOL_DEFAULT,&Texture,NULL))) {
		gLog ^ "SaveScreenshot() ERROR: Could not create texture.  Width[" ^ gSettings.Width ^ "] Height[" ^ gSettings.Height ^ "] Format[" ^ EnumToString.Find(mMaxSupportedTextureFormat[mAdapter],"D3DFMT") ^ "\n";
		return false;
	}

	Texture->GetSurfaceLevel(0,&RenderSurface);
	mDevice->GetRenderTarget(0,&BackBuffer);
	mDevice->SetRenderTarget(0,RenderSurface);

	if(!gEngine.Render(mDevice)) {
		gLog ^ "SaveScreenshot() ERROR: Could not render to texture.\n";
		return false;
	}
	
	mDevice->SetRenderTarget(0,BackBuffer);

	if(FAILED(D3DXSaveTextureToFile(Filename.c_str(),D3DXIFF_PNG,Texture,NULL))) {
		gLog ^ "SaveScreenshot() ERROR: Could not save texture to file [" ^ Filename ^ "]\n";
		return false;
	}

	SAFE_RELEASE(Texture);
	SAFE_RELEASE(BackBuffer);

	gLog << "Saved screenshot to file [" << Filename << "]\n";
		
	return true;
}

bool Direct3DInterface::SupportsTextureFormat(D3DFORMAT Format, DWORD Usage, D3DRESOURCETYPE Type)
{
	HRESULT hr;
	if((hr = mDirect3D->CheckDeviceFormat(mAdapter,gSettings.DeviceType,mMaxSupportedDisplayFormat[mAdapter],Usage,Type,Format)) == D3D_OK)
		return true;
	if(hr == D3DERR_INVALIDCALL) 
		gLog ^ "Direct3DInterface WARNING: CheckDeviceFormat call failed due to an invalid mAdapter value.  mAdapter[" ^ mAdapter ^ "]\n";
	else	
		gLog << "Direct3DInterface: Format[" << EnumToString.Find(Format,"D3DFMT") << "] Usage[" << EnumToString.Find(Usage,"D3DUSAGE") << "] Type[" << EnumToString.Find(Type,"D3DRESOURCETYPE") << "] not supported by Adapter[" << mAdapter << "].\n";
	return false;
}