#ifndef VERTEXDECLARATIONINTERFACE_H
#define VERTEXDECLARATIONINTERFACE_H

#include <vector>
#include <d3dx9.h>
#include "Macros.h"

class VDECLDATA
{
public:
	VDECLDATA() : Declaration(NULL), Elements(NULL) { }
	~VDECLDATA() { SAFE_RELEASE(Declaration); SAFE_DELETE_ARRAY(Elements); }
	LPDIRECT3DVERTEXDECLARATION9 Declaration;
	LPD3DVERTEXELEMENT9 Elements;
};
typedef VDECLDATA* LPVDECLDATA;

class VertexDeclarationInterface
{
private:
	std::vector<LPVDECLDATA> mDeclarations;
	LPDIRECT3DDEVICE9 mUseDevice;

	inline UINT ElemSize(LPD3DVERTEXELEMENT9 Elements) {
		for(UINT i = 0; Elements[i].Stream != 0xFF; i++) {
		}
		return i+1;
	}

	LPD3DVERTEXELEMENT9 mFillElements;
	UINT mFillCount;

	inline bool Valid(UINT Index) { return(Index >= 0 && Index < NumDeclarations() && mDeclarations[Index]->Declaration); }
public:
	VertexDeclarationInterface() : mUseDevice(NULL), mFillElements(NULL), mFillCount(-1) { }
	~VertexDeclarationInterface() {
		for(UINT i = 0; i < NumDeclarations(); i++) {
			SAFE_DELETE(mDeclarations[i]); }
	}

	void OnResetDevice(LPDIRECT3DDEVICE9 Device);
	void OnDestroyDevice();

	UINT CreateDeclaration(LPD3DVERTEXELEMENT9 Elements);
	bool OverwriteDeclaration(UINT Index, LPD3DVERTEXELEMENT9 Elements);
	bool SetDeclaration(UINT Index);
	bool ReleaseDeclaration(UINT Index);
	LPDIRECT3DVERTEXDECLARATION9 AccessDeclaration(UINT Index);

	VertexDeclarationInterface &Start(LPD3DVERTEXELEMENT9 Elements);
	VertexDeclarationInterface &Fill(BYTE Stream, BYTE Offset, BYTE Type, BYTE Method, BYTE Usage, BYTE UsageIndex);
	void End();

	inline UINT NumDeclarations() { return (UINT)mDeclarations.size(); }
};
extern VertexDeclarationInterface gDeclarations;

#endif