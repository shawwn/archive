#include "dllexport.h"
#include "MadnessEngine.h"
#include "Globals.h"
#include "LoggingInterface.h"

int MadnessInit(HWND hWnd, CONSOLECALLBACK ccb)
{
  gConsoleCallback = ccb;
	return (gMadness.Initialize(hWnd) ? M_OK : M_ERR);
}

int MadnessUpdate()
{
	return (gMadness.Update() ? M_OK : M_ERR);
}

int MadnessRender()
{
	return (gMadness.Render() ? M_OK : M_ERR);
}

int MadnessCleanup()
{
	return (gMadness.Cleanup() ? M_OK : M_ERR);
}


int MadnessLog(const char *str)
{
  gLog << str;
  return M_OK;
}

int MadnessCommand(const char *str)
{
  return (gMadness.Command(str) ? M_OK : M_ERR);
}