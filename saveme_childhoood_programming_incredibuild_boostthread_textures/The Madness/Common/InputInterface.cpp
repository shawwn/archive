#include "InputInterface.h"
#include "LoggingInterface.h"
#include "ClientSettingsInterface.h"
#include "Direct3DInterface.h"

InputInterface gInput;

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

Mouse::Mouse() : mDiffX(0), mDiffY(0), mAvgX(0), mAvgY(0), mSensitivity(10), mSmoothFactor(3), mCenterOfScreen(800,600) { }

bool Mouse::Initialize(LPDIRECTINPUT8 DI)
{
	if(!DI)
		return false;

	HRESULT hr;
	if((hr = DI->CreateDevice(GUID_SysMouse,&mDevice,NULL)) != DI_OK) {
		gLog ^ "ERROR: Could not create mouse!  (No point in playing - mouse won't work.\n";
		return false;
	}

	if((hr = mDevice->SetDataFormat(&c_dfDIMouse2)) != DI_OK) {
		gLog ^ "ERROR: Could not set data format for mouse!\n";
		return false;
	}

	if((hr = mDevice->SetCooperativeLevel(gD3D.GethWnd(),DISCL_EXCLUSIVE | DISCL_FOREGROUND)) != DI_OK) {
		gLog ^ "ERROR: Could not set cooperative level for mouse.\n";
		return false;
	}

	mDevice->Acquire();
	ZeroMemory(&mState,sizeof(DIMOUSESTATE2));
	ZeroMemory(&mPrevState,sizeof(DIMOUSESTATE2));

	return true;
}

bool Mouse::Update()
{ 
	if(!mDevice)
		return false;

	HRESULT hr;

	memcpy(&mPrevState,&mState,sizeof(DIMOUSESTATE2));
	if((hr = mDevice->GetDeviceState(sizeof(DIMOUSESTATE2),&mState)) != DI_OK)
	{
		hr = mDevice->Acquire();
		while(hr == DIERR_INPUTLOST)
			hr = mDevice->Acquire();

		if(hr != DI_OK)
			return false;
	}

	mPoints.push_back(MousePoint(mState.lX,mState.lY)); 
	while(mPoints.size() > mSmoothFactor)
		mPoints.pop_front();
	
    mAvgX = 0;
	mAvgY = 0;
	for(Iterator it = Begin(); it != End(); it++) 
	{
		mAvgX += (*it).X;
		mAvgY += (*it).Y;
	}
	mAvgX /= mPoints.size();
	mAvgY /= mPoints.size();

	mDiffX = mAvgX * mSensitivity;
	mDiffY = mAvgY * mSensitivity;

	SetCursorPos(mCenterOfScreen.X,mCenterOfScreen.Y);

	return true;
}

bool Keyboard::Initialize(LPDIRECTINPUT8 DI)
{
	HRESULT hr;
	if(FAILED(hr = DI->CreateDevice(GUID_SysKeyboard,&mDevice,NULL))) {
		gLog ^ "ERROR: Could not create keyboard device.\n";
		return false;
	}

	if(FAILED(hr = mDevice->SetDataFormat(&c_dfDIKeyboard))) {
		gLog ^ "ERROR: Could not set the keyboard data format.\n";
		return false;
	}

	if(FAILED(hr = mDevice->SetCooperativeLevel(gSettings.hWnd,DISCL_EXCLUSIVE | DISCL_FOREGROUND))) {
		gLog ^ "ERROR: Could not set the keyboard cooperative level.\n";
		return false;
	}

	mDevice->Acquire();
	return true;
}

bool Keyboard::Update()
{
	if(!mDevice)
		return false;

	memcpy(mKeysLast,mKeys,256);

	static HRESULT hr;
	if(FAILED(hr = mDevice->GetDeviceState(256,mKeys)))
	{
		hr = mDevice->Acquire();
		while(hr == DIERR_INPUTLOST)
			hr = mDevice->Acquire();

		if(hr != DI_OK)
			return false;
	}

	if(KeyDown(DIK_ESCAPE))
		gSettings.Done = true;

	return true;
}

bool InputInterface::Initialize()
{
	HRESULT hr;
	
	if(FAILED((hr = DirectInput8Create(GetModuleHandle(NULL),DIRECTINPUT_VERSION,IID_IDirectInput8,(VOID**)&mDI,NULL)))) {
		gLog ^ "ERROR: Could not create input interface!  (No point in playing - keyboard and mouse won't work.)\n";
		return false;
	}

	// Mouse intiialization
	if(!mMouse.Initialize(mDI)) {
		Deinitialize();
		return false;
	}

	if(!mKeyboard.Initialize(mDI)) {
		Deinitialize();
		return false;
	}

	return true;
}

bool InputInterface::Update()
{
	if(!mMouse.Update()) { mKeyboard.Update(); return false; }
	if(!mKeyboard.Update()) { return false; }
	return true;
}

void InputInterface::Deinitialize()
{
	mMouse.GetDevice()->Unacquire();
	mKeyboard.GetDevice()->Unacquire();
	SAFE_RELEASE(mKeyboard.GetDevice());
	SAFE_RELEASE(mMouse.GetDevice());
	SAFE_RELEASE(mDI);
}

