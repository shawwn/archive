#include "Globals.h"
#include "MadnessEngine.h"

bool gDebugSwitch = true;
bool gDone = false;
bool gWindowed = true;
UINT gWidth = 800;
UINT gHeight = 600;
HWND ghWnd = NULL;
D3DFORMAT gDepthStencilFormat = D3DFMT_D24S8;
D3DFORMAT gBackBufferFormat = D3DFMT_X8R8G8B8;
UINT gDisplayBits = 32;
UINT gDepthBits = 32;
UINT gVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
bool gDeviceDestroyed = false;
D3DDEVTYPE gDeviceType = D3DDEVTYPE_HAL;
CONSOLECALLBACK gConsoleCallback = NULL;


void OnLostDevice()
{
	
}

void OnResetDevice()
{

}

void OnDestroyDevice()
{
}