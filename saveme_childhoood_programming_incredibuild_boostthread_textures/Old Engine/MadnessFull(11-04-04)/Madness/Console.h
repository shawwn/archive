#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <list>
#include <map>
#include <vector>
#include <sstream>
#include "dxutgui.h"
#include "TextBox.h"
using namespace std;

#define CONSOLE_EDITBOX 0
#define CONSOLE_SUBMIT  1

class ConsoleItemDesc
{
public:
	ConsoleItemDesc() : X(0), Y(0), Width(0), Height(0)
	{ }
	int X;
	int Y;
	int Width;
	int Height;
};

class ConsoleCommand
{
public:
	ConsoleCommand()
	{ }

	wstring mCommand;
	std::vector<wstring> mArguments;

	int ArgCount() { return (int)mArguments.size(); }
};

class Console
{
private:
	CDXUTDialog mUI;
	int mX;
	int mY;
	int mWidth;
	int mHeight;
	int mScroll;
	int mScrollStride;
	bool mKeepFocus;
	bool mNetworkChatBox;

	TextBox mBox;
	ConsoleItemDesc mEditBox;
	ConsoleItemDesc mSubmit;

	std::map<wstring,bool> mCommandNames;
	std::list<ConsoleCommand> mCommands;
	
	bool mRequestingFocus;
	bool mJustPrinted;
	bool mEnabled;

	void Output();
public:
    Console();
	~Console();

	bool Initialize(int X, int Y, int Width, int Height, int ScrollStride, bool KeepFocus, bool NetworkChatBox);
	void ProcessCommand(const string &CommandName);
	void OnLostDevice();
	bool OnResetDevice(LPDIRECT3DDEVICE9 Device);
	void Print(const wstring &str);
	bool Render(LPDIRECT3DDEVICE9 Device);
	bool MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Destroy();

	int CommandCount() { return (int)mCommands.size(); }
	ConsoleCommand GetNextCommand() { if(mCommands.size() == 0) { return ConsoleCommand(); } ConsoleCommand cmd = (*mCommands.begin()); mCommands.pop_front(); return cmd; }
	const bool &GetEnabled() { return mEnabled; }
	const bool &HasFocus() { return mUI.GetEditBox(CONSOLE_EDITBOX)->m_bHasFocus; }

	void RequestFocus();
	void ClearText();
	wstring GetText() { return mUI.GetEditBox(CONSOLE_EDITBOX)->GetText(); }
	void SetEnabled(const bool &Set);
	void ToggleEnabled();
};

#endif