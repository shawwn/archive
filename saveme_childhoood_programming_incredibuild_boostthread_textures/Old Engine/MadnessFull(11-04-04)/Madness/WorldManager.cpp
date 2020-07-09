#include "dxstdafx.h"
#include "WorldManager.h"
#include "ConfigurationInterface.h"
#include "LoggingInterface.h"
#include "InputInterface.h"
#include "ParsingInterface.h"
#include "Quake3BSP.h"
#include "Heightmap.h"
#include "Engine.h"
#include "ClientNetworking.h"

WorldManager::WorldManager()
{

}

WorldManager::~WorldManager()
{
	Destroy();
	
	mLandscapes.Clear();
	mBuildings.Clear();
	mPlayers.Clear();
	mObjects.Clear();

	mChat.Destroy();
}

void WorldManager::Destroy()
{
	mCamera.Clear();
}

bool WorldManager::Initialize(LPDIRECT3DDEVICE9 Device)
{
	gConfig->SetLookWithMouse(true);
	
	RECT rect;
	GetWindowRect(DXUTGetHWND(),&rect);
	mCamera.Proj(45.0f,rect.right/(float)rect.bottom,1.0f,100000.0f);
	D3DXVECTOR3 Pos(10,10,0);
	D3DXVECTOR3 Facing(0,0,1);
	D3DXVECTOR3 Up(0,1,0);
	mPhysics.Set(Pos,Facing,Up);

	mChat.SetEnabled(true);

	return true;
}

void WorldManager::OnLostDevice()
{
	mChat.OnLostDevice();
}

bool WorldManager::OnResetDevice(LPDIRECT3DDEVICE9 Device)
{
	if(!Device)
		return false;
	
	int Width = 500;
	int Height = 200;
	mChat.Initialize(gConfig->GetWidth()/2-Width/2,0,Width,Height,5,false, true);
	mChat.SetEnabled(true);
	mChat.OnResetDevice(Device);

	return true;
}

bool WorldManager::AddToWorld(Object *obj)
{
	if(!obj)
		return false;
	switch(obj->GetType())
	{
	case 0:
		mLandscapes.Add(obj);
		return true;
	case 1:
		mBuildings.Add(obj);
		return true;
	case 2:
		mPlayers.Add(obj);
		return true;
	case 3:
		mObjects.Add(obj);
		return true;
	}
	return false;
}

bool WorldManager::Render(LPDIRECT3DDEVICE9 Device)
{
	if(!Device)
		return false;
	mCamera.Look(Device);

	ObjectManager::Iterator it;
	for(it = mLandscapes.Begin(); it != mLandscapes.End(); it++)
		(*it).second->Render(Device);
	for(it = mBuildings.Begin(); it != mBuildings.End(); it++)
		(*it).second->Render(Device);
	for(it = mPlayers.Begin(); it != mPlayers.End(); it++)
		(*it).second->Render(Device);
	for(it = mObjects.Begin(); it != mObjects.End(); it++)
		(*it).second->Render(Device);

	mChat.Render(Device);

	return true;
}

bool WorldManager::Update()
{
	mPhysics.Update();
	
	return true;
}

bool WorldManager::ActiveUpdate()
{
	if(gConfig->GetLookWithMouse())
	{
		gMouse->Update();
		gMouse->Center();
		mPhysics.RotateFirstPerson(gMouse->GetDifferenceY(),gMouse->GetDifferenceX());
	}
	mCamera.View(mPhysics.GetPosition(),mPhysics.GetFacing(),mPhysics.GetUp());

	if(gInput->KeyDownOnce(gConfig->GetKeyLookWithMouse()) && !mChat.HasFocus()) {
		gConfig->ToggleLookWithMouse();
	}
	
	return true;
}

bool WorldManager::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!mChat.MsgProc(hWnd,uMsg,wParam,lParam)) {
		mPhysics.UpdateWithKeyboard();
	}
	return true;
}

void WorldManager::Print(const wstring &str)
{
	mChat.Print(str);
}