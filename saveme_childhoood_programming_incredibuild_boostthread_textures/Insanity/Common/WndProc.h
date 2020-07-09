#ifndef WNDPROC_H
#define WNDPROC_H

#include <windows.h>

extern LRESULT CALLBACK GlobalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif