#ifndef XMESHINTERFACE_H
#define XMESHINTERFACE_H

#include "XMesh.h"
#include <string>
#include <vector>
#include "Types.h"

class XMeshInterface
{
private:
	std::vector<XBaseMesh *> mMeshes;
public:
	XMeshInterface() { }
	~XMeshInterface();
	UINT Load(const std::string &Filename);
    void GetNewInstance(UINT Index, XMesh **Fill);

	inline UINT Size() const { return (UINT)mMeshes.size(); }
};

extern XMeshInterface gMeshes;

#endif