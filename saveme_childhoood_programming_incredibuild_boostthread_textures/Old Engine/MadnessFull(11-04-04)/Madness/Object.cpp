#include "dxstdafx.h"
#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
	DestroyObject();
}

void Object::DestroyObject()
{
	mName = "";
	mGUID = -1;
}

bool Object::Render(LPDIRECT3DDEVICE9 Device)
{
	return true;
}