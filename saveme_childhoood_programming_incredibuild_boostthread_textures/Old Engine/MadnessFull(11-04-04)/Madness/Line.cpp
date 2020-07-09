#include "dxstdafx.h"
#include "Line.h"

Line::Line()
{

}

Line::~Line()
{

}

const Line &Line::operator =(const Line &L)
{
	mP1 = L.mP1;
	mP2 = L.mP2;

	return *this;
}

bool Line::operator ==(const Line &L)
{
	if(mP1 == L.mP1 && mP2 == L.mP2)
		return true;
	return false;
}