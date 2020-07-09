#include "dxstdafx.h"
#include "InputInterface.h"
#include "LoggingInterface.h"

Mouse *gMouse;
Input *gInput;

Mouse::Mouse() : mX(0), mY(0),
	mLastX(0), mLastY(0), 
	mCenterX(1), mCenterY(1), 
	mLButton(false),mMButton(false),mRButton(false),
	mSensitivity(0.1f),mSmoothingDegree(3)
{
}

Mouse::~Mouse()
{

}

void Mouse::UpdateSmoothing()
{
	mBuffer.clear();
}

float Mouse::GetAveragedX()
{
	float Average = 0;
	Iterator it;
	for(it = mBuffer.begin(); it != mBuffer.end(); it++)
	{
		Average += (*it).x;
	}
	Average /= mBuffer.size();
	return Average;
}

float Mouse::GetAveragedY()
{
	float Average = 0;
	Iterator it;
	for(it = mBuffer.begin(); it != mBuffer.end(); it++)
	{
		Average += (*it).y;
	}
	Average /= mBuffer.size();
	return Average;
}

void Mouse::Update()
{
	 POINT Position; 
	 GetCursorPos(&Position);
	 SetXY(Position.x,Position.y);
     mBuffer.push_back(Position);
	 if((int)mBuffer.size() > mSmoothingDegree)
		 mBuffer.pop_front();
}

void Mouse::CenterAll()
{
	mBuffer.clear();
	SetCursorPos(mCenterX,mCenterY);
}

void Mouse::Center()
{
	 SetCursorPos(mCenterX,mCenterY);
}

void Mouse::SetXY(const int &X, const int &Y)
{
	mLastX = mX;
	mLastY = mY;
	mX = X;
	mY = Y;
}

Input::Input()
{
	for(int i = 0; i < 256; i++)
		mKeys[i] = 0;

	mInput = NULL;
	mKeyboard = NULL;

	mDIK[DIK_A] = "A";
	mDIK[DIK_B] = "B";
	mDIK[DIK_C] = "C";
	mDIK[DIK_D] = "D";
	mDIK[DIK_E] = "E";
	mDIK[DIK_F] = "F";
	mDIK[DIK_G] = "G";
	mDIK[DIK_H] = "H";
	mDIK[DIK_I] = "I";
	mDIK[DIK_J] = "J";
	mDIK[DIK_K] = "K";
	mDIK[DIK_L] = "L";
	mDIK[DIK_M] = "M";
	mDIK[DIK_N] = "N";
	mDIK[DIK_O] = "O";
	mDIK[DIK_P] = "P";
	mDIK[DIK_Q] = "Q";
	mDIK[DIK_R] = "R";
	mDIK[DIK_S] = "S";
	mDIK[DIK_T] = "T";
	mDIK[DIK_U] = "U";
	mDIK[DIK_V] = "V";
	mDIK[DIK_W] = "W";
	mDIK[DIK_X] = "X";
	mDIK[DIK_Y] = "Y";
	mDIK[DIK_Z] = "Z";

	mDIK[DIK_0] = "0";
	mDIK[DIK_1] = "1";
	mDIK[DIK_2] = "2";
	mDIK[DIK_3] = "3";
	mDIK[DIK_4] = "4";
	mDIK[DIK_5] = "5";
	mDIK[DIK_6] = "6";
	mDIK[DIK_7] = "7";
	mDIK[DIK_8] = "8";
	mDIK[DIK_9] = "9";

	mDIK[DIK_NUMLOCK] = "Numlock";
	mDIK[DIK_NUMPADENTER] = "Num_Enter";
	mDIK[DIK_DECIMAL] = "Num_.";
	mDIK[DIK_DIVIDE] = "Num_/";
	mDIK[DIK_MULTIPLY] = "Num_*";
	mDIK[DIK_SUBTRACT] = "Num_-";
	mDIK[DIK_ADD] = "Num_+";
	mDIK[DIK_NUMPAD0] = "Num_0";
	mDIK[DIK_NUMPAD1] = "Num_1";
	mDIK[DIK_NUMPAD2] = "Num_2";
	mDIK[DIK_NUMPAD3] = "Num_3";
	mDIK[DIK_NUMPAD4] = "Num_4";
	mDIK[DIK_NUMPAD5] = "Num_5";
	mDIK[DIK_NUMPAD6] = "Num_6";
	mDIK[DIK_NUMPAD7] = "Num_7";
	mDIK[DIK_NUMPAD8] = "Num_8";
	mDIK[DIK_NUMPAD9] = "Num_9";

	mDIK[DIK_F1] = "F1";
	mDIK[DIK_F2] = "F2";
	mDIK[DIK_F3] = "F3";
	mDIK[DIK_F4] = "F4";
	mDIK[DIK_F5] = "F5";
	mDIK[DIK_F6] = "F6";
	mDIK[DIK_F7] = "F7";
	mDIK[DIK_F8] = "F8";
	mDIK[DIK_F9] = "F9";
	mDIK[DIK_F10] = "F10";
	mDIK[DIK_F11] = "F11";
	mDIK[DIK_F12] = "F12";

	mDIK[DIK_BACKSLASH] = "\\";
	mDIK[DIK_COLON] = ":";
	mDIK[DIK_COMMA] = ",";
	mDIK[DIK_EQUALS] = "=";
	mDIK[DIK_GRAVE] = "\'";
	mDIK[DIK_PERIOD] = ".";
	mDIK[DIK_SEMICOLON] = ";";
	mDIK[DIK_SLASH] = "/";
	mDIK[DIK_LBRACKET] = "[";
	mDIK[DIK_RBRACKET] = "]";
	
	mDIK[DIK_SPACE] = "Space";
	mDIK[DIK_BACK] = "Backspace";
	mDIK[DIK_CAPITAL] = "Capslock";
	mDIK[DIK_DELETE] = "Delete";
	mDIK[DIK_END] = "End";
	mDIK[DIK_ESCAPE] = "Escape";
	mDIK[DIK_HOME] = "Home";
	mDIK[DIK_INSERT] = "Insert";
	mDIK[DIK_LCONTROL] = "LControl";
	mDIK[DIK_RCONTROL] = "RControl";
    mDIK[DIK_LMENU] = "LAlt";
	mDIK[DIK_RMENU] = "RAlt";
	mDIK[DIK_LSHIFT] = "LShift";
	mDIK[DIK_RSHIFT] = "RShift";
	mDIK[DIK_LWIN] = "LWin";
	mDIK[DIK_RWIN] = "RWin";
	mDIK[DIK_NEXT] = "PageDown";
	mDIK[DIK_PRIOR] = "PageUp";
	mDIK[DIK_PAUSE] = "Pause";
	mDIK[DIK_SCROLL] = "ScrollLock";
	mDIK[DIK_TAB] = "Tab";
}

