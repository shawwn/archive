#ifndef _ENGINE_H
#define _ENGINE_H

#include "ObjectManager.h"
#include "MaterialManager.h"
#include "FirstPersonPhysics.h"
#include "Camera.h"
#include "Console.h"
#include "WorldManager.h"
#include "SkinnedMesh.h"

class Engine
{
private:
	DXUTDeviceSettings mDeviceSettings;
	bool mDone;

	SkinnedMesh Mesh;

	WorldManager mWorld;
	Console mConsole;
public:
	Engine();
	~Engine();

	void FirstCall();
	HRESULT Init(LPDIRECT3DDEVICE9 Device);
	HRESULT OnResetDevice(LPDIRECT3DDEVICE9 Device, const D3DSURFACE_DESC* BackBuffer);
	HRESULT OnLostDevice();
	void Update();
	HRESULT Render(LPDIRECT3DDEVICE9 Device);
	bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool ChangeResolution(const int &Width, const int &Height, const bool &Fullscreen);
	void Print(const wstring &str);
	void Destroy();
	void LastCall();

	const D3DXMATRIX &GetWorldMatrix() { return mWorld.GetWorldMatrix(); }
	const D3DXMATRIX &GetViewMatrix() { return mWorld.GetViewMatrix(); }
	const D3DXMATRIX &GetProjMatrix() { return mWorld.GetProjMatrix(); }
	D3DXMATRIX GetViewProjMatrix() { return mWorld.GetViewProjMatrix(); }

	WorldManager &World() { return mWorld; }
};
extern Engine *gEngine;

enum EngineRet
{
	ENG_ERR = 0,
	ENG_OK = 1
};

#endif