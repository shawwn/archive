#ifndef _FIRSTPERSONPHYSICS_H
#define _FIRSTPERSONPHYSICS_H

#include "Physics.h"

class FirstPersonPhysics : public Physics
{
private:
	float mRotationX;
	float mRotationRestraintX;
	float mSpeed;
	D3DXVECTOR3 mMovementVector;

	void DecayVelocity(const float &Factor,const float &StopHarderWhenLessThan);
public:
	FirstPersonPhysics();
	~FirstPersonPhysics();

	void RotateFirstPerson(const float &XAngle, const float &YAngle);

	bool Update();
	bool UpdateWithKeyboard();
};

#endif