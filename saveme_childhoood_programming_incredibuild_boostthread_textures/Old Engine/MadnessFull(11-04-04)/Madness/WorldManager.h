#ifndef _WORLDMANAGER_H
#define _WORLDMANAGER_H

#include "ObjectManager.h"
#include "Camera.h"
#include "FirstPersonPhysics.h"
#include "Console.h"

class WorldManager
{
private:
	Camera mCamera;
	FirstPersonPhysics mPhysics;

	ObjectManager mLandscapes;
	ObjectManager mBuildings;
	ObjectManager mPlayers;
	ObjectManager mObjects;

	Console mChat;
public:
	WorldManager();
	~WorldManager();

	void Destroy();
	bool Initialize(LPDIRECT3DDEVICE9 Device);
	void OnLostDevice();
	bool OnResetDevice(LPDIRECT3DDEVICE9 Device);
	bool AddToWorld(Object *obj);
	bool Render(LPDIRECT3DDEVICE9 Device);
	bool Update();
	bool ActiveUpdate();
	bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Print(const wstring &str);

	ObjectManager &Landscapes() { return mLandscapes; }
	ObjectManager &Buildings() { return mBuildings; }
	ObjectManager &Players() { return mPlayers; }
	ObjectManager &Objects() { return mObjects; }
	Console &Chat() { return mChat; }

	const D3DXMATRIX &GetWorldMatrix() { return mCamera.GetWorldMatrix(); }
	const D3DXMATRIX &GetViewMatrix() { return mCamera.GetViewMatrix(); }
	const D3DXMATRIX &GetProjMatrix() { return mCamera.GetProjMatrix(); }
	D3DXMATRIX GetViewProjMatrix() { return mCamera.GetViewProjMatrix(); }

};

#endif