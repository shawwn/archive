#include "GUIInterface.h"
#include "ClientSettingsInterface.h"
using namespace std;

GUIInterface gGUI;

UINT GUIInterface::AddWindow(const std::string &Name, UINT Width, UINT Height, UINT X, UINT Y)
{
	GUIWindow *wnd = new GUIWindow(Name,Width,Height,X,Y,mGUID);
	mWindows[mGUID] = wnd;
	mGUID++;
	if(WindowCount() == 1)
		mFocusWindow = wnd;
	return mGUID-1;

}

bool GUIInterface::RemoveWindow(UINT Handle)
{
	Iterator it = mWindows.find(Handle);
	if(it == End())
		return false;

	SAFE_DELETE((*it).second);
	mWindows.erase(it);

	return true;
}

bool GUIInterface::Render(LPDIRECT3DDEVICE9 Device)
{
	D3DXMatrixIdentity(&mScaling);
	D3DXMatrixIdentity(&mTranslation);

	D3DXMatrixOrthoOffCenterLH(&mOrtho,0,(float)gSettings.Width,(float)gSettings.Height,0,1,1000);
	Device->SetTransform(D3DTS_PROJECTION,&mOrtho);

	D3DXMatrixLookAtLH(&mView,&D3DXVECTOR3(0,0,-1),&D3DXVECTOR3(0,0,0),&D3DXVECTOR3(0,1,0));
	Device->SetTransform(D3DTS_VIEW,&mView);

	for(Iterator it = Begin(); it != End(); it++) {
		GUIWindow *wnd = (*it).second;
		D3DXMatrixTranslation(&mTranslation,(float)wnd->GetXOffset(),(float)wnd->GetYOffset(),0);
		D3DXMatrixScaling(&mScaling,(float)wnd->GetWidth(),(float)wnd->GetHeight(),1);
		mFinal = mScaling * mTranslation;
		Device->SetTransform(D3DTS_WORLD,&mFinal);
		Device->SetFVF(D3DFVF_XYZ);
		if(Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,mQuad,sizeof(D3DXVECTOR3)) != D3D_OK)
			return false;
	}

	return true;
}