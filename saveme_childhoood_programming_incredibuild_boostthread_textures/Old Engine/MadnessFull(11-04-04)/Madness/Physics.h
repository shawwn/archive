#ifndef _APHYS_H
#define _APHYS_H

#include <d3dx9.h>

class Physics
{
protected:
	D3DXVECTOR3 mPosition;
	D3DXVECTOR3 mFacing;
	D3DXVECTOR3 mUp;
	D3DXVECTOR3 mSide;

	D3DXVECTOR3 mVelocity;
	D3DXVECTOR3 mAcceleration; // Gravity?

    D3DXMATRIX mMatrix;

	void CalculateSide();
	void UpdatePhysics();
	void DestroyPhysics();
public:
	Physics();
	virtual ~Physics();

	void Set(const D3DXVECTOR3 &Position, const D3DXVECTOR3 &Facing, const D3DXVECTOR3 &Up);
	void SetPosition(const D3DXVECTOR3 &Position);
	void SetFacing(const D3DXVECTOR3 &Facing);
	void SetUp(const D3DXVECTOR3 &Up);

	void SetVelocity(const D3DXVECTOR3 &Velocity);
	void ZeroVelocity();
	void AddVelocityInstant(const D3DXVECTOR3 &Velocity);
	void AddVelocityOverTime(const D3DXVECTOR3 &Velocity);

	void ZeroAcceleration();

	void Rotate(const float &XAngle, const float &YAngle);

	virtual bool Update();
	
	const D3DXVECTOR3 &GetPosition()	{ return mPosition; }
	const D3DXVECTOR3 &GetFacing()		{ return mFacing; }
	const D3DXVECTOR3 &GetUp()			{ return mUp; }
	const D3DXVECTOR3 &GetSide()		{ return mSide; }
	const D3DXVECTOR3 &GetVelocity()	{ return mVelocity; }
};

#endif