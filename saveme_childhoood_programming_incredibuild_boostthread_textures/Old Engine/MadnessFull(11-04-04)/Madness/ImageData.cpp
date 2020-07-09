#include "dxstdafx.h"
#include "LoggingInterface.h"
#include "ImageData.h"

ImageData::ImageData()
{
	mData = NULL;
}

ImageData::~ImageData()
{
	Destroy();
}

bool ImageData::Load(LPDIRECT3DDEVICE9 Device, const string &Filename)
{
	LPDIRECT3DTEXTURE9 replaceme;

	HRESULT hr = D3DXCreateTextureFromFileA(Device,Filename.c_str(),&replaceme);
	if(FAILED(hr))
	{
		return false;
	}


	if(FAILED(replaceme->GetLevelDesc(0,&mSurf)))
	{
		gLog->Stream << "ImageData " << Filename << ": Could not get a surface description.";
		gLog->Print();
		return false;
	}

	if(FAILED(replaceme->LockRect(0,&mRect,NULL,D3DLOCK_READONLY)))
	{
		gLog->Stream << "ImageData " << Filename << ": Could not access data.";
		gLog->Print();
		if(replaceme)
			replaceme->Release();
		return false;
	}
	char *At = (char *)mRect.pBits;
	

	int i, j;
	if(!mData)
	{
		mData = new BYTE **[mSurf.Height];
		for(i = 0; i < (int)mSurf.Height; i++)
		{
			mData[i] = new BYTE *[mSurf.Width];
			for(j = 0; j < (int)mSurf.Width; j++)
				mData[i][j] = new BYTE[4];
		}
	}

	for(i = 0; i < (int)mSurf.Height; i++)
	{
		for(j = 0; j < (int)mSurf.Width; j++)
		{
			memcpy(&mData[i][j][0],At,sizeof(BYTE) * 4);
			At += sizeof(BYTE) * 4;
		}
	}

	replaceme->UnlockRect(0);
	replaceme->Release();
	return true;
}

void ImageData::Destroy()
{
	if(mData)
	{
		int i, j;
		for(i = 0; i < (int)mSurf.Height; i++)
		{
			for( j = 0; j < (int)mSurf.Width; j++)
			{
				delete [] mData[i][j];
			}
			delete [] mData[i];
		}
		delete[] mData;
	}
}