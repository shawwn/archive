#define NOMINMAX
#include "XMeshInterface.h"
#include "PlayerInterface.h"
#include "NetworkInterface.h"
#include "GameInterface.h"
#include "ClientSettingsInterface.h"
#include "LoggingInterface.h"
#include "GUIInterface.h"
#include "InputInterface.h"
#include "Vertex.h"
#include "NovodexInterface.h"
#include "EffectInterface.h"
#include "TextureInterface.h"
#include "TimeInterface.h"
#include "LightingInterface.h"
#include "BufferInterface.h"
#include "VertexDeclarationInterface.h"
#include "ObjectInterface.h"
#include "ContactReport.h"
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
	if(!gSettings.Load("Settings.cfg")) {
		gLog << "WARNING: Error loading settings.\n";
	}

	gLog << "Initializing Direct3D... \n";
	if(!gD3D.Initialize(gSettings.Adapter,gSettings.DeviceType)) {
		gLog << "WARNING: Direct3D failed initialization.\n";
	}
	LPDIRECT3DDEVICE9 Device = gD3D.GetDevice();
	OnResetDevice(Device);

	//gGUI.AddWindow("Hi",500,500,20,20);

	gPhysics.Initialize(&gContact);
	
	if(!gInput.Initialize()) {
		gLog ^ "ERROR: Could not initialize input.\n";
		return false;
	}

	
	Heightmap map;
	//map.BuildMesh("Data/Heightmaps/final.jpg",Device,&mStaticVerts,0,&mStaticIndices,0);
	mNumVertices = map.GetHeight()*map.GetWidth();
	mNumIndices = ((map.GetWidth()-1)*2)*(map.GetHeight()-1)*3;
	mNumTriangles = (map.GetWidth()-1)*2*(map.GetHeight()-1);

	mHeightEffectIndex = gEffects.Load("Data/Shaders/new_material_4.fx");

	mPlayer.Initialize(gPhysics.GetScene());

	//mCloud.Initialize(16,16,16);

	if(!gTime.Initialize()) { 
		gLog ^ "WARNING: CPU does not support QueryPerformanceCounter()\n";
	}
	gTextures.Load("Data/Textures/cloud.jpg");
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);

	mPlane.Initialize(Device,(float)gSettings.PlaneSize,(float)gSettings.PlaneSize);

	mSkydome.GenerateDome(9000,1,1,20,20);

	//mEmitter.SetScene(gPhysics.GetScene());
	//mEmitter.Initialize();

	if(!mBSP.Load("Data/Maps/woot.bsp")) {
		return false;
	}
	if(gMeshes.Load("Data/Models/tiny_4anim.x") == -1)
		return false;

	gLog << "Connecting to server [" << gSettings.ServerAddress << "] on port [" << gSettings.ServerPort << "]\n";
	gNetwork.Start(gSettings.ServerAddress,gSettings.ServerPort);

	return true;
}

bool GameInterface::Update(LPNXSCENE Scene)
{
	Sleep(5);
	if(!gTime.Update()) {
		gLog ^ "WARNING: TimeInterface::Update() failed.\n";
	}
	mOneSecond += gTime.GetDeltaTime();
	mFPSCount++;
	if(mOneSecond > 1000) {
		mFPS = mFPSCount;
		mFPSCount = 0;
		mOneSecond = 0;
	}
	
	/*
	Scene->simulate((NxReal)gTime.GetDeltaTime());
	Scene->flushStream();
	Scene->fetchResults(NX_RIGID_BODY_FINISHED,true);
	*/

	gInput.Update();

	gNetwork.Update();

	mPlayer.Update(gTime.GetDeltaTimeInSeconds());

	//mEmitter.Update(gTime.GetDeltaTimeInSeconds());

	//mCloud.Update(timenow-timelast);

	/*gLog << mEmitter.Stats() << " FPS[" << GetFPS() << "]";
	SetWindowText(gSettings.hWnd,gLog.GetStream().str().c_str());
	gLog.GetStream().str(std::string());*/


	return true;
}
	
bool GameInterface::Render(LPDIRECT3DDEVICE9 Device)
{
	//static LPDIRECT3DVERTEXBUFFER9 vb = NULL;
	//if(!vb) { if(FAILED(Device->CreateVertexBuffer(1024*sizeof(MyParticle),D3DUSAGE_DYNAMIC,D3DFVF_XYZ,D3DPOOL_MANAGED,&vb,NULL))) { return false; } }
	

	HRESULT hr = Device->TestCooperativeLevel();
	if(hr == D3DERR_DEVICELOST) {
		Sleep(50);
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
	mSkydome.Render(Device);

	//if(!mEmitter.Render(gTime.GetDeltaTimeInSeconds(),Device,mCamera.GetMatView(),mCamera.GetEye(),0))
		//return false;

	//mBSP.Render(Device);

	static LPD3DXMESH Mesh = NULL;

	if(!Mesh) {
		if(FAILED(D3DXCreateSphere(Device,5.0f,32,32,&Mesh,NULL))) {
			gLog ^ "Gwar\n";
		}
	}

	Device->SetFVF(D3DFVF_XYZ);
	D3DXMATRIX Mat;

	D3DXMatrixIdentity(&Mat);

	static bool RenderNetwork = false;
	if(gInput.GetKeyboard().KeyDownOnce(DIK_R))
		RenderNetwork = !RenderNetwork;

	for(PlayerInterface::Iterator it = gPlayers.Begin(); it != gPlayers.End(); it++) {
		Player *ply = (*it).second;
		if(ply->GetGUID() == gNetwork.GetGUID())
			continue;
	
		XMesh *x = (XMesh *)ply;

		//D3DXMatrixIdentity(&Mat);
		//D3DXMatrixTranslation(&Mat,ply->GetPosition().x,ply->GetPosition().y,ply->GetPosition().z);
		//Device->SetTransform(D3DTS_WORLD,&Mat);
		//Mesh->DrawSubset(0);
		x->Render(Device);

	}

	for(ObjectInterface::Iterator it = gObjects.Begin(); it != gObjects.End(); it++) {
		Object *obj = (*it).second;

		D3DXMatrixIdentity(&Mat);
		D3DXMatrixTranslation(&Mat,obj->GetPosition().x,obj->GetPosition().y,obj->GetPosition().z);
		Device->SetTransform(D3DTS_WORLD,&Mat);
		Device->SetTexture(0,NULL);
		Mesh->DrawSubset(0);

	}

	NxVec3 SetPos = mPlayer.GetEye();
	SetPos.y += 4.0f;

	mCamera.Set(SetPos,mPlayer.GetView(),mPlayer.GetUp());
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
	gNetwork.Stop("GameInterface::Deinitialize()");
}

bool GameInterface::Start(HINSTANCE hInstance)
{
	
	gLog.SetOutputFile("Game.log");
	gLog << "Session start.\n";
	gSettings.hInstance = hInstance;
	if(!Initialize()) {
		gLog ^ "ERROR: Game initialization failed.\n";
	}
	
	MSG msg;
	while(!gSettings.Done)
	{
		//gSettings.Done = true;
		if(PeekMessage(&msg,gD3D.GethWnd(),0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if(!Update(gPhysics.GetScene())) {
			gLog ^ "WARNING: <GameInterface> Update() returned false.\n";
			gSettings.Done = true;
		}
		if(!Render(gD3D.GetDevice())) {
			gLog ^ "WARNING: <GameInterface> Render() returned false.\n";
			gSettings.Done = true;
		}
	}


	Deinitialize();

	gLog << "Session end.\n";
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
	mEmitter.Reset();
}