#ifndef SMARTPOINTER_H
#define SMARTPOINTER_H

#include "Resource.h"
#include "LoggingInterface.h"
#include "Macros.h"
#include "Globals.h"
#include "GUIDManager.h"

extern bool gDebugSwitch;
extern Resource gInvalidResource;

class AutoPointer
{
private:
	static const UINT PointerGUIDTrack = 1;
	Resource *mResource;
	std::string mResourceName;
	ResourceType mResourceType;
	QuadWord mPointerID;
	bool mManaged;
	inline bool Changed() const { if((mResource->GetResourceName() != mResourceName) || (mResource->GetResourceType() != mResourceType)) { return true; } return false; }
	inline LoggingInterface &PrintRefCount() { gLog << "RefCount[" << mResource->GetRefCount() << "]"; return gLog; }

	inline void SetManaged(bool Set) { mManaged = Set; }

	void Unacquire()
	{
		if(!mResource)
			return;
		mResource->Release();
		if(gDebugSwitch) {
			PrintProperties() << ": Destroyed.  Removing RefCount from resource.\n";
		}
		if(mManaged) { 
			if(Changed()) {
				PrintProperties() << " WARNING: On release, resource has changed types since initialization.  Releasing...\n";		
			}
			if(mResource->GetRefCount()) {
				PrintProperties() << " WARNING: On delete, RefCount is >0.\n";
			}

			SAFE_DELETE(mResource); 
		} 
	}
public:
	AutoPointer() : mResource(NULL), mResourceType(NULLTYPE), mPointerID(-1), mManaged(false)
	{ }
	AutoPointer(const AutoPointer &other)  {
		Acquire(other.mResource,false);
	}
	AutoPointer(Resource* res, bool Managed = true) { 
		if(!res) {
			mManaged = true;
			Acquire(new Resource());
		}
		else {
			Acquire(res,Managed);
		}
	}
	~AutoPointer() { 
		Unacquire();
	}


	friend Resource &operator *(AutoPointer &p) { 
		if(!p.IsValid()) {
			gLog << "AutoPointer ID[" << p.GetID() << "] Name[" << p.mResourceName << "] Type[" << ResourceTypeToString(p.mResourceType) << "] ERROR: Resource pointer has become null, attempting a dereference of an invalid pointer.\n";
			return gInvalidResource;
		}
		return *p.mResource;
	}
	Resource *operator ->() {
		if(!IsValid()) {
			gLog << "AutoPointer ID[" << GetID() << "] Name[" << mResourceName << "] Type[" << ResourceTypeToString(mResourceType) << "] ERROR: Resource pointer has become null, attempting a dereference of an invalid pointer.\n";
			return &gInvalidResource;
		}
		return mResource;
	}

	AutoPointer &operator =(const AutoPointer &other) {
		Unacquire();
		Acquire(other.mResource);
	}

	void Acquire(Resource *res, bool Managed = true)
	{
		Unacquire();

		mResource = res;
		mResourceName = mResource->GetResourceName();
		mResourceType = mResource->GetResourceType();
		mPointerID = gGUID.Get(PointerGUIDTrack);
		if(gDebugSwitch) {
			PrintProperties() << ": Created.  Adding a RefCount to resource.\n";
		}
		mResource->AddRef();
		mManaged = Managed;

	}

	LoggingInterface &PrintProperties() {
		gLog << "AutoPointer[";
		if(IsManaged())
			gLog << "Managed";
		else
			gLog << "Unmanaged";
		gLog << "] containing ";
		mResource->PrintProperties();
		gLog << " with ID[" << GetID() << "] and ";
		PrintRefCount();
		return gLog;
	}

	inline QuadWord GetID() const { return mPointerID; }
	inline bool IsValid() { return (mResource != NULL); }
	inline bool IsManaged() const { return mManaged; }
	inline Resource *Access() {
		mResource->AddRef();
		return mResource;
	}
};

#endif