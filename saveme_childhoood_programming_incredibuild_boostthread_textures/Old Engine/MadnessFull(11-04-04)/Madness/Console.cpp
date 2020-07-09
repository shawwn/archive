#include "dxstdafx.h"
#include "Console.h"
#include "ConfigurationInterface.h"
#include "InputInterface.h"
#include "ParsingInterface.h"
#include "ClientNetworking.h"

Console::Console()
{
}

Console::~Console()
{
	Destroy();
}

bool Console::Initialize(int X, int Y, int Width, int Height, int ScrollStride, bool KeepFocus, bool NetworkChatBox)
{
	Destroy();

	mWidth = Width;
	mHeight = Height;
	mX = X;
	mY = Y;
	mScrollStride = ScrollStride;
	mKeepFocus = KeepFocus;
	mNetworkChatBox = NetworkChatBox;

	mUI.SetLocation(mX,mY);
	mUI.SetSize(mWidth,mHeight);

	mUI.AddEditBox(CONSOLE_EDITBOX,L"",0,mHeight,mWidth,mHeight/4);
	mUI.AddButton(CONSOLE_SUBMIT,L"Submit",0,0,mWidth,mHeight);

	return true;
}

void Console::ProcessCommand(const string &CommandName)
{
	mCommandNames[StringToWideString(UpperString(CommandName))] = true;
}

void Console::OnLostDevice()
{
	mBox.OnLostDevice();
}


bool Console::OnResetDevice(LPDIRECT3DDEVICE9 Device)
{
	if(!Device)
		return false;
	mBox.OnResetDevice(Device);

	mEditBox.X = 0;
	mEditBox.Y = mHeight;
	mEditBox.Width = (int)(mWidth/1.10f);
	mEditBox.Height = 35;
	mSubmit.X = (int)(mWidth/1.10f);
	mSubmit.Y = mHeight;
	mSubmit.Width = mWidth - mEditBox.Width;
	mSubmit.Height = 35;

	mUI.SetBackgroundColors(D3DCOLOR_ARGB(128,255,255,255));
	mUI.GetEditBox(CONSOLE_EDITBOX)->SetLocation(mEditBox.X,mEditBox.Y);
	mUI.GetEditBox(CONSOLE_EDITBOX)->SetSize(mEditBox.Width,mEditBox.Height);
	mUI.GetButton(CONSOLE_SUBMIT)->SetLocation(mSubmit.X,mSubmit.Y);
	mUI.GetButton(CONSOLE_SUBMIT)->SetSize(mSubmit.Width,mSubmit.Height);

	mBox.Set(Device,mX,mY,mWidth,mHeight,L"Arial",15,0);
	return true;
}

void Console::Print(const wstring &str)
{
	wstring message = str;
	wstring command = message.substr(0,message.find_first_of(' '));
	for(size_t i = 0; i < command.size(); i++) { command[i] = toupper(command[i]); }
	//if(command != L"MAP" && command != L"DEALLOCATE" && command != L"RESOLUTION" && command != L"LIGHTMAPS" && command != L"HMAP") {
	std::map<wstring,bool>::iterator it;
	if((it = mCommandNames.find(command)) == mCommandNames.end()) 
	{
		mBox.Print(str);
		return;
	}

	ConsoleCommand cmd;
	cmd.mCommand = command;
	wstring argument;
	wostringstream stream;

/*	if(message.find_first_of(' ') == std::wstring::npos) {
		stream << cmd.mCommand << L": This command does not take 0 arguments.";
		Print(stream.str());

		return;  // No arguments
	}*/
	message = message.substr(message.find_first_of(' ')+1,message.length());
	/*if(message.length() == 0)
	{
		stream << cmd.mCommand << L": This command does not take 0 arguments.";
		Print(stream.str());

		return; // No arguments
	}*/

	while(message.length() > 0)
	{
		if(message.find_first_of(' ') == std::wstring::npos) {
			argument = message;
			message = L"";
		}
		else
		{
			argument = message.substr(0,message.find_first_of(' '));
			message = message.substr(message.find_first_of(' ')+1,message.length());
		}

		cmd.mArguments.push_back(argument);
        
		//message = message.substr(message.find_first_of(' ')+1,message.length());
	}

	mCommands.push_back(cmd);
}

bool Console::Render(LPDIRECT3DDEVICE9 Device)
{
	if(!mEnabled)
		return false;

	if(gInput->KeyDownOnce(DIK_PRIOR))
		mScroll += mScrollStride;
	if(gInput->KeyDownOnce(DIK_NEXT))
		mScroll -= mScrollStride;

	mUI.OnRender(DXUTGetElapsedTime());
	mBox.Render(mScroll);


	return true;
}
void Console::Output()
{
	if(mUI.GetEditBox(CONSOLE_EDITBOX)->GetTextLength() > 0) {
		if(mNetworkChatBox)
			gNetworking->Chat().Say(mUI.GetEditBox(CONSOLE_EDITBOX)->GetText());
		else
			Print(mUI.GetEditBox(CONSOLE_EDITBOX)->GetText());
	}
	ClearText();
	if(!mKeepFocus)
	{
		mUI.ClearFocus();
		gConfig->SetLookWithMouse(true);
	}
	mJustPrinted = true;
}
bool Console::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(!mEnabled)
		return false;
	mUI.MsgProc(hWnd,uMsg,wParam,lParam);

	if(gInput && mUI.GetEditBox(CONSOLE_EDITBOX))
	{
		if(gInput->KeyDownOnce(DIK_RETURN) && mUI.GetEditBox(CONSOLE_EDITBOX)->m_bHasFocus && !mRequestingFocus)
		{
			Output();
			return true;
		}
		if(gInput->KeyDownOnce(DIK_RETURN) && !mUI.GetEditBox(CONSOLE_EDITBOX)->m_bHasFocus && !mJustPrinted)
		{
			RequestFocus();
			return true;
		}
		if(!gInput->KeyDownOnce(DIK_RETURN)) {
			mRequestingFocus = false;
			mJustPrinted = false;
		}

		if(mUI.GetEditBox(CONSOLE_EDITBOX)->m_bHasFocus)
			return true;
		if(mUI.GetButton(CONSOLE_SUBMIT)->m_bHasFocus) {
			Output();
			return true;
		}
	}

	return false;
}

void Console::Destroy()
{
	mRequestingFocus = false;
	mJustPrinted = false;
	mNetworkChatBox = false;
	mUI.RemoveAllControls();
	mEnabled = false;
	mScroll = 0;
	mX = 0;
	mY = 0;
	mWidth = 0;
	mHeight = 0;
}

void Console::RequestFocus()
{
	mRequestingFocus = true;
	mUI.RequestFocus(mUI.GetEditBox(CONSOLE_EDITBOX));
	gConfig->SetLookWithMouse(true);
}

void Console::ClearText()
{
	mUI.GetEditBox(CONSOLE_EDITBOX)->ClearText();
}

void Console::SetEnabled(const bool &Set)
{
	mEnabled = Set;
	if(mEnabled && mKeepFocus)
	{
		RequestFocus();
	}
	if(!mEnabled)
	{
		ClearText();
	}
}

void Console::ToggleEnabled()
{
	mEnabled = !mEnabled;
	if(mEnabled && mKeepFocus)
	{
		RequestFocus();
	}
	if(!mEnabled)
	{
		ClearText();
	}
}