#ifndef _MATHINTERFACE_H
#define _MATHINTERFACE_H

#define PI 3.141592653f
#define PIOVER180 0.0174532925166f
#define _180OVERPI 57.2957795238388f



class MathInterface
{
private:
	D3DXQUATERNION mRotation;
	D3DXQUATERNION mRotationConjugate;
	D3DXQUATERNION mRotationFinal;
public:
	void	Clamp(float &Num, const float &Min, const float &Max)	{ if(Num < Min) { Num = Min; } if(Num > Max) { Num = Max; } }
	void	Clamp(int &Num, const int &Min, const int &Max)			{ if(Num < Min) { Num = Min; } if(Num > Max) { Num = Max; } }

	float		Distance(const D3DXVECTOR3 &p1, const D3DXVECTOR3 &p2);
	double		AngleBetweenVectors(const D3DXVECTOR3 &Vector1, const D3DXVECTOR3 &Vector2);
	
	D3DXVECTOR3 GetVector(const D3DXVECTOR3 &Point1, const D3DXVECTOR3 &Point2);
	D3DXVECTOR3 GetNormal(D3DXVECTOR3 Triangle[]);
	D3DXVECTOR3 RotatePointAroundAxis(const D3DXVECTOR3 &Axis, const D3DXVECTOR3 &Point, const float &Angle);
	D3DXVECTOR3 ClosestPointOnLine(const D3DXVECTOR3 &PointA, const D3DXVECTOR3 &PointB, const D3DXVECTOR3 &ClosestToWhat);

	float PlaneDistance(const D3DXVECTOR3 &Normal, const D3DXVECTOR3 &PointOnPlane);
	/*float DistanceFromPolygon(MeshVertex Poly[], const D3DXVECTOR3 &Point);
	bool IntersectedPlane(MeshVertex Poly[], const int &VertexCount, D3DXVECTOR3 *Line, D3DXVECTOR3 &Normal, float &OriginDistance);
	D3DXVECTOR3 IntersectionPoint(const D3DXVECTOR3 &Normal, D3DXVECTOR3 *Line, const double &Distance);
	bool InsidePolygon(const D3DXVECTOR3 &Intersection, MeshVertex Poly[], const long &VertexCount);
    bool IntersectedPolygon(MeshVertex Poly[], D3DXVECTOR3 *Line, const int &VertexCount, D3DXVECTOR3 *IntersectedWhere);*/
};
extern MathInterface *gMath;

#endif