#include "dxstdafx.h"
#include "ClientNetworking.h"
#include "dxut.h"
#include "Engine.h"
#include "LoggingInterface.h"
#include "ConfigurationInterface.h"
#include "MaterialManager.h"
#include "Quake3BSP.h"
#include "InputInterface.h"
#include "MathInterface.h"
#include "Heightmap.h"
#include "ParsingInterface.h"
#include "EffectManager.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"gdiplus.lib")
#pragma comment(lib,"ws2_32.lib")

Engine *gEngine;

Engine::Engine()
{
	gLog = NULL;
	gParsing = NULL;
	gMaterials = NULL;
	gMouse = NULL;
	gInput = NULL;
	gMath = NULL;
	gNetworking = NULL;
	gEffects = NULL;

	mDone = false;
}

Engine::~Engine()
{
	Destroy();
}

void Engine::FirstCall()
{
	gLog = new LoggingInterface;
	gLog->SetOutputFile("Game.log");
	gLog->Stream << "Engine: Initializing...";
	gLog->Print();

	gInput = new Input;
	gInput->Init();

	gParsing = new ParsingInterface;

	gConfig = new ConfigurationInterface; // Necessary
	gConfig->Load(gParsing->GetPath("Game.cfg"));


	gMath = new MathInterface;

	gMouse = new Mouse;
	gMouse->SetSmoothingDegree(gConfig->GetMouseSmoothingDegree());
	gMouse->SetSensitivity(gConfig->GetMouseSensitivity());

	gNetworking = new ClientNetworking;
}

HRESULT Engine::Init(LPDIRECT3DDEVICE9 Device)
{
	//DXUTSetHandleDefaultHotkeys(false);

	gMaterials = new MaterialManager;
	gMaterials->SetBasePath(gConfig->GetPath().c_str());

	gEffects = new EffectManager;
	
	mWorld.Initialize(Device);
	mConsole.ProcessCommand("hmap");
	mConsole.ProcessCommand("map");
	mConsole.ProcessCommand("resolution");
	mConsole.ProcessCommand("lightmaps");
	mConsole.ProcessCommand("deallocate");

	if(!gNetworking->Start("Networking.cfg"))
		gLog->Print("Networking: Could not start.");

	ConfigParameter *cfgIP = gNetworking->Config().Find("IP");
	if(!cfgIP)
	{
		gLog->Stream << "Networking: You must specify an IP address in the Networking.cfg file.";
		gLog->Print();
		gNetworking->Stop();
	}

	ConfigParameter *cfgPort = gNetworking->Config().Find("Port");
	if(!cfgPort)
	{
		gLog->Stream << "Networking: You must specify a port in the Networking.cfg file.";
		gLog->Print();
		gNetworking->Stop();
	}

	int Port = atoi(cfgPort->Arguments[0].c_str());
	if(Port == 0)
	{
		gLog->Stream << "Networking: Invalid port specified in the Networking.cfg file.";
		gLog->Print();
		gNetworking->Stop();
	}

	ConfigParameter *cfgUsername = gNetworking->Config().Find("Username");
	if(!cfgUsername)
	{
		gLog->Stream << "Networking: You must specify a username in the Networking.cfg file.";
		gLog->Print();
		gNetworking->Stop();
	}

	ConfigParameter *cfgPassword = gNetworking->Config().Find("Password");
	if(!cfgPassword)
	{
		gLog->Stream << "Networking: You must specify a password in the Networking.cfg file.";
		gLog->Print();
		gNetworking->Stop();
	}

	if(gNetworking->Running())
	{
		if(!gNetworking->ConnectToLoginServer(cfgIP->Arguments[0],Port,cfgUsername->Arguments[0],cfgPassword->Arguments[0]))
		{
			gLog->Stream << "Networking: Connect to " << cfgIP->Arguments[0] << " on port " << Port << " failed.";
			gLog->Print();
			gNetworking->Stop();
		}
	}

	Mesh.Load(Device,"tiny.x");


	return ENG_OK;
}

HRESULT Engine::OnLostDevice()
{
	if(gInput)
		delete gInput;
	gInput = NULL;

	mWorld.OnLostDevice();
	mConsole.OnLostDevice();
	
	gEffects->OnLostDevice();

	return ENG_OK;
}

HRESULT Engine::OnResetDevice(LPDIRECT3DDEVICE9 Device, const D3DSURFACE_DESC* BackBuffer)
{
	if(!Device || ! BackBuffer)
		return ENG_ERR;

	mDeviceSettings = DXUTGetDeviceSettings();

	if(!gInput)
		gInput = new Input;
	gInput->Init();

	gConfig->SetBackBuffer(BackBuffer);

	RECT WindowRect = DXUTGetWindowClientRect();
	gMouse->SetCenter(BackBuffer->Width,BackBuffer->Height,WindowRect.left,WindowRect.top);
	gMouse->Center();

	Device->SetRenderState(D3DRS_LIGHTING,FALSE);
	Device->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

	DWORD Filter = gConfig->GetFilter();
	Device->SetSamplerState(0,D3DSAMP_MAGFILTER,Filter);
	Device->SetSamplerState(0,D3DSAMP_MINFILTER,Filter);
	Device->SetSamplerState(0,D3DSAMP_MIPFILTER,Filter);

	Device->SetSamplerState(1,D3DSAMP_MAGFILTER,Filter);
	Device->SetSamplerState(1,D3DSAMP_MINFILTER,Filter);

	mWorld.OnResetDevice(Device);
	mConsole.Initialize(0,0,gConfig->GetWidth(),gConfig->GetHeight()/2,20,true, false);
	mConsole.OnResetDevice(Device);

	gEffects->OnResetDevice(Device);

	return ENG_OK;
}

