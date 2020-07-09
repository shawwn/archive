#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include "Types.h"
#include "LoggingInterface.h"

class Resource
{
private:
	std::string mResourceName;
	ResourceType mResourceType;

	UINT mRefCount;

protected:
	LoggingInterface &PrintBaseResourceProperties() { gLog << "Resource [" << GetResourceName() << "] with type [" << ResourceTypeToString(GetResourceType()) << "]"; return gLog; }

public:
	Resource(const std::string &Name = "Invalid Resource", ResourceType Type = NULLTYPE);
	virtual ~Resource();
	inline std::string GetResourceName() { return mResourceName; }
	inline ResourceType GetResourceType() { return mResourceType; }

	inline void SetResourceName(const std::string &str) { mResourceName = str; }
	inline void SetResourceType(ResourceType t) { mResourceType = t; }

	virtual LoggingInterface &PrintProperties() { return PrintBaseResourceProperties(); }

	inline void AddRef() { mRefCount++; }
	inline void Release() { mRefCount--; }
	inline UINT GetRefCount() const { return mRefCount; }
};

#endif