#ifndef _LINE_H
#define _LINE_H

#include <d3dx9.h>

class Line
{
public:
	Line();
	~Line();
	Line(const D3DXVECTOR3 &P1, const D3DXVECTOR3 &P2) {
		mP1 = P1;
		mP2 = P2;
	}

	D3DXVECTOR3 mP1;
	D3DXVECTOR3 mP2;

	const Line &operator =(const Line &L);
	bool operator ==(const Line &L);
};

#endif