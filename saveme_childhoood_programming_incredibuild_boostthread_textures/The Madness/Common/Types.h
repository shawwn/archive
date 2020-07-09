#ifndef TYPES_H
#define TYPES_H

typedef unsigned __int8  Byte;
typedef unsigned __int16 Word;
typedef unsigned __int32 DoubleWord;
typedef unsigned __int64 QuadWord;

#ifndef BYTE
typedef Byte BYTE;
#endif

#ifndef UINT
typedef DoubleWord UINT;
#endif

struct VTEXCOORD2
{
	VTEXCOORD2() : U(0), V(0) { }
	VTEXCOORD2(float sU, float sV) : U(sU), V(sV) { }
	float U;
	float V;
};

struct VPOINT2
{
	VPOINT2() : X(0), Y(0) { }
	VPOINT2(int sX, int sY) : X(sX), Y(sY) { }
	int X;
	int Y;
};

struct VCOLOR4
{
	VCOLOR4() { }
	VCOLOR4(BYTE a, BYTE r, BYTE g, BYTE b) { Color[0] = a; Color[1] = r; Color[2] = g; Color[3] = b; }
	BYTE Color[4];
};

#define NOMINMAX

#endif