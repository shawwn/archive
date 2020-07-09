#ifndef VERTEX_H
#define VERTEX_H

#include <d3dx9.h>
#include <vector>
#include "Types.h"

class Vertex
{
private:
	LPDIRECT3DVERTEXDECLARATION9 Declaration;
public:
	Vertex(int NumPos, int NumColor, int NumCoords) { int i; 
	for(i = 0; i < NumPos; i++) { PositionData.push_back(D3DXVECTOR3(0,0,0)); }
	for(i = 0; i < NumColor; i++) { ColorData.push_back(VCOLOR4(0,0,0,0)); }
	for(i = 0; i < NumCoords; i++) { TextureCoordinates.push_back(VTEXCOORD2(0,0)); } }
	Vertex() : Declaration(NULL) { }
	~Vertex() { if(Declaration) { Declaration->Release(); } Declaration = NULL; }
	std::vector<D3DXVECTOR3> PositionData;
	std::vector<VCOLOR4> ColorData;
	std::vector<VTEXCOORD2> TextureCoordinates;

	inline int PositionDataCount() { return (int)PositionData.size(); }
	inline int ColorDataCount() { return (int)ColorData.size(); }
	inline int TextureCoordinateCount() { return (int)TextureCoordinates.size(); }
	inline int Size() { return sizeof(D3DXVECTOR3)*PositionDataCount()+sizeof(VCOLOR4)*ColorDataCount()+sizeof(VTEXCOORD2)*TextureCoordinateCount(); }
};

#endif