Input::~Input()
{
	ReleaseKeyboard();
	ReleaseDirectInput();
}

bool Input::InitDirectInput()
{
	gLog->Print("Input: Initializing DirectInput...");
	if(FAILED(DirectInput8Create(GetModuleHandle(NULL),DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&mInput,NULL)))
	{
		gLog->Print("Input: Could not initialize DirectInput 8.");
		return false;
	}
	return true;
}

bool Input::InitKeyboard()
{
	gLog->Print("Input: Initializing keyboard...");
	if(FAILED(mInput->CreateDevice(GUID_SysKeyboard,&mKeyboard,NULL)))
	{
		gLog->Print("Input: Could not initialize keyboard.");
		return false;
	}

	if(FAILED(mKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		gLog->Print("Input: Could not set the keyboard data format.");
		return false;
	}

	if(FAILED(mKeyboard->Acquire()))
	{
		gLog->Print("Input: Could not acquire keyboard.");
		return false;
	}

	return true;
}

bool Input::UpdateKeyboard()
{
	if(!mKeyboard) {
		gLog->Print("Input: Could not update keyboard.  The keyboard is null.");
		return false;
	}
	HRESULT hr;
	memcpy((void *)&mKeysLast,(void *)&mKeys,256);
	hr = mKeyboard->GetDeviceState(256,(void *)&mKeys);
	if(FAILED(hr))
	{
		if(hr == DIERR_INPUTLOST)
		{
			if(FAILED(mKeyboard->Acquire()))
				return false;
			if(FAILED(mKeyboard->GetDeviceState(256,(void *)&mKeys)))
				return false;
			return true;
		}
		gLog->Print("Input: Could not update keyboard.  Could not get the device state.");
		return false;
	}
	return true;
}

void Input::ReleaseKeyboard()
{
	if(mKeyboard)
		mKeyboard->Release();
	mKeyboard = NULL;
}

void Input::ReleaseDirectInput()
{
	if(mInput)
		mInput->Release();
	mInput = NULL;
}

bool Input::KeyDown(const int &Key)
{
	if(!this)
		return false;
	if(mKeys[Key] & 0x80)
		return true;
	return false;
}

bool Input::KeyDownOnce(const int &Key)
{
	if(!this)
		return false;
	if(!(mKeysLast[Key] & 0x80) && (mKeys[Key] & 0x80))
		return true;
	return false;
}

bool Input::Init()
{
	if(!InitDirectInput())
		return false;
	if(!InitKeyboard())
		return false;
	return true;
}

bool Input::Update()
{
	return UpdateKeyboard();
}

string Input::DIKtoASCII(const int &DIK) 
{ 
	return mDIK[DIK]; 
}

int Input::ASCIItoDIK(const string &s) 
{ 
	for(int i = 0; i < 1024; i++) 
	{ 
		if(stricmp(mDIK[i].c_str(),s.c_str()) == 0) 
		{ 
			return i; 
		} 
	} 
	return 0; 
}

void Input::Destroy()
{
	ReleaseKeyboard();
	ReleaseDirectInput();
}