#include "EnumStringInterface.h"
#include <d3dx9.h>
#include <sstream>
using namespace std;

EnumStringInterface EnumToString;

const std::string &EnumStringInterface::Find(int Value, const std::string &Prefix, const std::string &AlternatePrefix) 
{ 
	static string EnumPrefix;
	static size_t Pos;
	Iterator it = mStrings.find(Value); 
	if(it != End()) 
	{ 
		const vector<string> &v = (*it).second;
		for(size_t i = 0; i < v.size(); i++) {
			Pos = v[i].find_first_of('_');
			if(Pos == std::string::npos || Prefix == "")
				return v[i];
			EnumPrefix = v[i].substr(0,Pos);
			if(EnumPrefix == Prefix || EnumPrefix == AlternatePrefix)
				return v[i];
			
		}
	} 
	static ostringstream stream;
	stream << Value;
	static string blank = stream.str();
	stream.str(std::string());
	return blank; 
}

EnumStringInterface::EnumStringInterface()
{
	mStrings[D3DFMT_A8R8G8B8].push_back	("D3DFMT_A8R8G8B8");
	mStrings[D3DFMT_X8R8G8B8].push_back	("D3DFMT_X8R8G8B8");
	mStrings[D3DFMT_R8G8B8].push_back	("D3DFMT_R8G8B8");
	mStrings[D3DFMT_R5G6B5].push_back	("D3DFMT_R5G6B5");
	mStrings[D3DFMT_D16].push_back		("D3DFMT_D16");
	mStrings[D3DFMT_D24S8].push_back	("D3DFMT_D24S8");

	mStrings[D3DDEVTYPE_HAL].push_back	("D3DDEVTYPE_HAL");
	mStrings[D3DDEVTYPE_REF].push_back	("D3DDEVTYPE_REF");
	mStrings[D3DDEVTYPE_SW].push_back	("D3DDEVTYPE_SW");

	mStrings[D3DCREATE_HARDWARE_VERTEXPROCESSING].push_back	("D3DCREATE_HARDWARE_VERTEXPROCESSING");
	mStrings[D3DCREATE_SOFTWARE_VERTEXPROCESSING].push_back	("D3DCREATE_SOFTWARE_VERTEXPROCESSING");
	mStrings[D3DCREATE_MIXED_VERTEXPROCESSING].push_back	("D3DCREATE_MIXED_VERTEXPROCESSING");

	mStrings[D3DERR_CONFLICTINGRENDERSTATE].push_back	("D3DERR_CONFLICTINGRENDERSTATE");
	mStrings[D3DERR_DEVICELOST].push_back				("D3DERR_DEVICELOST");
	mStrings[D3DERR_DEVICENOTRESET].push_back			("D3DERR_DEVICENOTRESET");
	mStrings[D3DERR_DRIVERINTERNALERROR].push_back		("D3DERR_DRIVERINTERNALERROR");
	mStrings[D3DERR_INVALIDCALL].push_back				("D3DERR_INVALIDCALL");
	mStrings[D3DERR_INVALIDDEVICE].push_back			("D3DERR_INVALIDDEVICE");
	mStrings[D3DERR_MOREDATA].push_back					("D3DERR_MOREDATA");
	mStrings[D3DERR_NOTAVAILABLE].push_back				("D3DERR_NOTAVAILABLE");
	mStrings[D3DERR_NOTFOUND].push_back					("D3DERR_NOTFOUND");
	mStrings[D3DERR_OUTOFVIDEOMEMORY].push_back			("D3DERR_OUTOFVIDEOMEMORY");
	mStrings[D3DERR_TOOMANYOPERATIONS].push_back		("D3DERR_TOOMANYOPERATIONS");
	mStrings[D3DERR_UNSUPPORTEDALPHAARG].push_back		("D3DERR_UNSUPPORTEDALPHAARG");
	mStrings[D3DERR_UNSUPPORTEDALPHAOPERATION].push_back("D3DERR_UNSUPPORTEDALPHAOPERATION");
	mStrings[D3DERR_UNSUPPORTEDCOLORARG].push_back		("D3DERR_UNSUPPORTEDCOLORARG");
	mStrings[D3DERR_UNSUPPORTEDCOLOROPERATION].push_back("D3DERR_UNSUPPORTEDCOLOROPERATION");
	mStrings[D3DERR_UNSUPPORTEDFACTORVALUE].push_back	("D3DERR_UNSUPPORTEDFACTORVALUE");
	mStrings[D3DERR_UNSUPPORTEDTEXTUREFILTER].push_back	("D3DERR_UNSUPPORTEDTEXTUREFILTER");
	mStrings[D3DERR_WRONGTEXTUREFORMAT].push_back		("D3DERR_WRONGTEXTUREFORMAT");

	mStrings[E_FAIL].push_back			("E_FAIL");
	mStrings[E_INVALIDARG].push_back	("E_INVALIDARG");
	mStrings[E_OUTOFMEMORY].push_back	("E_OUTOFMEMORY");
	mStrings[S_OK].push_back			("S_OK");

	mStrings[D3DPOOL_DEFAULT].push_back		("D3DPOOL_DEFAULT");
	mStrings[D3DPOOL_MANAGED].push_back		("D3DPOOL_MANAGED");
	mStrings[D3DPOOL_SYSTEMMEM].push_back	("D3DPOOL_SYSTEMMEM");
	mStrings[D3DPOOL_SCRATCH].push_back		("D3DPOOL_SCRATCH");

	mStrings[D3DUSAGE_AUTOGENMIPMAP].push_back		("D3DUSAGE_AUTOGENMIPMAP");
	mStrings[D3DUSAGE_DEPTHSTENCIL].push_back		("D3DUSAGE_DEPTHSTENCIL");
	mStrings[D3DUSAGE_DMAP].push_back				("D3DUSAGE_DMAP");
	mStrings[D3DUSAGE_DONOTCLIP].push_back			("D3DUSAGE_DONOTCLIP");
	mStrings[D3DUSAGE_DYNAMIC].push_back			("D3DUSAGE_DYNAMIC");
	mStrings[D3DUSAGE_NPATCHES].push_back			("D3DUSAGE_NPATCHES");
	mStrings[D3DUSAGE_POINTS].push_back				("D3DUSAGE_POINTS");
	mStrings[D3DUSAGE_RENDERTARGET].push_back		("D3DUSAGE_RENDERTARGET");
	mStrings[D3DUSAGE_RTPATCHES].push_back			("D3DUSAGE_RTPATCHES");
	mStrings[D3DUSAGE_SOFTWAREPROCESSING].push_back	("D3DUSAGE_SOFTWAREPROCESSING");
	mStrings[D3DUSAGE_WRITEONLY].push_back			("D3DUSAGE_WRITEONLY");

	mStrings[D3DRTYPE_SURFACE].push_back		("D3DRTYPE_SURFACE");
	mStrings[D3DRTYPE_VOLUME].push_back			("D3DRTYPE_VOLUME");
	mStrings[D3DRTYPE_TEXTURE].push_back		("D3DRTYPE_TEXTURE");
	mStrings[D3DRTYPE_VOLUMETEXTURE].push_back	("D3DRTYPE_VOLUMETEXTURE");
	mStrings[D3DRTYPE_CUBETEXTURE].push_back	("D3DRTYPE_CUBETEXTURE");
	mStrings[D3DRTYPE_VERTEXBUFFER].push_back	("D3DRTYPE_VERTEXBUFFER");
	mStrings[D3DRTYPE_INDEXBUFFER].push_back	("D3DRTYPE_INDEXBUFFER");
	mStrings[D3DRTYPE_FORCE_DWORD].push_back	("D3DRTYPE_FORCE_DWORD");
}