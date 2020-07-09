#ifndef _SHPERE_H
#define _SPHERE_H

class Sphere
{
private:
public:
	Sphere();
	~Sphere();
	Sphere(const D3DXVECTOR3 &Position, const float &Radius) {
		mPosition = Position;
		mRadius = Radius;
	}

	D3DXVECTOR3 mPosition;
	float mRadius;

	const Sphere &operator = (const Sphere &S);
	bool operator ==(const Sphere &S);
};

#endif