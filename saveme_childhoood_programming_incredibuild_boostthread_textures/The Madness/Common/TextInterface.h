#ifndef TEXTINTERFACE_H
#define TEXTINTERFACE_H

#include <string>
#include <d3dx9.h>
#include <vector>
#include "Macros.h"

class TextObject
{
public:
	TextObject() : X(0), Y(0) { }
	int X;
	int Y;
	std::string Text;
};

class TextInterface
{
private:
	LPD3DXFONT mFont;
	std::vector<TextObject> mText;
public:
	TextInterface() : mFont(NULL) { }
	~TextInterface() { SAFE_RELEASE(mFont); }

	bool Initialize(LPDIRECT3DDEVICE9 Device);
	void Draw(UINT X, UINT Y, const std::string &str);

	bool Render(LPDIRECT3DDEVICE9 Device);
};

extern TextInterface gText;

#endif