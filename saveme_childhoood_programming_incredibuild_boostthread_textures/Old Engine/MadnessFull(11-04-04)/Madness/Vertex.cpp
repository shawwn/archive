#include "dxstdafx.h"
#include "Vertex.h"

bool Vertex::CalculateNormal(const D3DXVECTOR3 &V1, const D3DXVECTOR3 &V2, const D3DXVECTOR3 &V3)
{
	D3DXVec3Cross(&Normal,&(V3-V1),&(V2-V1));
	D3DXVec3Normalize(&Normal,&Normal);
	if(Normal.x==0 && Normal.y==0 && Normal.z==0)
		return false; 
	return true;
}

void Vertex::AddCoord(const TexCoord &T)
{
	Coord.push_back(T);
}

DWORD Vertex::FVF()
{
	switch(CoordCount())
	{
	case 1:
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	case 2:
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2;
	case 3:
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX3;
	case 4:
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX4;
	default:
		return D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
	}
}

bool Vertex::operator ==(const Vertex &V) { 
/*	if( (Position == V.Position) &&
		(TextureCoord == V.TextureCoord) &&
		(LightmapCoord == V.LightmapCoord) &&
		(Normal == V.Normal) && 
		(Color[0] == V.Color[0]) && (Color[1] == V.Color[1]) && (Color[2] == V.Color[2]) && (Color[3] == V.Color[3]))
	{
		return true;
	}*/

	if(Position != V.Position)	{ return false; }
	if(Normal != V.Normal)		{ return false; }
	for(int i = 0; i < CoordCount(); i++) { if(!(Coord[i] == V.Coord[i])) { return false; } }
	if(Color[0] != V.Color[0] || Color[1] != V.Color[1] || Color[2] != V.Color[2] || Color[3] != V.Color[3]) { return false; }

	return true;
}

const Vertex &Vertex::operator =(const Vertex &V)
{
	Position = V.Position;
	Normal = V.Normal;
	Coord.clear();
	for(int i = 0; i < (int)V.Coord.size(); i++)
		Coord.push_back(V.Coord[i]);
    memcpy(Color,V.Color,sizeof(BYTE)*4);

	return *this;
}

bool IndexedFace::CalculateNormal(const D3DXVECTOR3 &V1, const D3DXVECTOR3 &V2, const D3DXVECTOR3 &V3)
{
	D3DXVec3Cross(&Normal,&(V3-V1),&(V2-V1));
	D3DXVec3Normalize(&Normal,&Normal);
	if(Normal.x==0 && Normal.y==0 && Normal.z==0)
		return false; 
	return true;
}

void IndexedFace::AddCoordID(const Word &ID)
{
	CoordID.push_back(ID);
}

bool IndexedFace::operator ==(const IndexedFace &F)
{
	for(int i = 0; i < (int)F.CoordID.size(); i++)
	{
		if(CoordID[i] != F.CoordID[i])
			return false;
	}

	if(Normal != F.Normal)
		return false;

	for(i = 0; i < 3; i++) {
		if(Indices[i] != F.Indices[i])
			return false;
	}

	return true;
}

const IndexedFace &IndexedFace::operator = (const IndexedFace &F)
{
	CoordID.clear();
	for(int i = 0; i < (int)F.CoordID.size(); i++)
	{
		CoordID.push_back(F.CoordID[i]);
	}
	Normal = F.Normal;
	memcpy(Indices,F.Indices,sizeof(WORD)*3);

	return *this;
}