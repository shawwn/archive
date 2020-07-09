#ifndef SERVERVIEWER_H
#define SERVERVIEWER_H

#include "Object.h"
#include "FirstPersonOrientation.h"
#include "LightingInterface.h"

class ServerViewer
{
private:
	FirstPersonOrientation mOrientation;
	Light *mLight;
	Object mObject;
public:
	NxVec3 mNetPosition;
	NxVec3 mNetVelocity;

	ServerViewer() : mNetPosition(NxVec3(0,0,0)), mNetVelocity(NxVec3(0,0,0)) { mOrientation.Set(NxVec3(0,0,-30),NxVec3(0,0,1),NxVec3(0,1,0)); mLight = NULL;	}
	~ServerViewer() { gLights.ReleaseLight(mLight); }

	bool Initialize(LPNXSCENE mScene);
	bool Update(float fDeltaTimeInSeconds);

	inline NxVec3 GetEye() { return mOrientation.GetEye(); }
	inline NxVec3 GetView() { return mOrientation.GetView(); }
	inline NxVec3 GetUp() { return mOrientation.GetUp(); }
	inline NxVec3 GetVelocity() { return mObject.GetActor()->getLinearVelocity(); }
	inline FirstPersonOrientation &GetOrientation() { return mOrientation; }
	inline LPNXACTOR GetActor() const { return mObject.GetActor(); }
	inline const NxVec3 &GetAcceleration() { return mObject.GetAcceleration(); }
	inline Object &GetObject() { return mObject; }

	inline void SetEye(const NxVec3 &Eye) { if(mObject.GetActor()) { mOrientation.SetEye(Eye); mObject.GetActor()->setGlobalPosition(Eye); } }
	inline void SetVelocity(const NxVec3 &Vel) { if(mObject.GetActor()) { mObject.GetActor()->setLinearVelocity(Vel); } }
	inline void SetView(const NxVec3 &View) { mOrientation.SetView(View); }
	inline void SetUp(const NxVec3 &Up) { mOrientation.SetUp(Up); }
};

#endif