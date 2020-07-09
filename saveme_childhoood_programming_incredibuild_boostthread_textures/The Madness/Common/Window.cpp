
#include "Window.h"
#include "ClientSettingsInterface.h"
#include "LoggingInterface.h"
#include "WndProc.h"
using namespace std;

Window::~Window()
{
	UnregisterClass(mName.c_str(),GetModuleHandle(NULL));
	gSettings.hWnd = NULL;
}

bool Window::Make(const string &Name, int Width, int Height, bool Windowed, int X, int Y)
{
	mName = Name;
	mWidth = Width;
	mHeight = Height;
	mX = X;
	mY = Y;

	WNDCLASS wc;
	ZeroMemory(&wc,sizeof(WNDCLASS));
	wc.hCursor = LoadCursor(gSettings.hInstance,IDC_ARROW);
	wc.hIcon = LoadIcon(gSettings.hInstance,IDI_APPLICATION);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = mName.c_str();
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.hInstance = gSettings.hInstance;

	if(!RegisterClass(&wc)) {
		gLog ^ "WARNING: RegisterClass() failed.\n";
		return false;
	}

	mhWnd = CreateWindowEx(
						(Windowed ? WS_EX_APPWINDOW | WS_EX_WINDOWEDGE : WS_EX_APPWINDOW),
						mName.c_str(),
						mName.c_str(),
						(Windowed ? WS_OVERLAPPEDWINDOW : WS_POPUP),
						mX,
						mY,
						mWidth,
						mHeight,
						NULL,
						NULL,
						gSettings.hInstance,
						NULL);
	if(!mhWnd) {
		gLog ^ "WARNING: CreateWindow([" ^ mName ^ "],[" ^ mName ^ "],[" ^ (int)(Windowed ? WS_OVERLAPPEDWINDOW : WS_POPUP) ^ \
			    "],[" ^ mX ^ "],[" ^ mY ^ "],[" ^ mWidth ^ "],[" ^ mHeight ^ "],NULL,NULL,[" ^ (unsigned __int64)gSettings.hInstance ^ "],NULL) failed.\n";
		return false;
	}

	ShowWindow(mhWnd,SW_SHOW);
	UpdateWindow(mhWnd);

	gSettings.hWnd = mhWnd;
	
	return true;
}