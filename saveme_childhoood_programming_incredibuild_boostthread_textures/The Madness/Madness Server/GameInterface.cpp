
#define NOMINMAX
#include "PlayerInterface.h"
#include "GameInterface.h"
#include "LoggingInterface.h"
#include "InputInterface.h"
#include "Vertex.h"
#include "NovodexInterface.h"
#include "EffectInterface.h"
#include "TextureInterface.h"
#include "TimeInterface.h"
#include "LightingInterface.h"
#include "BufferInterface.h"
#include "VertexDeclarationInterface.h"
#include "ServerInterface.h"
#include "ClientSettingsInterface.h"
#include "ObjectInterface.h"
#include <list>
using namespace std;

GameInterface gEngine;	

void GameInterface::RunTests()
{
	gSettings.Test();
}

bool GameInterface::Initialize()
{
	gLog << "Loading settings... \n";
//	if(!gSettings.Load("Settings.cfg")) {
//		gLog << "WARNING: Error loading settings.\n";
//	}

	gLog << "Initializing Direct3D... \n";
	if(!gD3D.Initialize(gSettings.Adapter,gSettings.DeviceType)) {
		gLog << "WARNING: Direct3D failed initialization.\n";
	}
	LPDIRECT3DDEVICE9 Device = gD3D.GetDevice();
	OnResetDevice(Device);

	//gPhysics.Initialize();
	
	if(!gInput.Initialize()) {
		gLog ^ "ERROR: Could not initialize input.\n";
		return false;
	}

	mPlayer.Initialize(gPhysics.GetScene());

	if(!gTime.Initialize()) { 
		gLog ^ "WARNING: CPU does not support QueryPerformanceCounter()\n";
	}
	gTextures.Load("Data/Textures/cloud.jpg");
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);

	mPlane.Initialize(Device,(float)gSettings.PlaneSize,(float)gSettings.PlaneSize);

	//mSkydome.GenerateDome(9000,1,1,20,20);

	//mEmitter.SetScene(gPhysics.GetScene());
	//mEmitter.Initialize();

	return true;
}

bool GameInterface::Update(LPNXSCENE Scene)
{
	mOneSecond += gTime.GetDeltaTime();
	mFPSCount++;
	while(mOneSecond > 1000) {
		mFPS = mFPSCount;
		mFPSCount = 0;
		mOneSecond -= 1000;
	}

	gInput.Update();

	if(!mPlayer.Update(gTime.GetDeltaTimeInSeconds()))
		return false;

	return true;
}
	
bool GameInterface::Render(LPDIRECT3DDEVICE9 Device)
{
	//static LPDIRECT3DVERTEXBUFFER9 vb = NULL;
	//if(!vb) { if(FAILED(Device->CreateVertexBuffer(1024*sizeof(MyParticle),D3DUSAGE_DYNAMIC,D3DFVF_XYZ,D3DPOOL_MANAGED,&vb,NULL))) { return false; } }
	

	HRESULT hr = Device->TestCooperativeLevel();
	if(hr == D3DERR_DEVICELOST) {
		//Sleep(50);
		return true;
	}
	if(hr == D3DERR_DRIVERINTERNALERROR) { 
		gLog ^ "Internal video driver error.  Exiting...\n";
		return false;
	}
	if(hr == D3DERR_DEVICENOTRESET) {
		if(!gD3D.Reset()) {
			gLog << "Reset failed.\n";
			return true;
		}
		OnResetDevice(Device);
		gLog << "success.\n";
		hr = D3D_OK;
	}

	if(FAILED(Device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,RGB(20,20,20),1.0f,0))) {
		gLog % "Device::Clear failed.\n";
		return true;
	}
	if(FAILED(Device->BeginScene())) {
		gLog % "Device::BeginScene failed.\n";
		return true;
	}


	mPlane.Render(Device);
	//mSkydome.Render(Device);
	

	static LPD3DXMESH Mesh = NULL;

	if(!Mesh) {
		if(FAILED(D3DXCreateSphere(Device,5.0f,64,64,&Mesh,NULL))) {
			gLog ^ "Gwar\n";
		}
	}
	

	Device->SetFVF(D3DFVF_XYZ);
	D3DXMATRIX Mat;

	D3DXMatrixIdentity(&Mat);

	for(PlayerInterface::Iterator it = gPlayers.Begin(); it != gPlayers.End(); it++) {
		Player *ply = (*it).second;

		D3DXMatrixIdentity(&Mat);
		D3DXMatrixTranslation(&Mat,ply->GetPosition().x,ply->GetPosition().y,ply->GetPosition().z);
		Device->SetTransform(D3DTS_WORLD,&Mat);
		Mesh->DrawSubset(0);
	}

	for(ObjectInterface::Iterator it = gObjects.Begin(); it != gObjects.End(); it++) {
		Object *obj = (*it).second;

		D3DXMatrixIdentity(&Mat);
		D3DXMatrixTranslation(&Mat,obj->GetPosition().x,obj->GetPosition().y,obj->GetPosition().z);
		Device->SetTransform(D3DTS_WORLD,&Mat);
		Mesh->DrawSubset(0);
	}

	mCamera.Set(mPlayer.GetEye(),mPlayer.GetView(),mPlayer.GetUp());
	mCamera.Look(Device);

	Device->EndScene();
	if(FAILED(Device->Present(NULL,NULL,0,NULL))) {
		gLog ^ "Present error.\n";
		return false;
	}
	return true;
}

void GameInterface::Deinitialize()
{
}

bool GameInterface::Start()
{
	gLog << "Session start.\n";
	gSettings.hInstance = GetModuleHandle(NULL);
	if(!Initialize()) {
		gLog ^ "ERROR: Game initialization failed.\n";
	}
	
		//gSettings.Done = true;


	gLog << "Session end.\n";
	return true;
}

bool GameInterface::Loop()
{
	MSG msg;
	if(PeekMessage(&msg,gD3D.GethWnd(),0,0,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//if(GetActiveWindow() != gSettings.hWnd)
		//Sleep(50);
	if(!Update(gPhysics.GetScene())) {
		gLog ^ "WARNING: <GameInterface> Update() returned false.\n";
		gSettings.Done = true;
	}
	if(!Render(gD3D.GetDevice())) {
		gLog ^ "WARNING: <GameInterface> Render() returned false.\n";
		gSettings.Done = true;
	}
	return true;
	
}

void GameInterface::OnLostDevice()
{
	gTextures.OnLostDevice();
	gBuffers.OnLostDevice();
	gEffects.OnLostDevice();
}

void GameInterface::OnDestroyDevice()
{
	gTextures.OnDestroyDevice();
	gBuffers.OnDestroyDevice();
	gDeclarations.OnDestroyDevice();
	gEffects.OnDestroyDevice();
}

void GameInterface::OnResetDevice(LPDIRECT3DDEVICE9 Device)
{
	if(gSettings.DeviceDestroyed)
		OnDestroyDevice();
	gSettings.DeviceDestroyed = false;

	gEffects.OnResetDevice(Device);
	gTextures.OnResetDevice(Device);
	gBuffers.OnResetDevice(Device);
	gDeclarations.OnResetDevice(Device);
}