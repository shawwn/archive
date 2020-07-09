#define NOMINMAX
#include "ServerInterface.h"
#include "ServerViewer.h"
#include "InputInterface.h"
#include "LoggingInterface.h"
#include "Direct3DInterface.h"
#include "TimeInterface.h"

bool ServerViewer::Initialize(LPNXSCENE mScene)
{
	if(!mScene)
		return false;

	mLight = gLights.AddLight(D3DXVECTOR3(0,0,-30),0.5f);
	mObject.CreateDefaultActor(mScene);
	//mObject.CreateNetworkActor(mScene);

	mObject.GetActor()->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
	mObject.GetActor()->setGlobalPosition(mOrientation.GetEye());

	return true;
}

bool ServerViewer::Update(float fDeltaTimeInSeconds)
{
	if(!mObject.GetActor()) {
		return false;
	}

	if(mLight) mLight->Position = NV3TODV3(mObject.GetActor()->getGlobalPosition());

	mOrientation.RotateFirstPerson(-gInput.GetMouse().GetDiffY() * 0.01745f,gInput.GetMouse().GetDiffX() * 0.01745f);	

	mOrientation.SetEye(mObject.GetActor()->getGlobalPosition());	

	NxVec3 Force = NxVec3(0,0,0);
	static NxVec3 PrevForce = NxVec3(0,0,0);

	float Speed = 50;
	if(gInput.GetKeyboard().KeyDown(DIK_W)) {
		Force += mOrientation.GetView()*Speed;
	}
	if(gInput.GetKeyboard().KeyDown(DIK_S)) {
		Force -= mOrientation.GetView()*Speed;
	}
	if(gInput.GetKeyboard().KeyDown(DIK_A)) {
		Force += mOrientation.GetSide()*Speed;
	}
	if(gInput.GetKeyboard().KeyDown(DIK_D)) {
		Force -= mOrientation.GetSide()*Speed;
	}

	if(gInput.GetKeyboard().KeyDown(DIK_F9)) {
		gD3D.SaveScreenshot("Screen.bmp");
	}		

	if(gInput.GetKeyboard().KeyDown(DIK_ESCAPE)) {
		gServer.Stop();
	}
	/*
	gLog << "\n";
	gLog << "Befor Acc[" << Force.x << "," << Force.y << "," << Force.z << "]\n";
	gLog << "Befor Vel[" << mObject.GetActor()->getLinearVelocity().x << "," << mObject.GetActor()->getLinearVelocity().y << "," << mObject.GetActor()->getLinearVelocity().z << "]\n";

	*/


	mObject.GetActor()->addForce(Force * gTime.GetDeltaTimeInSeconds(),NX_VELOCITY_CHANGE);	
	//mObject.SetAcceleration(Force);

	return true;
}