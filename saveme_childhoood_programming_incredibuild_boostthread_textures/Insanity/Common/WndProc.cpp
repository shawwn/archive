#include "WndProc.h"
#include "Globals.h"

LRESULT CALLBACK GlobalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
	case WM_CLOSE:
		gDone = true;
		return TRUE;		
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}