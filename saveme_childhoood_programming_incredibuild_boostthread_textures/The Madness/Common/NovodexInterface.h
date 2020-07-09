#ifndef NOVODEXINTERFACE_H
#define NOVODEXINTERFACE_H

#define NOMINMAX

#include "NxPhysics.h"
#include <d3dx9.h>

typedef NxPhysicsSDK* LPNXPHYSICSSDK;
typedef NxScene* LPNXSCENE;
typedef NxActor* LPNXACTOR;

#pragma comment(lib,"NxPhysics.lib")

class ExtendedUserContactReport : public NxUserContactReport
{
public:
	virtual void SetActorGroupFlags(NxPhysicsSDK *SDK, NxScene *Scene) = 0;
};


class NovodexInterface
{
private:
	LPNXPHYSICSSDK mSDK;
	LPNXSCENE mScene;
	NxVec3 mDefaultGravity;

	LPNXACTOR mBoxActor;

public: 
	NovodexInterface() : mSDK(NULL), mScene(NULL), mDefaultGravity(0,-9.81f,0) { }

	bool Initialize(ExtendedUserContactReport *Contact = NULL);

	inline LPNXSCENE GetScene() { return mScene; }
	inline LPNXPHYSICSSDK GetSDK() { return mSDK; }

};
extern NovodexInterface gPhysics;
extern NovodexInterface gPhysics2;

extern D3DXVECTOR3 NV3TODV3(NxVec3 V);
extern NxVec3 DV3TONV3(D3DXVECTOR3 V);

#endif