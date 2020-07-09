#define NOMINMAX
#include <windows.h>
#include "GameInterface.h" 
#include "Block.h"

#pragma comment(lib,"winmm.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR, INT)
{
	gEngine.Start(hInstance);
	return 0;
}