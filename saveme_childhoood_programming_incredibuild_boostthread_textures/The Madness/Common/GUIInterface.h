#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <map>
#include "GUITypes.h"
#include "Direct3DInterface.h"

class GUIInterface
{
private:
	typedef std::map<UINT,GUIWindow *>::iterator Iterator;
	std::map<UINT,GUIWindow *> mWindows;
	GUIWindow *mFocusWindow;
	UINT mGUID;

	D3DXVECTOR3 mQuad[4];
	D3DXMATRIX mOrtho;
	D3DXMATRIX mView;
	D3DXMATRIX mScaling;
	D3DXMATRIX mTranslation;
	D3DXMATRIX mFinal;

	inline Iterator Begin() { return mWindows.begin(); }
	inline Iterator End() { return mWindows.end(); }
public:
	GUIInterface() : mFocusWindow(NULL), mGUID(0) {
		mQuad[0] = D3DXVECTOR3(0,1,1);
		mQuad[1] = D3DXVECTOR3(1,1,1);
		mQuad[2] = D3DXVECTOR3(1,0,1);
		mQuad[3] = D3DXVECTOR3(0,0,1);
	}
	~GUIInterface() { for(Iterator it = Begin(); it != End(); it++) { SAFE_DELETE((*it).second); } mWindows.clear(); }

	UINT AddWindow(const std::string &Name, UINT Width, UINT Height, UINT X, UINT Y);
	bool RemoveWindow(UINT Handle);

	bool Render(LPDIRECT3DDEVICE9 Device);

	inline UINT WindowCount() { return (UINT)mWindows.size(); }

};
extern GUIInterface gGUI;

#endif