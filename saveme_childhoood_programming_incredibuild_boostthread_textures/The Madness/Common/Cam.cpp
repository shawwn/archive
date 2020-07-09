#include "Cam.h"
#include "Direct3DInterface.h"

void Camera::Look(LPDIRECT3DDEVICE9 Device)
{
	HRESULT hr;
	D3DXMatrixLookAtLH(&mMatView,&NV3TODV3(mEye),&NV3TODV3(mEye+mView),&NV3TODV3(mUp));
	D3DXMatrixPerspectiveFovLH(&mMatProj,mFov,mAspect,1.0f,10000.0f);
	if(FAILED(hr = Device->SetTransform(D3DTS_VIEW,&mMatView)))
		LogDxErr(hr,"Camera::Look");
	if(FAILED(hr = Device->SetTransform(D3DTS_PROJECTION,&mMatProj)))
		LogDxErr(hr,"Camera::Look");
	if(FAILED(hr = Device->SetTransform(D3DTS_WORLD,&gMatIdent)))
		LogDxErr(hr,"Camera::Look");
}