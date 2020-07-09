#include "VertexDeclarationInterface.h"
#include "Direct3DInterface.h"

VertexDeclarationInterface gDeclarations;

void VertexDeclarationInterface::OnResetDevice(LPDIRECT3DDEVICE9 Device)
{
	if(!Device)
		return;
	HRESULT hr;
	mUseDevice = Device;
	for(UINT i = 0; i < NumDeclarations(); i++) {
		LPVDECLDATA Data = mDeclarations[i];
		if(!Data->Declaration) {
			if(FAILED(hr = mUseDevice->CreateVertexDeclaration(Data->Elements,&Data->Declaration)))
			{
				LogDxErr(hr,"VertexDeclarationInterface::OnResetDevice");
			}
		}
	}
}

void VertexDeclarationInterface::OnDestroyDevice()
{
	for(UINT i = 0; i < NumDeclarations(); i++) 
		SAFE_RELEASE(mDeclarations[i]->Declaration);
}


UINT VertexDeclarationInterface::CreateDeclaration(LPD3DVERTEXELEMENT9 Elements)
{
	if(!mUseDevice)
		return -1;
	LPVDECLDATA Data = new VDECLDATA;

	UINT NumElements = ElemSize(Elements);
	Data->Elements = new D3DVERTEXELEMENT9[NumElements];
	memcpy(Data->Elements,Elements,sizeof(D3DVERTEXELEMENT9)*NumElements);

	HRESULT hr;
	if(FAILED(hr = mUseDevice->CreateVertexDeclaration(Data->Elements,&Data->Declaration))) {
		LogDxErr(hr,"VertexDeclarationInterface::CreateDeclaration");
		SAFE_DELETE(Data);
		return -1;
	}

	for(UINT i = 0; i < NumDeclarations(); i++) {
		if(!mDeclarations[i])
		{
			mDeclarations[i] = Data;
			return i;
		}
	}

	mDeclarations.push_back(Data);
	return NumDeclarations()-1;
}

bool VertexDeclarationInterface::OverwriteDeclaration(UINT Index, LPD3DVERTEXELEMENT9 Elements)
{
	if(!Valid(Index) || !mUseDevice)
		return false;

	LPVDECLDATA Data = mDeclarations[Index];

	UINT NumElements = ElemSize(Elements);
	SAFE_DELETE_ARRAY(Data->Elements);
	Data->Elements = new D3DVERTEXELEMENT9[NumElements];
	memcpy(Data->Elements,Elements,sizeof(D3DVERTEXELEMENT9)*NumElements);

	HRESULT hr;
	if(FAILED(hr = mUseDevice->CreateVertexDeclaration(Data->Elements,&Data->Declaration))) {
		LogDxErr(hr,"VertexDeclarationInterface::OverwriteDeclaration");
		SAFE_DELETE(Data);
		return false;
	}

	return true;
}

bool VertexDeclarationInterface::SetDeclaration(UINT Index)
{
	if(!Valid(Index) || !mUseDevice)
		return false;
	HRESULT hr;
	if(FAILED(hr = mUseDevice->SetVertexDeclaration(mDeclarations[Index]->Declaration)))
	{
		LogDxErr(hr,"VertexDeclarationInterface::SetDelcaration(SetVertexDeclaration)");
		return false;
	}
	return true;
}

bool VertexDeclarationInterface::ReleaseDeclaration(UINT Index)
{
	if(!Valid(Index) || !mUseDevice)
		return false;
	SAFE_DELETE(mDeclarations[Index]);
	return true;
}

LPDIRECT3DVERTEXDECLARATION9 VertexDeclarationInterface::AccessDeclaration(UINT Index)
{
	if(!Valid(Index))
		return NULL;
	return mDeclarations[Index]->Declaration;
}

VertexDeclarationInterface &VertexDeclarationInterface::Start(LPD3DVERTEXELEMENT9 Elements)
{
	mFillElements = Elements;
	mFillCount = 0;

	return *this;
}

VertexDeclarationInterface &VertexDeclarationInterface::Fill(BYTE Stream, BYTE Offset, BYTE Type, BYTE Method, BYTE Usage, BYTE UsageIndex)
{
	if(!mFillElements || mFillCount == -1)
		return *this;

	mFillElements[mFillCount].Stream = Stream;
	mFillElements[mFillCount].Offset = Offset;
	mFillElements[mFillCount].Type = Type;
	mFillElements[mFillCount].Method = Method;
	mFillElements[mFillCount].Usage = Usage;
	mFillElements[mFillCount].UsageIndex = UsageIndex;

	mFillCount++;

	return *this;
}

void VertexDeclarationInterface::End()
{
	mFillElements[mFillCount].Stream = 0xFF;
	mFillElements[mFillCount].Offset = 0;
	mFillElements[mFillCount].Type = D3DDECLTYPE_UNUSED;
	mFillElements[mFillCount].Method = 0;
	mFillElements[mFillCount].Usage = 0;
	mFillElements[mFillCount].UsageIndex = 0;

	mFillElements = NULL;
	mFillCount = -1;
}