void Engine::Update()
{
	if(DXUTGetHWND() == GetActiveWindow())
	{
		gInput->Update();
		
		if(gInput->KeyDownOnce(gConfig->GetKeyMenu()))
		{
			mDone = true;
		}
		if(gInput->KeyDownOnce(gConfig->GetKeyConsole()) && !mWorld.Chat().HasFocus())
			mConsole.ToggleEnabled();
		if(mDone) {
			SendMessage(DXUTGetHWND(),WM_CLOSE,0,0);
		}

		mWorld.Update();
		mWorld.ActiveUpdate();
	}
	if(gNetworking->Running())
		if(!gNetworking->Update())
			gNetworking->Stop();
}

HRESULT Engine::Render(LPDIRECT3DDEVICE9 Device)
{
	if(!Device)
		return ENG_ERR;

	mWorld.Render(Device);
	
	Mesh.Render(Device);

	mConsole.Render(Device);

	

	return ENG_OK;
}

bool Engine::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	if(!mConsole.MsgProc(hWnd,uMsg,wParam,lParam) && !mConsole.GetEnabled())
		mWorld.MsgProc(hWnd,uMsg,wParam,lParam);

	while(mConsole.CommandCount() > 0)
	{
		ConsoleCommand cmd = mConsole.GetNextCommand();
		if(cmd.mCommand == L"MAP")
		{
			if(cmd.ArgCount() == 1)
			{
				Quake3BSP *bsp = new Quake3BSP;
				if(!bsp->Load(DXUTGetD3DDevice(),gParsing->GetPath(WideStringToString(cmd.mArguments[0])),BSP_SWAPYZ,gConfig->GetUseLightmaps(),gConfig->GetLightmapIntensity())) {
					delete bsp;
					continue;
				}
				mWorld.Buildings().Clear();
				mWorld.AddToWorld(bsp);
			}
		}
		if(cmd.mCommand == L"HMAP")
		{
			if(cmd.ArgCount() == 1)
			{
				Heightmap *hgt = new Heightmap;
				if( !hgt->Load(DXUTGetD3DDevice(),gParsing->GetPath(WideStringToString(cmd.mArguments[0])),gConfig->GetUnitsPerMeter(),500.0f) ) {
					delete hgt;
					continue;
				}
				mWorld.Landscapes().Clear();
				mWorld.AddToWorld(hgt);
			}
			if(cmd.ArgCount() == 3)
			{
				Heightmap *hgt = new Heightmap;
				if( !hgt->Load(DXUTGetD3DDevice(),
					gParsing->GetPath(WideStringToString(cmd.mArguments[0])),
					(float)atof(WideStringToString(cmd.mArguments[1]).c_str()),
					(float)atof(WideStringToString(cmd.mArguments[2]).c_str())) ) {
					delete hgt;
					continue;
				}
				mWorld.Landscapes().Clear();
				mWorld.AddToWorld(hgt);
			}
		}
		if(cmd.mCommand == L"DEALLOCATE")
		{
			mWorld.Players().Clear();
			mWorld.Objects().Clear();
			mWorld.Landscapes().Clear();
			mWorld.Buildings().Clear();
			gMaterials->Clear();
		}
		if(cmd.mCommand == L"RESOLUTION")
		{
			if(cmd.ArgCount() == 3)
				gEngine->ChangeResolution(
				atoi(WideStringToString(cmd.mArguments[0]).c_str()),
				atoi(WideStringToString(cmd.mArguments[1]).c_str()),
				((atoi(WideStringToString(cmd.mArguments[2]).c_str())==1)?true:false));
			else
				gEngine->ChangeResolution(
				atoi(WideStringToString(cmd.mArguments[0]).c_str()),
				atoi(WideStringToString(cmd.mArguments[1]).c_str()),
				gConfig->GetWindowed());
		}
		if(cmd.mCommand == L"LIGHTMAPS")
		{
			if(mWorld.Buildings().Count() <= 0)
				continue;
			Quake3BSP *Obj = (Quake3BSP *)mWorld.Buildings().Begin()->second;
			Obj->ToggleUseLightmaps();
		}
	}

	return true;
}

bool Engine::ChangeResolution(const int &Width, const int &Height, const bool &Fullscreen)
{
	mDeviceSettings.pp.BackBufferWidth = Width;
	mDeviceSettings.pp.BackBufferHeight = Height;

	mDeviceSettings.pp.Windowed = !Fullscreen;
	if(mDeviceSettings.pp.Windowed)
	{
		mDeviceSettings.pp.BackBufferFormat = D3DFMT_UNKNOWN;
		mDeviceSettings.pp.FullScreen_RefreshRateInHz = 0;
	}
	else
	{
		mDeviceSettings.pp.BackBufferFormat = gConfig->GetBackBufferFormat();
	}
	DXUTCreateDeviceFromSettings(&mDeviceSettings);

	gConfig->SetWindowed(!Fullscreen);

	return true;
}

void Engine::Print(const wstring &str)
{
	//mWorld.Print(str);
	mConsole.Print(str);
}

void Engine::Destroy()
{
	mWorld.Destroy();
	mConsole.Destroy();
}

void Engine::LastCall()
{
	mWorld.Destroy();

	if(gNetworking)
		delete gNetworking;
	gNetworking = NULL;

	if(gMaterials)
		delete gMaterials;
	gMaterials = NULL;

	if(gConfig)
		delete gConfig;
	gConfig = NULL;

	if(gParsing)
		delete gParsing;
	gParsing = NULL;

	if(gLog)
		delete gLog;
	gLog = NULL;

	if(gMouse)
		delete gMouse;
	gMouse = NULL;

	if(gMath)
		delete gMath;
	gMath = NULL;

	ShowCursor(true);
}
