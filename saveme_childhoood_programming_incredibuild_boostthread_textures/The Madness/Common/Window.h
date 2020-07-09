#ifndef WINDOW_H
#define WINDOW_H

#define NOMINMAX
#include <windows.h>
#include <string>

class Window
{
private:
	int mX;
	int mY;
	int mWidth;
	int mHeight;
	HWND mhWnd;
	std::string mName;
public:
	Window() : mX(0), mY(0), mWidth(640), mHeight(480) { }
	~Window();

	bool Make(const std::string &Name, int Width, int Height, bool Windowed = false, int X = 0, int Y = 0);

	inline HWND GethWnd() { return mhWnd; }
};

#endif