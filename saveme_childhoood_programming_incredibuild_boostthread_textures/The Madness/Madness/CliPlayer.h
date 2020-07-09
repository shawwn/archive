#ifndef CLIPLAYER_H
#define CLIPLAYER_H

#include "Object.h"
#include "FirstPersonOrientation.h"
#include "LightingInterface.h"

class CliPlayer
{
private:
	FirstPersonOrientation mOrientation;
	Light *mLight;
public:
	CliPlayer() { mOrientation.Set(NxVec3(0,0,-30),NxVec3(0,0,1),NxVec3(0,1,0)); mLight = NULL;	}
	~CliPlayer() { gLights.ReleaseLight(mLight); }

	bool Initialize(LPNXSCENE mScene);
	bool Update(float fDeltaTimeInSeconds);

	NxVec3 GetEye();
	NxVec3 GetVelocity();
	inline NxVec3 GetView() { return mOrientation.GetView(); }
	inline NxVec3 GetUp() { return mOrientation.GetUp(); }
	inline FirstPersonOrientation &GetOrientation() { return mOrientation; }

	void SetEye(const NxVec3 &Eye);
	void SetVelocity(const NxVec3 &Vel);	

	inline void SetView(const NxVec3 &View) { mOrientation.SetView(View); }
	inline void SetUp(const NxVec3 &Up) { mOrientation.SetUp(Up); }
};

#endif