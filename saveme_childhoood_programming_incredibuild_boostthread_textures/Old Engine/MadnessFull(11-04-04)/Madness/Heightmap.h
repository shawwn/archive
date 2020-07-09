#ifndef _HEIGHTMAP_H
#define _HEIGHTMAP_H

#include "Mesh.h"
#include "ImageData.H"

class Heightmap : public Mesh
{
private:
	ImageData mData;
public:
	bool Load(LPDIRECT3DDEVICE9 Device, const string &Filename, const float &Scale, const float &Height);
};

#endif