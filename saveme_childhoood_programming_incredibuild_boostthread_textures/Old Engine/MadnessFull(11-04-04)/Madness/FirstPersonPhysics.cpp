#include "dxstdafx.h"
#include "FirstPersonPhysics.h"
#include "InputInterface.h"
#include "ConfigurationInterface.h"
#include "MathInterface.h"

FirstPersonPhysics::FirstPersonPhysics()
{
	mRotationX = 0.0f;
	mRotationRestraintX = 89.0f;
	mSpeed = 960;
}

FirstPersonPhysics::~FirstPersonPhysics()
{

}

void FirstPersonPhysics::RotateFirstPerson(const float &XAngle, const float &YAngle)
{
	float NextRot = mRotationX + XAngle;
	if( (NextRot > mRotationRestraintX)) {
		SetFacing(gMath->RotatePointAroundAxis(mSide,mFacing,(mRotationRestraintX - mRotationX)));
		mRotationX = mRotationRestraintX;
	}
	else if(NextRot < -mRotationRestraintX) {
		SetFacing(gMath->RotatePointAroundAxis(mSide,mFacing,-(mRotationX + mRotationRestraintX)));
		mRotationX = -mRotationRestraintX;
	}
	else {
		SetFacing(gMath->RotatePointAroundAxis(mSide,mFacing,XAngle));
		mRotationX += XAngle;
	}
	SetFacing(gMath->RotatePointAroundAxis(mUp,mFacing,YAngle));
}

bool FirstPersonPhysics::Update()
{
	UpdatePhysics();
	mMovementVector = D3DXVECTOR3(0,0,0);
	DecayVelocity(3,40);

	return true;
}

bool FirstPersonPhysics::UpdateWithKeyboard()
{
	if(gInput)
	{
		if(gInput->KeyDown(gConfig->GetKeyMoveForward()))
		{
			mMovementVector += mFacing;
		}
		if(gInput->KeyDown(gConfig->GetKeyMoveBackward()))
		{
			mMovementVector -= mFacing;
		}
		if(gInput->KeyDown(gConfig->GetKeyStrafeRight()))
		{
			mMovementVector -= mSide;
		}
		if(gInput->KeyDown(gConfig->GetKeyStrafeLeft()))
		{
			mMovementVector += mSide;
		}

		if(D3DXVec3Length(&mVelocity) < 160) {
			mVelocity += mMovementVector * mSpeed * DXUTGetElapsedTime();
		}
	}
	return true;
}

void FirstPersonPhysics::DecayVelocity(const float &Factor,const float &StopHarderWhenLessThan)
{
	if(D3DXVec3Length(&mVelocity) < StopHarderWhenLessThan)
		mVelocity -= mVelocity * DXUTGetElapsedTime() * Factor * 2;
	else
		mVelocity -= mVelocity * DXUTGetElapsedTime() * Factor;
}