#ifndef GUITYPES_H 
#define GUITYPES_H

#include "Macros.h"
#define NOMINMAX
#include <windows.h>
#include <string>
#include <vector>

class GUIElement
{
protected:
	UINT mX, mY;
	UINT mWidth, mHeight;
public:
	GUIElement(int X, int Y, int Width, int Height) : mX(X), mY(Y), mWidth(Width), mHeight(Height) { }
	virtual void OnClick() { }
	virtual void OnMouseOver() { }
	virtual void OnMouseLeave() { }
	virtual void OnKeyboardEvent(char Key) { }
};

class GUIButton : public GUIElement
{
private:
public:
	void OnClick() { MessageBox(NULL,"omg","wtf",MB_OK); }
};

class GUITextBox : public GUIElement
{
private:
	std::string mBuffer;
public:
};

class GUIWindow
{
private:
	std::vector<GUIElement *> mElements;
	std::string mName;
	UINT mX, mY;
	UINT mWidth, mHeight;
	UINT mGUID;
	UINT mHandle;
public:
	GUIWindow(const std::string &Name, UINT Width, UINT Height, UINT X, UINT Y, UINT Handle) : mName(Name), mWidth(Width), mHeight(Height), mX(X), mY(Y), mGUID(0), mHandle(Handle) { }
	~GUIWindow() { for(UINT i = 0; i < ElemCount(); i++) { SAFE_DELETE(mElements[i]); } mElements.clear(); }

	bool AddButton(UINT X, UINT Y, UINT Width, UINT Height, const std::string &Caption);
	bool AddTextBox(UINT X, UINT Y, UINT Width, UINT Height, const std::string &DefaultText = "");

	inline UINT ElemCount() { return (UINT)mElements.size(); }
	inline UINT GetXOffset() { return mX; }
	inline UINT GetYOffset() { return mY; }
	inline UINT GetWidth() { return mWidth; }
	inline UINT GetHeight() { return mHeight; }
};

#endif