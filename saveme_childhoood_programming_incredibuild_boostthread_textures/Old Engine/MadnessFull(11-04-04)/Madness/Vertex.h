#ifndef _VERTEX_H
#define _VERTEX_H

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
using namespace std;
#include "Types.h"

class TexCoord
{
public:
	TexCoord() : u(0), v(0)
	{ }
	float u;
	float v;

	TexCoord operator +(const TexCoord &t) { TexCoord r; r.u=t.u+u; r.v=t.v+v; return r; }
	TexCoord operator -(const TexCoord &t) { TexCoord r; r.u=t.u-u; r.v=t.v-v; return r; }

	TexCoord operator +(const float &f) { TexCoord r; r.u=f+u; r.v=f+v; return r; }
	TexCoord operator -(const float &f) { TexCoord r; r.u=f-u; r.v=f-v; return r; }

	const TexCoord &operator =(const TexCoord &t) { u = t.u; v = t.v; return *this; }

	bool operator == (const TexCoord &T) { if(u == T.u && v == T.v) { return true; } return false; }
};

class Vertex
{
public:
	Vertex() { Position = D3DXVECTOR3(0,0,0); Normal = D3DXVECTOR3(0,0,0); memset((void *)&Color, 0, 4); }

	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	std::vector<TexCoord> Coord;
	BYTE Color[4];

	bool	CalculateNormal(const D3DXVECTOR3 &V1, const D3DXVECTOR3 &V2, const D3DXVECTOR3 &V3);

	void	AddCoord(const TexCoord &T);
	int		CoordCount() { return (int)Coord.size(); }
	DWORD	FVF();

	bool			operator ==(const Vertex &V);
	const Vertex &	operator = (const Vertex &V);
};

class IndexedFace
{
public:
	IndexedFace() { memset(Indices,0,sizeof(WORD)*3); }
	
	std::vector<Word> CoordID;
	WORD Indices[3];
	D3DXVECTOR3 Normal;

	bool CalculateNormal(const D3DXVECTOR3 &V1, const D3DXVECTOR3 &V2, const D3DXVECTOR3 &V3);

	void	AddCoordID(const Word &ID);
	int		CoordIDCount() { return (int)CoordID.size(); }
	int		Min() { int min = Indices[0]; if(Indices[1] < min) { min = Indices[1]; } if(Indices[2] < min) { min = Indices[2]; } return min; }
	int		Max() { int max = Indices[0]; if(Indices[1] > max) { max = Indices[1]; } if(Indices[2] > max) { max = Indices[2]; } return max; }

	bool				operator ==(const IndexedFace &F);
	const IndexedFace  &operator = (const IndexedFace &F);
};

#endif