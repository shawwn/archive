#ifndef ORIENTATION_H
#define ORIENTATION_H 

#include "NovodexInterface.h"
#include <d3dx9.h>

class Orientation
{
protected:
	NxVec3 mEye;
	NxVec3 mView;
	NxVec3 mUp;
	NxVec3 mSide;
    
	inline void CalcSide() { mSide.cross(mView,mUp); mSide.normalize(); }

public:
	inline void Set(const NxVec3 &Eye, const NxVec3 &View, const NxVec3 &Up) { mEye = Eye; mView = View; mUp = Up; mView.normalize(); mUp.normalize(); CalcSide(); }
	inline void SetEye(const NxVec3 &Eye) { mEye = Eye; }
	inline void SetView(const NxVec3 &View) { mView = View; mView.normalize(); CalcSide(); }
	inline void SetUp(const NxVec3 &Up)  { mUp = Up; mUp.normalize(); CalcSide(); }

	inline NxVec3 GetEye() { return mEye; }
	inline NxVec3 GetView() { return mView; }
	inline NxVec3 GetUp() { return mUp; }
	inline NxVec3 GetSide() { return mSide; }
};

#endif