
#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#define NOMINMAX
#include <windows.h>
#include <list>
#include <time.h>
#include "Direct3DInterface.h"
#include "LoggingInterface.h"
#include "NovodexInterface.h"
#include "Cam.h"
#include "ServerViewer.h"
#include "Heightmap.h"
#include "Effect.h"
#include "Particle.h"
#include "Plane.h"
#include "LightingInterface.h"
#include "Skydome.h"
#include "Block.h"
#include "Q3BSP.h"

class GameInterface
{
private:
	void RunTests();
	bool Initialize();
	Camera mCamera;
	ServerViewer mPlayer;

	LPDIRECT3DVERTEXBUFFER9 mStaticVerts;
	LPDIRECT3DINDEXBUFFER9 mStaticIndices;
	int mNumVertices;
	int mNumIndices;
	int mNumTriangles;

	float mOneSecond;
	int mFPSCount;
	int mFPS;

	Effect mHeightEffect;
	UINT mHeightEffectIndex;

	Plane mPlane;
	Skydome mSkydome;
	Q3BSP mBSP;

public:
	GameInterface() : mStaticVerts(NULL), mStaticIndices(NULL), mNumVertices(0), mNumIndices(0), mOneSecond(0), mFPSCount(0), mFPS(0) {} 
	~GameInterface() { SAFE_RELEASE(mStaticVerts); SAFE_RELEASE(mStaticIndices); }
	void Deinitialize();
	bool Start();
	bool Update(LPNXSCENE Scene);
	bool Render(LPDIRECT3DDEVICE9 Device);

	bool Loop();
	
	void OnResetDevice(LPDIRECT3DDEVICE9 Device);
	void OnDestroyDevice();
	void OnLostDevice();

	inline D3DXMATRIX GetMatVP() { return mCamera.GetMatView()*mCamera.GetMatProj(); }
	inline int GetFPS() const { return mFPS; }
	inline Camera &GetCamera() { return mCamera; }
	inline ServerViewer &GetPlayer() { return mPlayer; }
};

extern GameInterface gEngine;

#endif