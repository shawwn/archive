#ifndef _INPUT_H
#define _INPUT_H

#include <dinput.h>
#include <list>
#include <string>
using namespace std;

class Mouse
{
private:
	typedef std::list<POINT>::iterator Iterator;
	int mX, mY;
	int mLastX, mLastY;
	int mCenterX, mCenterY;
	bool mLButton;
	bool mMButton;
	bool mRButton;

	float mSensitivity;
	int mSmoothingDegree;
	std::list<POINT> mBuffer;

	void UpdateSmoothing();
	float GetAveragedX();
	float GetAveragedY();
public:
	Mouse();
	~Mouse();

	void Update();
	void CenterAll();
	void Center();

	void SetXY(const int &X, const int &Y);
	void SetCenter(const int &Width, const int &Height, const int &OffsetX, const int &OffsetY) { mCenterX = (Width >> 1) + OffsetX; mCenterY = (Height >> 1) + OffsetY; }
	void SetLButton(const bool &Yes) { mLButton = Yes; }
	void SetMButton(const bool &Yes) { mMButton = Yes; }
	void SetRButton(const bool &Yes) { mRButton = Yes; }
	void SetSensitivity(const float &Sensitivity) { mSensitivity = Sensitivity; }
	void SetSmoothingDegree(const int &Degree) { if(Degree < 1) { return; } mSmoothingDegree = Degree;  UpdateSmoothing(); }
	const int &GetX() { return mX; }
	const int &GetY() { return mY; }
	const int &GetLastX() { return mLastX; }
	const int &GetLastY() { return mLastY; }
	float GetDifferenceX() { return (GetAveragedX() - mCenterX) * mSensitivity; }
	float GetDifferenceY() { return (mCenterY - GetAveragedY()) * mSensitivity; }
};
extern Mouse *gMouse;

class Input
{
private:
	IDirectInput8 *mInput;
	IDirectInputDevice8 *mKeyboard;
	char mKeys[256];
	char mKeysLast[256];
	string mDIK[1024];

	bool InitDirectInput();
	bool InitKeyboard();
	bool UpdateKeyboard();
	void ReleaseKeyboard();
	void ReleaseDirectInput();
public:
	Input();
	~Input();

    bool Init();
	bool Update();
	void Destroy();
	bool KeyDown(const int &Key);
	bool KeyDownOnce(const int &Key);
	string DIKtoASCII(const int &DIK);
	int ASCIItoDIK(const string &s);
};
extern Input *gInput;

#endif