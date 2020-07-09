#ifndef MADNESSENGINE_H
#define MADNESSENGINE_H

#include "Engine.h"
#include <windows.h>

class MadnessEngine : public Engine
{
private:
public:
	MadnessEngine() : Engine("MadnessEngine",ENGINE_MADNESS) { }

	bool Initialize(HWND hWnd);
	bool Update();
	bool Render();
	bool Cleanup();
  bool Command(const std::string &cmd);
};
extern MadnessEngine gMadness;

#endif