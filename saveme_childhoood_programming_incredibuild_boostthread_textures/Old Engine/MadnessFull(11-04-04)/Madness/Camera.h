#ifndef _CAMERA_H
#define _CAMERA_H

#include <d3d9.h>
#include <d3dx9.h>

class Camera
{
private:
	D3DXMATRIX mWorldMatrix;
	D3DXMATRIX mViewMatrix;
	D3DXMATRIX mProjMatrix;
	D3DXMATRIX mOrthoMatrix;

	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mViewPosition;
	D3DXVECTOR3 mvView;
	D3DXVECTOR3 mvUp;
	D3DXVECTOR3 mvSide;

	float mFov;
	float mAspect;
	float mNear;
	float mFar;

	float mOrthoWidth;
	float mOrthoHeight;
	float mOrthoNear;
	float mOrthoFar;
public:
	Camera();
	~Camera();

	void ResetMatrices(LPDIRECT3DDEVICE9 Device);
    void Look(LPDIRECT3DDEVICE9 Device);
	void LookOrtho(LPDIRECT3DDEVICE9 Device);
	void LookAt(LPDIRECT3DDEVICE9 Device, const D3DXVECTOR3 &Position, const D3DXVECTOR3 &vView, const D3DXVECTOR3 &vUp);
	void View(const D3DXVECTOR3 &Position, const D3DXVECTOR3 &vView, const D3DXVECTOR3 &vUp);
	void Proj(const float &Fov, const float &Aspect, const float &Near, const float &Far);
	void Ortho(const float &Width, const float &Height, const float &zNear, const float &zFar);
	void UpdateWorldMatrix();
	void UpdateViewMatrix();
	void UpdateProjMatrix();
	void UpdateOrthoMatrix();
	void ClearMatrices();
	void Clear();
	
	void SetPosition(const D3DXVECTOR3 &Position);
	void SetView(const D3DXVECTOR3 &vView);
	void SetUp(const D3DXVECTOR3 &vUp);
	const D3DXVECTOR3 &GetPosition() { return mPosition; }
	const D3DXVECTOR3 &GetView() { return mvView; }
	const D3DXVECTOR3 &GetUp() { return mvUp; }
	const D3DXVECTOR3 &GetSide() { return mvSide; }
	const D3DXMATRIX &GetWorldMatrix() { return mWorldMatrix; }
	const D3DXMATRIX &GetViewMatrix() { return mViewMatrix; }
	const D3DXMATRIX &GetProjMatrix() { return mProjMatrix; }
	D3DXMATRIX GetViewProjMatrix() { return mViewMatrix * mProjMatrix; }
};

#endif