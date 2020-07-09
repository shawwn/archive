#ifndef INPUTINTERFACE_H
#define INPUTINTERFACE_H

#include <string>
#include <list>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class MousePoint
{
public:
	MousePoint() : X(0), Y(0) { }
	MousePoint(int sX, int sY) : X(sX), Y(sY) { }
	int X, Y;

	MousePoint &operator =(const MousePoint &m) { X = m.X; Y = m.Y; return *this; }
};

#define MOUSE_LEFT		 0
#define MOUSE_RIGHT		 1
#define MOUSE_MIDDLE	 2
#define MOUSE_SIDE_LEFT  3
#define MOUSE_SIDE_RIGHT 4

class Mouse
{
private:
	LPDIRECTINPUTDEVICE8 mDevice;
	DIMOUSESTATE2 mPrevState;
	DIMOUSESTATE2 mState;

	typedef std::list<MousePoint>::iterator Iterator;
	std::list<MousePoint> mPoints;
	MousePoint mCenterOfScreen;
	float mDiffX, mDiffY;
	float mAvgX, mAvgY;
	float mSensitivity;
	unsigned int mSmoothFactor;
	inline Iterator Begin() { return mPoints.begin(); }
	inline Iterator End() { return mPoints.end(); }
public:
	Mouse();
	bool Initialize(LPDIRECTINPUT8 DI);
	bool Update();

	inline void SetSensitivity(float Sens) { mSensitivity = Sens; }
	inline void SetCenterOfScreen(const MousePoint &Center) { mCenterOfScreen = Center; }

	inline float GetDiffX() { return mDiffX; }
	inline float GetDiffY() { return mDiffY; }
	inline bool ButtonDown(UINT Which) { return (Which < 5 ? ((mState.rgbButtons[Which] >> 7) == 1) : false); }
	inline bool ButtonDownOnce(UINT Which) { return (Which < 5 ? 
		(((mPrevState.rgbButtons[Which] >> 7) == 0) && ((mState.rgbButtons[Which] >> 7) == 1))
		: false); }
	inline LPDIRECTINPUTDEVICE8 &GetDevice() { return mDevice; }
};

class Keyboard
{
private:
	LPDIRECTINPUTDEVICE8 mDevice;
	BYTE mKeys[256];
	BYTE mKeysLast[256];
public:
	Keyboard() : mDevice(NULL) { ZeroMemory(mKeysLast,256); }

	bool Initialize(LPDIRECTINPUT8 DI);
	bool Update();

	inline bool KeyDown(short KeyCode) { return ((mKeys[KeyCode] & 0x80) != 0); }
	inline bool KeyDownOnce(short KeyCode) { return ((mKeys[KeyCode] & 0x80) && !(mKeysLast[KeyCode] & 0x80)); }
	inline void EraseKey(short KeyCode) { mKeys[KeyCode] = 0; }

	inline LPDIRECTINPUTDEVICE8 &GetDevice() { return mDevice; }
};

class InputInterface
{
private:
	LPDIRECTINPUT8 mDI;
	std::string mDikToString[256];
	Mouse mMouse;
	Keyboard mKeyboard;

public:
	InputInterface() : mDI(NULL) { }
	~InputInterface() { Deinitialize(); }
	Mouse &GetMouse() { return mMouse; }
	Keyboard &GetKeyboard() { return mKeyboard; }

	bool Initialize();
	bool Update();
	void Deinitialize();
};
extern InputInterface gInput;

#endif