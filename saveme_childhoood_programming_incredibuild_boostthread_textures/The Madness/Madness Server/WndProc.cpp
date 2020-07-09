
#include "WndProc.h"
#include "ClientSettingsInterface.h" 

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{ 
	case WM_CLOSE:
	case WM_DESTROY:
		gSettings.Done = true;
		return 0;
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
			gSettings.Done = true;
		return 0;
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}