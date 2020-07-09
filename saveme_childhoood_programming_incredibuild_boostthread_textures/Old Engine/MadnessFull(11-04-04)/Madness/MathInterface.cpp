#include "dxstdafx.h"
#include "MathInterface.h"
#include "LoggingInterface.h"

MathInterface *gMath;

float MathInterface::Distance(const D3DXVECTOR3 &p1, const D3DXVECTOR3 &p2)
{
	return D3DXVec3Length(&D3DXVECTOR3(p2-p1));
}

double MathInterface::AngleBetweenVectors(const D3DXVECTOR3 &Vector1, const D3DXVECTOR3 &Vector2)
{
	float dot = D3DXVec3Dot(&Vector1,&Vector2);
	float mag = D3DXVec3Length(&Vector1) * D3DXVec3Length(&Vector2);
	double angle = acos(dot/mag);
	if(_isnan(angle))
		return 0;

	return angle;
}

D3DXVECTOR3 MathInterface::GetVector(const D3DXVECTOR3 &Point1, const D3DXVECTOR3 &Point2)
{
	return D3DXVECTOR3(Point2-Point1);
}

D3DXVECTOR3 MathInterface::GetNormal(D3DXVECTOR3 Triangle[])
{
	D3DXVECTOR3 Out;
	D3DXVec3Cross(&Out,&D3DXVECTOR3(Triangle[2]-Triangle[0]),&D3DXVECTOR3(Triangle[1]-Triangle[0]));
	D3DXVec3Normalize(&Out,&Out);
	if(D3DXVec3Length(&Out) == 0) {
		gLog->Stream << "MathInterface Warning: Zero normal.  Triangle " << 
			Triangle[0].x << "," << Triangle[0].y << "," << Triangle[0].z << "   " <<
			Triangle[1].x << "," << Triangle[1].y << "," << Triangle[1].z << "   " <<
			Triangle[2].x << "," << Triangle[2].y << "," << Triangle[2].z;
		gLog->Print();		
	}
	return Out;
}

D3DXVECTOR3 MathInterface::RotatePointAroundAxis(const D3DXVECTOR3 &Axis, const D3DXVECTOR3 &Point, const float &Angle)
{
	if(Angle == 0)
		return Point;

	mRotationFinal = D3DXQUATERNION(Point.x,Point.y,Point.z,0);
	D3DXQuaternionRotationAxis(&mRotation,&Axis,-Angle*PIOVER180);
	D3DXQuaternionConjugate(&mRotationConjugate,&mRotation);
	mRotationFinal = mRotation * mRotationFinal * mRotationConjugate;

	return D3DXVECTOR3(mRotationFinal.x,mRotationFinal.y,mRotationFinal.z);
}

D3DXVECTOR3 MathInterface::ClosestPointOnLine(const D3DXVECTOR3 &PointA, const D3DXVECTOR3 &PointB, const D3DXVECTOR3 &ClosestToWhat)
{
	D3DXVECTOR3 Vec1 = ClosestToWhat - PointA;
	D3DXVECTOR3 Vec2 = PointB - PointA;
	D3DXVec3Normalize(&Vec2,&Vec2);
	D3DXVECTOR3 Len = PointA - PointB;
	float d = D3DXVec3Length(&Len);
	float t = D3DXVec3Dot(&Vec2,&Vec1);

	if(t <= 0)
		return PointA;
	if(t >= d)
		return PointB;

	D3DXVECTOR3 Vec3 = Vec2 * t;

	return D3DXVECTOR3(PointA + Vec3);
}

float MathInterface::PlaneDistance(const D3DXVECTOR3 &Normal, const D3DXVECTOR3 &PointOnPlane)
{
	return -((Normal.x * PointOnPlane.x) + (Normal.y * PointOnPlane.y) + (Normal.z * PointOnPlane.z));
}
