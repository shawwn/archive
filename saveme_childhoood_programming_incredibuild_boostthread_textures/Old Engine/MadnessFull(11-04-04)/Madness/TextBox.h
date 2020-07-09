#ifndef _TEXTBOX_H
#define _TEXTBOX_H

#include <string>
#include <list>
#include "dxutmisc.h"
using namespace std;

class TextBox
{
private:
	RECT mRect;
	wstring mFontName;
	int mFontHeight;
	int mMaxLines;

    std::list<wstring> mBuffer;
	LPD3DXFONT mFont;
	LPD3DXSPRITE mSprite;
	ABC mWidths[256];

	int GetLineWidth(const wstring &str);
	bool RenderLine(CDXUTTextHelper *Text, int &mTotalHeight, const wstring &str);
public:
	TextBox();
	~TextBox();

	typedef std::list<wstring>::iterator Iterator;
	typedef std::list<wstring>::reverse_iterator RIterator;

	Iterator Begin()	{ return mBuffer.begin(); }
	Iterator End()		{ return mBuffer.end(); }
	int LineCount()		{ return (int)mBuffer.size(); }
	int Width()			{ return mRect.right-mRect.left; }
	int Height()		{ return mRect.bottom-mRect.top; }


	void Set(LPDIRECT3DDEVICE9 Device, int X, int Y, int BoxWidth, int BoxHeight, const wstring &FontName, int FontHeight, int MaxLines);
	void Print(const wstring &str);
	void Render(int &StartLine);
	void OnLostDevice();
	void OnResetDevice(LPDIRECT3DDEVICE9 Device);
	void Destroy();

};

#endif