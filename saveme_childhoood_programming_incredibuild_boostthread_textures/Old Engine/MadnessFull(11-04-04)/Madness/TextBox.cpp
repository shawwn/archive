#include "dxstdafx.h"
#include "TextBox.h"
#include "LoggingInterface.h"

TextBox::TextBox()
{
	mFont = NULL;
	mSprite = NULL;
	Destroy();
}

TextBox::~TextBox()
{
	Destroy();
}

int TextBox::GetLineWidth(const wstring &str)
{
	int Width = 0;
	/*for(int i = 0; i < (int)str.length(); i++) {
		Width += mWidths[str[i]].abcA;
		Width += mWidths[str[i]].abcB;
		Width += mWidths[str[i]].abcC;
	}*/

	for ( std::wstring::const_iterator i = str.begin(), end = str.end();
              i != end;
              ++i )
        {
                ABC const& kerning = mWidths[*i];
		Width += kerning.abcA;
		Width += kerning.abcB;
		Width += kerning.abcC;
	}
	return Width;
}

bool TextBox::RenderLine(CDXUTTextHelper *Text, int &mTotalHeight, const wstring &str)
{
    wstring Bulk = str;
	wstring Remainder;
	int TotalWidth = GetLineWidth(Bulk);
	while(TotalWidth > Width())
	{
		size_t pos = Bulk.find_last_of(' ');
		if(pos == std::wstring::npos)
		{
			Remainder = Bulk.substr(Bulk.length()-1,Bulk.length()) + Remainder;
			Bulk = Bulk.substr(0,Bulk.length()-1);
		}
		else
		{
			Remainder = Bulk.substr(Bulk.find_last_of(' '),Bulk.length()) + Remainder;
			Bulk = Bulk.substr(0,Bulk.find_last_of(' '));
		}
		TotalWidth = GetLineWidth(Bulk);
	}
	
	if(Remainder.length() > 0) {
		if(!RenderLine(Text,mTotalHeight,Remainder))
			return false;
	}
	
	mTotalHeight += mFontHeight;
	if(mTotalHeight > Height())
		return false;

	Text->SetInsertionPos(mRect.left,mRect.bottom-mTotalHeight);
	Text->DrawTextLine(Bulk.c_str());

	return true;
}

void TextBox::Set(LPDIRECT3DDEVICE9 Device, int X, int Y, int BoxWidth, int BoxHeight, const wstring &FontName, int FontHeight, int MaxLines)
{
	if(!Device)
		return;

	Destroy();

	SetRect(&mRect,X,Y,X+BoxWidth,Y+BoxHeight);
	mFontName = FontName;
	mFontHeight = FontHeight;
	mMaxLines = MaxLines;
}

void TextBox::Print(const wstring &str)
{
	mBuffer.push_back(str);
	if(LineCount() > mMaxLines && mMaxLines > 0)
		mBuffer.pop_front();
}

void TextBox::Render(int &StartLine)
{
	if(!mFont)
		return;
	CDXUTTextHelper Text(mFont,mSprite,mFontHeight);

	Text.Begin();
	Text.SetInsertionPos(mRect.left,mRect.bottom);
	int TotalHeight = 0;
	int TotalWidth = 0;

	wstring Bulk;
	wstring Remainder;
	RIterator it = mBuffer.rbegin();
	if(StartLine > (int)mBuffer.size()-1)
		StartLine = (int)mBuffer.size()-1;
	if(StartLine < 0)
		StartLine = 0;
	for(int i = 0; i < StartLine; i++)
		it++;
	
	for(it; it != mBuffer.rend(); it++)
	{
		if(!RenderLine(&Text,TotalHeight,(*it)))
			break;
	}
	Text.End();
}

void TextBox::OnLostDevice()
{
	if(mSprite)
		mSprite->Release();
	mSprite = NULL;
	
	if(mFont)
		mFont->Release();
	mFont = NULL;
}

void TextBox::OnResetDevice(LPDIRECT3DDEVICE9 Device)
{
	D3DXCreateFont( Device, mFontHeight, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
					OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                    mFontName.c_str(), &mFont );
	if(!mFont)
		return;

	int ret = GetCharABCWidths(mFont->GetDC(),0,255,mWidths);

	D3DXCreateSprite(Device,&mSprite);
}

void TextBox::Destroy()
{
	SetRect(&mRect,0,0,500,200);
	mFontHeight = 15;
	mMaxLines = 1000;
	mFontName = L"Arial";
}