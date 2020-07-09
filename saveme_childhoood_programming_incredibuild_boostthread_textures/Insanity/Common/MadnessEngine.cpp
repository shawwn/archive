#include "MadnessEngine.h"
#include "Direct3DInterface.h"
#include "LoggingInterface.h"

MadnessEngine gMadness;

bool MadnessEngine::Initialize(HWND hWnd)
{
	gLog.SetOutputFile("Game.log");
	gLog.SetUseCout(false);
	PrintProperties() << ": initializing...\n";

	ghWnd = hWnd;

	if(!gD3D.Initialize()) {
		PrintProperties() << " ERROR: Failed to initialize D3D.\n";
		return false;
	}

	PrintProperties() << ": initialization complete.\n";

	return true;
}

bool MadnessEngine::Update()
{
	return true;
}

bool MadnessEngine::Render()
{
	gD3D.GetDevice()->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,RGB(20,0,0),1.0f,0);
	gD3D.GetDevice()->BeginScene();
	gD3D.GetDevice()->EndScene();
	gD3D.GetDevice()->Present(NULL,NULL,0,NULL);

	return true;
}

bool MadnessEngine::Cleanup()
{
  gConsoleCallback = NULL;
  PrintProperties() << ": Cleaned up.\n";
	return true;
}

bool MadnessEngine::Command(const std::string &cmd)
{
  if(cmd == "exit") {
    if(gConsoleCallback)
      gConsoleCallback("SYSTEM[EXIT]");
  }

  gLog << "COMMAND[" << cmd << "]\n";
  return true;
}
