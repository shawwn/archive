#ifndef GLOBALS_H
#define GLOBALS_H

#include <windows.h>
#include <d3dx9.h>
#include "Types.h"
#include "dllexport.h"

extern bool gDebugSwitch;
extern bool gDone;
extern UINT gWidth;
extern UINT gHeight;
extern bool gWindowed;
extern HWND ghWnd;
extern D3DFORMAT gDepthStencilFormat;
extern D3DFORMAT gBackBufferFormat;
extern UINT gDisplayBits;
extern UINT gDepthBits;
extern UINT gVertexProcessing;
extern bool gDeviceDestroyed;
extern D3DDEVTYPE gDeviceType;
extern CONSOLECALLBACK gConsoleCallback;

void OnLostDevice();
void OnResetDevice();
void OnDestroyDevice();

#endif