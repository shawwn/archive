#include "dxstdafx.h"
#include "Physics.h"
#include "MathInterface.h"

Physics::Physics()
{
	DestroyPhysics();
}

Physics::~Physics()
{
}

void Physics::CalculateSide()
{
	D3DXVec3Cross(&mSide,&mFacing,&mUp);
}

void Physics::UpdatePhysics()
{
	mPosition += mVelocity * DXUTGetElapsedTime();	
}

void Physics::DestroyPhysics()
{
	mPosition = D3DXVECTOR3(0,20,0);
	mFacing = D3DXVECTOR3(-1,0,0);
	mUp = D3DXVECTOR3(0,1,0);
	mVelocity = D3DXVECTOR3(0,0,0);
	mAcceleration = D3DXVECTOR3(0,0,0);

	D3DXMatrixIdentity(&mMatrix);
}

void Physics::Set(const D3DXVECTOR3 &Position, const D3DXVECTOR3 &Facing, const D3DXVECTOR3 &Up)
{
	mPosition = Position;
	mFacing = Facing;
	mUp = Up;
	CalculateSide();
}

void Physics::SetPosition(const D3DXVECTOR3 &Position)
{
	mPosition = Position;
}
void Physics::SetFacing(const D3DXVECTOR3 &Facing)
{
	mFacing = Facing;
	CalculateSide();
}
void Physics::SetUp(const D3DXVECTOR3 &Up)
{
	mUp = Up;
	CalculateSide();
}

void Physics::SetVelocity(const D3DXVECTOR3 &Velocity)
{
	mVelocity = Velocity;
}

void Physics::ZeroVelocity()
{
	mVelocity = D3DXVECTOR3(0,0,0);
}

void Physics::AddVelocityInstant(const D3DXVECTOR3 &Velocity)
{
	mVelocity += Velocity;
}

void Physics::AddVelocityOverTime(const D3DXVECTOR3 &Velocity)
{
	mVelocity += Velocity * DXUTGetElapsedTime();
}

void Physics::ZeroAcceleration()
{
	mAcceleration = D3DXVECTOR3(0,0,0);
}

void Physics::Rotate(const float &XAngle, const float &YAngle)
{
	SetFacing(gMath->RotatePointAroundAxis(mSide,mFacing,XAngle));
	SetFacing(gMath->RotatePointAroundAxis(mUp,mFacing,YAngle));
	D3DXVec3Cross(&mUp,&mSide,&mFacing);
	D3DXVec3Normalize(&mUp,&mUp);
}

bool Physics::Update()
{
	UpdatePhysics();

	return true;
}