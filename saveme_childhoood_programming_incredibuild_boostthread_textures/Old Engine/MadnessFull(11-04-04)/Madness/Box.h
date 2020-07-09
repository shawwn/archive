#ifndef _BOX_H
#define _BOX_H

class Box
{
public:
	Box();
	~Box();
	Box(const D3DXVECTOR3 &Min, const D3DXVECTOR3 &Max) {
		mMin = Min;
		mMax = Max;
	}

	D3DXVECTOR3 mMin;
	D3DXVECTOR3 mMax;

	void Make(const D3DXVECTOR3 &Min, const D3DXVECTOR3 &Max);

	const Box &operator =(const Box &B);
	bool operator ==(const Box &B);	
};

#endif