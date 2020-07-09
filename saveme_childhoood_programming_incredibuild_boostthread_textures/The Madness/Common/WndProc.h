#ifndef WNDPROC_H
#define WNDPROC_H

#define NOMINMAX 
#include <windows.h>

extern LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif