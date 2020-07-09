#define NOMINMAX
#include "NetworkInterface.h"
#include "CliPlayer.h"
#include "InputInterface.h"
#include "LoggingInterface.h"
#include "Direct3DInterface.h"
#include "TimeInterface.h"
#include "ClientSettingsInterface.h"
#include "PlayerInterface.h"
#include "NetworkInterface.h"

NxVec3 CliPlayer::GetEye()
{
	Player *ply = gPlayers.Find(gNetwork.GetGUID());
	if(!ply)
		return NxVec3();
	return ply->GetPosition();
}

NxVec3 CliPlayer::GetVelocity()
{
	Player *ply = gPlayers.Find(gNetwork.GetGUID());
	if(!ply)
		return NxVec3();
	return ply->GetVelocity();
}

void CliPlayer::SetEye(const NxVec3 &Eye)
{
	Player *ply = gPlayers.Find(gNetwork.GetGUID());
	if(!ply)
		return;
	ply->SetPosition(Eye);
}

void CliPlayer::SetVelocity(const NxVec3 &Vel)
{
	Player *ply = gPlayers.Find(gNetwork.GetGUID());
	if(!ply)
		return;
	ply->SetVelocity(Vel);
}

bool CliPlayer::Initialize(LPNXSCENE mScene)
{
	if(!mScene)
		return false;

	mLight = gLights.AddLight(D3DXVECTOR3(0,0,-30),0.5f);

	return true;
}

bool CliPlayer::Update(float fDeltaTimeInSeconds)
{


	Object *obj = (Object*)gPlayers.Find(gNetwork.GetGUID());
	if(!obj)
		return false;
	if(mLight) mLight->Position = NV3TODV3(obj->GetPosition());

	static NxVec3 PrevView = NxVec3(0,0,1);

	//NxQuat q = mObject.GetActor()->getGlobalOrientationQuat();
	mOrientation.RotateFirstPerson(-gInput.GetMouse().GetDiffY() * 0.01745f,gInput.GetMouse().GetDiffX() * 0.01745f);	
	if(1 - abs(PrevView.dot(mOrientation.GetView())) > 0.01f) {
		Packet viewpack(CMSG_ADJUSTVIEW);
		viewpack << mOrientation.GetView();
		gNetwork.SendPacket(&viewpack);
		PrevView = mOrientation.GetView();
	}

//	NxQuat XRot(-gInput.GetMouse().GetDiffY(),mOrientation.GetSide());
//	NxQuat YRot(gInput.GetMouse().GetDiffX(),mOrientation.GetUp());

	//mObject.GetActor()->setGlobalOrientationQuat(q * XRot * YRot);

	mOrientation.SetEye(obj->GetPosition());	

	NxVec3 Force = NxVec3(0,0,0);
	if(gInput.GetKeyboard().KeyDown(DIK_W)) {
		Force += mOrientation.GetView();
	}
	if(gInput.GetKeyboard().KeyDown(DIK_S)) {
		Force -= mOrientation.GetView();
	}
	if(gInput.GetKeyboard().KeyDown(DIK_A)) {
		Force += mOrientation.GetSide();
	}
	if(gInput.GetKeyboard().KeyDown(DIK_D)) {
		Force -= mOrientation.GetSide();
	}

	if(gInput.GetKeyboard().KeyDown(DIK_F9)) {
		gD3D.SaveScreenshot("Screen.bmp");
	}		

	if(gInput.GetKeyboard().KeyDownOnce(DIK_ESCAPE))
		gSettings.Done = true;

	if(gInput.GetMouse().ButtonDownOnce(MOUSE_LEFT))
	{
		Packet pack(CMSG_OBJECT_CREATE);
		NxVec3 CreateVel = mOrientation.GetView() * 50.0f;
		pack << (Word)1 << CreateVel;
		gNetwork.SendPacket(&pack);
	}

	if(gInput.GetMouse().ButtonDownOnce(MOUSE_RIGHT)) {
		Packet pack(CMSG_OBJECT_CLEARALL);
		gNetwork.SendPacket(&pack);
	}


	static NxVec3 PrevForce = NxVec3(0,0,0);

	if(PrevForce != Force) {
		Packet pack(CMSG_PLAYER_INPUT);
		pack << Force;
		PrevForce = Force;
		gNetwork.SendPacket(&pack);
	}
	/*
	gLog << "\n";
	gLog << "Befor Acc[" << Force.x << "," << Force.y << "," << Force.z << "]\n";
	gLog << "Befor Vel[" << mObject.GetActor()->getLinearVelocity().x << "," << mObject.GetActor()->getLinearVelocity().y << "," << mObject.GetActor()->getLinearVelocity().z << "]\n";

	*/

	//mObject.GetActor()->addForce(Force * gTime.GetDeltaTimeInSeconds(),NX_VELOCITY_CHANGE);

	/*
	gLog << "After Acc[" << Force.x << "," << Force.y << "," << Force.z << "]\n";
	gLog << "After Vel[" << mObject.GetActor()->getLinearVelocity().x << "," << mObject.GetActor()->getLinearVelocity().y << "," << mObject.GetActor()->getLinearVelocity().z << "]\n";
	gLog << "Time [" << fDeltaTimeInSeconds << "]\n";
	*/
	

	return true;
}