#include "NovodexInterface.h"
#include "FirstPersonOrientation.h"

void FirstPersonOrientation::RotateFirstPerson(float XAxis, float YAxis)
{
	static const float Fix = 89.99f;
	if(mClamp + XAxis > Fix) {
		XAxis = Fix - mClamp;
	}
	if(mClamp + XAxis < -Fix) {
		XAxis = -(mClamp + Fix);
	}
	mClamp += XAxis;
	/*
	if(mClamp > Fix || mClamp < -Fix) {
		NxQuat qfix(-fmod(mClamp,Fix),(NxVec3)mSide);
		qfix.rotate(mView);
		mClamp = (mClamp > 0 ? Fix : -Fix);
		CalcSide();
	}*/
	NxQuat qx(XAxis,(NxVec3)mSide);
	qx.rotate(mView);
	NxQuat qy(YAxis,(NxVec3)mUp);
	qy.rotate(mView);
	CalcSide();

} 