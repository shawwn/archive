#include "dxstdafx.h"
#include "Box.h"

Box::Box()
{

}

Box::~Box()
{

}

const Box &Box::operator =(const Box &B)
{
	mMin = B.mMin;
	mMax = B.mMax;

	return *this;
}

bool Box::operator ==(const Box &B)
{
	if(mMin == B.mMin && mMax == B.mMax)
		return true;
	return false;
}