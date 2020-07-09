#ifndef CAMERA_H
#define CAMERA_H

#include "Orientation.h"

class Camera : public Orientation
{
private:
	D3DXMATRIX mMatView;
	D3DXMATRIX mMatProj;
	float mFov;
	float mAspect;
public:
	Camera() : mFov(45.0f), mAspect(1600.0f/1200.0f) { }
	void Look(LPDIRECT3DDEVICE9 Device);

	inline const D3DXMATRIX &GetMatView() { return mMatView; }
	inline const D3DXMATRIX &GetMatProj() { return mMatProj; }
	
};

#endif