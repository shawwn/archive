#ifndef _BITSET_H
#define _BITSET_H

class Bitset
{
private:
	unsigned int *mBits;
	int mSize;
public:
	Bitset() : mBits(0), mSize(0)
	{ }
	~Bitset()
	{
		if(mBits)
		{
			delete mBits;
			mBits = NULL;
		}
	}

	void Resize(int count)
	{
		mSize = count/32 + 1;
		if(mBits)
		{
			delete mBits;
			mBits = 0;
		}
		mBits = new unsigned int[mSize];
		ClearAll();
	}

	void Set(int i)
	{
		mBits[i >> 5] |= (1 << (i & 31));
	}

	int On(int i)
	{
		return mBits[i >> 5] & (1 << (i & 31));
	}

	void Clear(int i)
	{
		mBits[i >> 5] &= ~(1 << (i & 31));
	}

	void ClearAll()
	{
		memset(mBits, 0, sizeof(unsigned int) * mSize);
	}
};

#endif