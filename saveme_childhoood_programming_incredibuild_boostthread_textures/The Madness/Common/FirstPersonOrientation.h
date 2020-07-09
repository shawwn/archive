#ifndef FIRSTPERSONORIENTATION_H
#define FIRSTPERSONORIENTATION_H

#include "Orientation.h"

class FirstPersonOrientation : public Orientation
{
private:
	float mClamp;
public:
	FirstPersonOrientation() : mClamp(0.0f) { }
	void RotateFirstPerson(float XAxis, float YAxis);
};

#endif 