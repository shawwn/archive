#ifndef DLLEXPORT_H
#define DLLEXPORT_H

#ifdef _MSC_VER
#ifdef COMMON_EXPORTS
#define DLLIMP __declspec(dllexport)
#else  //SIMPLEDLL_EXPORTS
#define DLLIMP __declspec(dllimport)
#endif //SIMPLEDLL_EXPORTS
#else  //_MSC_VER
#define DLLIMP
#endif //_MSC_VER

#include <Windows.h>

#define M_OK 1
#define M_ERR 0

typedef VOID(CALLBACK* CONSOLECALLBACK)(const char *);

extern "C" DLLIMP int MadnessInit(HWND hWnd, CONSOLECALLBACK ccb);
extern "C" DLLIMP int MadnessUpdate();
extern "C" DLLIMP int MadnessRender();
extern "C" DLLIMP int MadnessCleanup();
extern "C" DLLIMP int MadnessLog(const char *str);
extern "C" DLLIMP int MadnessCommand(const char *str);

#endif