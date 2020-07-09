#ifndef SETTINGSINTERFACE_H
#define SETTINGSINTERFACE_H

#include "Interface.h"
#include "Globals.h"

class SettingsInterface : public Interface
{
private:
	bool Done;
	UINT Width;
	UINT Height;
public:
	SettingsInterface();
	~SettingsInterface();

	inline bool GetDone() const { return Done; }
	inline UINT GetWidth() const { return Width; }
	inline UINT GetHeight() const { return Height; }
	void SetDone(bool Set) { Done = Set; gDone = Done; }
	void SetWidth(UINT Set ) { Width = Set; gWidth = Width; }
	void SetHeight(UINT Set) { Height = Set; gHeight = Height; }

};

#endif