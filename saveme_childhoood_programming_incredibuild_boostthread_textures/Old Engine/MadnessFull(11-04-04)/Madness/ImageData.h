#ifndef _IMAGEDATA_H
#define _IMAGEDATA_H

#include <d3d9.h>
#include <d3dx9.h>

class ImageData
{
public:
	ImageData();
	~ImageData();

	D3DLOCKED_RECT mRect;
	D3DSURFACE_DESC mSurf;
	BYTE ***mData;

	bool Load(LPDIRECT3DDEVICE9 Device, const string &Filename);
	void Destroy();

	int Width() { return (int)mSurf.Width; }
	int Height() { return (int)mSurf.Height; }
};

#endif