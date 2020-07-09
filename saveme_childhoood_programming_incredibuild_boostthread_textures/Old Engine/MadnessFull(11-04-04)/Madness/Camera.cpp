#include "dxstdafx.h"
#include "Camera.h"
#include "MathInterface.h"
#include "InputInterface.h"

Camera::Camera()
{
	Clear();
}

Camera::~Camera()
{
}

void Camera::ResetMatrices(LPDIRECT3DDEVICE9 Device)
{
	if(!Device)
		return;
	D3DXMATRIX Identity;
	D3DXMatrixIdentity(&Identity);

	Device->SetTransform(D3DTS_WORLD,&Identity);
	Device->SetTransform(D3DTS_VIEW,&Identity);
	Device->SetTransform(D3DTS_PROJECTION,&Identity);
}

void Camera::Look(LPDIRECT3DDEVICE9 Device)
{
	if(!Device)
		return;
	//Device->SetTransform(D3DTS_WORLD,&mWorldMatrix);
	Device->SetTransform(D3DTS_VIEW,&mViewMatrix);
	Device->SetTransform(D3DTS_PROJECTION,&mProjMatrix);
}

void Camera::LookOrtho(LPDIRECT3DDEVICE9 Device)
{
	if(!Device)
		return;

	ResetMatrices(Device);
	Device->SetTransform(D3DTS_PROJECTION,&mOrthoMatrix);
}

void Camera::LookAt(LPDIRECT3DDEVICE9 Device, const D3DXVECTOR3 &Position, const D3DXVECTOR3 &vView, const D3DXVECTOR3 &vUp)
{
	if(!Device)
		return;
	SetPosition(Position);
	SetView(vView);
	SetUp(vUp);
	Look(Device);
}

void Camera::View(const D3DXVECTOR3 &Position, const D3DXVECTOR3 &vView, const D3DXVECTOR3 &vUp)
{
	mPosition = Position;
	D3DXVec3Normalize(&mvView,&vView);
	mViewPosition = mPosition + mvView;
	D3DXVec3Normalize(&mvUp,&vUp);
	D3DXVec3Cross(&mvSide,&mvView,&mvUp);

	UpdateWorldMatrix();
	UpdateViewMatrix();
}

void Camera::Proj(const float &Fov, const float &Aspect, const float &Near, const float &Far)
{
	mFov = Fov*PIOVER180;
	mAspect = Aspect;
	mNear = Near;
	mFar = Far;

	if(mFov <= 0)
		mFov = 60.0f*PIOVER180;
	if(mAspect <= 0)
		mAspect = 1600.0f/1200.0f;
	if(mNear <= 0)
		mNear = 1.0f;

	UpdateProjMatrix();
}

void Camera::Ortho(const float &Width, const float &Height, const float &zNear, const float &zFar)
{
	mOrthoWidth = Width;
	mOrthoHeight = Height;
	mOrthoNear = zNear;
	mOrthoFar = zFar;

	UpdateOrthoMatrix();
}

void Camera::SetPosition(const D3DXVECTOR3 &Position)
{
	mPosition = Position;
	UpdateViewMatrix();
}

void Camera::SetView(const D3DXVECTOR3 &vView)
{
	D3DXVec3Normalize(&mvView,&vView);
	mViewPosition = mPosition + mvView;
	D3DXVec3Cross(&mvSide,&mvView,&mvUp);
	UpdateViewMatrix();
}

void Camera::SetUp(const D3DXVECTOR3 &vUp)
{
	D3DXVec3Normalize(&mvUp,&vUp);
	D3DXVec3Cross(&mvSide,&mvView,&mvUp);
	UpdateViewMatrix();
}

void Camera::UpdateWorldMatrix()
{
	D3DXMatrixIdentity(&mWorldMatrix);
	D3DXMatrixTranslation(&mWorldMatrix,mPosition.x,mPosition.y,mPosition.z);
}

void Camera::UpdateViewMatrix()
{
	D3DXMatrixLookAtLH(&mViewMatrix,&mPosition,&mViewPosition,&mvUp);
}

void Camera::UpdateProjMatrix()
{
	D3DXMatrixPerspectiveFovLH(&mProjMatrix,mFov,mAspect,mNear,mFar);
}

void Camera::UpdateOrthoMatrix()
{
	D3DXMatrixOrthoLH(&mOrthoMatrix,mOrthoWidth,mOrthoHeight,mOrthoNear,mOrthoFar);
}

void Camera::Clear()
{
	D3DXMatrixIdentity(&mWorldMatrix);
	D3DXMatrixIdentity(&mViewMatrix);
	D3DXMatrixIdentity(&mProjMatrix);
	D3DXMatrixIdentity(&mOrthoMatrix);
	mPosition = D3DXVECTOR3();
	mvView = D3DXVECTOR3();
	mvUp = D3DXVECTOR3();
	mvSide = D3DXVECTOR3();
	
	mFov = 10.0f;
	mAspect = 1600.0f/1200.0f;
	mNear = 1.0f;
	mFar = 10000.0f;

	mOrthoWidth = 1.0f;
	mOrthoHeight = 1.0f;
	mOrthoNear = 1.0f;
	mOrthoFar = 2.0f;
}