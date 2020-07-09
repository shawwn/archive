#include "Resource.h"
#include "Globals.h"

Resource::Resource(const std::string &Name, ResourceType Type) : mResourceName(Name), mResourceType(Type), mRefCount(0)
{
	if(gDebugSwitch) {
		PrintProperties() << ": Constructed.\n";	
	}
}

Resource::~Resource()
{
	if(gDebugSwitch) {
		PrintProperties() << ": Destructed.\n";
	}
}

