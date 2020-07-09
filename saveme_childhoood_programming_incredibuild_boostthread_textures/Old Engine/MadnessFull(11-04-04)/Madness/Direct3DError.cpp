#include "dxstdafx.h"
#include "Direct3DError.h"
#include "LoggingInterface.h"

#define USET


HRESULT T(const char *Function, const HRESULT &hr)
{
#ifndef USET
	return hr;
#endif

	switch(hr)
	{
	case D3D_OK:
		break;
	case D3DOK_NOAUTOGEN:
		gLog->Stream << Function << ": D3DERR_NOAUTOGEN.  This is a success code.  However, the autogeneration of mipmaps is not supported for this format.  This means that resource creation will succeed but the mipmap levels will not be automatically generated.";
		gLog->Print();
		break;
	case D3DERR_CONFLICTINGRENDERSTATE:
		gLog->Stream << Function << ": D3DERR_CONFLICTINGRENDERSTATE.  The currently set render states cannot be used together.";
		gLog->Print();
		break;
	case D3DERR_CONFLICTINGTEXTUREFILTER:
		gLog->Stream << Function << ": D3DERR_CONFLICTINGTEXTUREFILTER.  The current texture filters cannot be used together.";
		gLog->Print();
		break;
	case D3DERR_CONFLICTINGTEXTUREPALETTE:
		gLog->Stream << Function << ": D3DERR_CONFLICTINGTEXTUREPALETTE.  The current textures cannot be used simultaniously.";
		gLog->Print();
		break;
	case D3DERR_DEVICELOST:
		gLog->Stream << Function << ": D3DERR_DEVICELOST.  The device has been lost but cannot be reset at this time.  Therefore, rendering is not possible.";
		gLog->Print();
		break;
	case D3DERR_DEVICENOTRESET:
		gLog->Stream << Function << ": D3DERR_DEVICENOTRESET.  The device has been lost but can be reset at this time.";
		gLog->Print();
		break;
	case D3DERR_DRIVERINTERNALERROR:
		gLog->Stream << Function << ": D3DERR_DRIVERINTERNALERROR.  Internal driver error.  Shutting down...";
		gLog->Print();
		exit(0);
		break;
	case D3DERR_INVALIDDEVICE:
		gLog->Stream << Function << ": D3DERR_INVALIDDEVICE.  The required device type is not valid.";
		gLog->Print();
		break;
	case D3DERR_MOREDATA:
		gLog->Stream << Function << ": D3DERR_MOREDATA.  There is more data available than the specified buffer size can hold.";
		gLog->Print();
		break;
	case D3DERR_NOTAVAILABLE:
		gLog->Stream << Function << ": D3DERR_NOTAVAILABLE.  This device does not support the queried technique.";
		gLog->Print();
		break;
	case D3DERR_NOTFOUND:
		gLog->Stream << Function << ": D3DERR_NOTFOUND.  The requested item was not found.";
		gLog->Print();
		break;
	case D3DERR_OUTOFVIDEOMEMORY:
		gLog->Stream << Function << ": D3DERR_OUTOFVIDEOMEMORY.  Direct3D does not have enough display memory to perform the operation.";
		gLog->Print();
		break;
	case D3DERR_TOOMANYOPERATIONS:
		gLog->Stream << Function << ": D3DERR_TOOMANYOPERATIONS.  The application is requesting more texture-filtering operations than the device supports.";
		gLog->Print();
		break;
	case D3DERR_UNSUPPORTEDALPHAARG:
		gLog->Stream << Function << ": D3DERR_UNSUPPORTEDALPHAARG.  The device does not support a specified texture-blending argument for the alpha channel.";
		gLog->Print();
		break;
	case D3DERR_UNSUPPORTEDALPHAOPERATION:
		gLog->Stream << Function << ": D3DERR_UNSUPPORTEDALPHAOPERATION.  The device does not support a specified texture-blending operation for the alpha channel.";
		gLog->Print();
		break;
	case D3DERR_UNSUPPORTEDCOLORARG:
		gLog->Stream << Function << ": D3DERR_UNSUPPORTEDCOLORARG.  The device does not support a specified texture-blending argument for color values.";
		gLog->Print();
		break;
	case D3DERR_UNSUPPORTEDCOLOROPERATION:
		gLog->Stream << Function << ": D3DERR_UNSUPPORTEDCOLOROPERATION.  The device does not support a specified texture-blending operation for color values.";
		gLog->Print();
		break;
	case D3DERR_UNSUPPORTEDFACTORVALUE:
		gLog->Stream << Function << ": D3DERR_UNSUPPORTEDFACTORVALUE.  The device does not support the specified texture factor value.";
		gLog->Print();
		break;
	case D3DERR_UNSUPPORTEDTEXTUREFILTER:
		gLog->Stream << Function << ": D3DERR_UNSUPPORTEDTEXTUREFILTER.  The device does not support the specified texture filter.";
		gLog->Print();
		break;
	case D3DERR_WRONGTEXTUREFORMAT:
		gLog->Stream << Function << ": D3DERR_WRONGTEXTUREFORMAT.  The pixel format of the texture surface is not valid.";
		gLog->Print();
		break;
	case D3DERR_INVALIDCALL:
		gLog->Stream << Function << ": D3DERR_INVALIDCALL.  The method call is invalid.  For example, a method's parameter may not be a valid pointer.";
		gLog->Print();
		break;
	case D3DXERR_CANNOTMODIFYINDEXBUFFER:
		gLog->Stream << Function << ": D3DXERR_CANNOTMODIFYINDEXBUFFER.  The index buffer cannot be modified.";
		gLog->Print();
		break;
	case D3DXERR_INVALIDMESH:
		gLog->Stream << Function << ": D3DXERR_INVALIDMESH.  The mesh is invalid.";
		gLog->Print();
		break;
	case D3DXERR_CANNOTATTRSORT:
		gLog->Stream << Function << ": D3DXERR_CANNOTATTRSORT.  Attribute sort (D3DXMESHOPT_ATTRSORT) is not supported as an optimization technique.";
		gLog->Print();
		break;
	case D3DXERR_SKINNINGNOTSUPPORTED:
		gLog->Stream << Function << ": D3DXERR_SKINNINGNOTSUPPORTED.  Skinning is not supported.";
		gLog->Print();
		break;
	case D3DXERR_TOOMANYINFLUENCES:
		gLog->Stream << Function << ": D3DXERR_TOOMANYINFLUENCES.  Too many influences specified.";
		gLog->Print();
		break;
	case D3DXERR_INVALIDDATA:
		gLog->Stream << Function << ": D3DXERR_INVALIDDATA.  The data is invalid.";
		gLog->Print();
		break;
	case D3DXERR_LOADEDMESHASNODATA:
		gLog->Stream << Function << ": D3DXERR_LOADEDMESHASNODATA.  The mesh has no data.";
		gLog->Print();
		break;
	case E_FAIL:
		gLog->Stream << Function << ": E_FAIL.  An undetermined error occured.";
		gLog->Print();
		break;
	case E_INVALIDARG:
		gLog->Stream << Function << ": E_INVALIDARG.  An invalid parameter was passed to the returning function.";
		gLog->Print();
		break;
	case E_OUTOFMEMORY:
		gLog->Stream << Function << ": E_OUTOFMEMORY.  Direct3D could not allocate sufficient memory to complete the call.";
		gLog->Print();
		break;
	default:
		gLog->Stream << Function << ": An unhandled error occured.";
		gLog->Print();
		break;
	}
	return hr;
}