#include "XMeshInterface.h"
#include "Macros.h"
#include "XMesh.h"

XMeshInterface gMeshes;

XMeshInterface::~XMeshInterface()
{

}

UINT XMeshInterface::Load(const std::string &Filename)
{
	XBaseMesh *mesh = new XBaseMesh;
	if(!mesh->Load(Filename)) {
		SAFE_DELETE(mesh);
		return -1;
	}

	mMeshes.push_back(mesh);
	return Size()-1;
}

void XMeshInterface::GetNewInstance(UINT Index, XMesh **Fill)
{
	if(Index >= Size())
		return;

	mMeshes[Index]->CreateNewInstance(Fill);
}