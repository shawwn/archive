#include "BillboardInterface.h"
#include "LoggingInterface.h"
#include "Direct3DInterface.h"


bool BillboardInterface::Make(LPDIRECT3DDEVICE9 Device, const char *VertexPositionData, UINT Count, UINT Stride, const NxVec3 &ViewUp, const NxVec3 &ViewRight, short Width, short Height)
{
	UINT CurrentBuffer = 0;
	mBillboardCount = 0;
	while(Count  > 0) {
		LPDIRECT3DVERTEXBUFFER9 Buffer;
		
	
		if(CurrentBuffer == BufferCount() || !mVB[CurrentBuffer]) {
			if(!Device) { 
				gLog ^ "BillboardInterface ERROR: Device is null.\n";
				return false;
			}

			LPDIRECT3DVERTEXBUFFER9 VB = NULL;
			Device->CreateVertexBuffer(mBillboardsPerBuffer*sizeof(Billboard),D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,D3DFVF_XYZ | D3DFVF_TEX1,D3DPOOL_DEFAULT,&VB,NULL);
			if(!VB) {
				gLog ^ "BillboardInterface ERROR: VertexBuffer could not be created with size [" ^ mBillboardsPerBuffer*4*sizeof(BillboardVertex) ^ "].\n";
				return false;
			}
			mVB.push_back(VB);
		}
		Buffer = mVB[CurrentBuffer];
		if(!Buffer) {
			Device->CreateVertexBuffer(mBillboardsPerBuffer*sizeof(Billboard),D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,D3DFVF_XYZ | D3DFVF_TEX1,D3DPOOL_DEFAULT,&Buffer,NULL);
			mVB[CurrentBuffer] = Buffer;
		}


		char *ptr = NULL;
		if(FAILED(Buffer->Lock(0,min(Count,mBillboardsPerBuffer)*sizeof(Billboard),(void **)&ptr, 0))) {
			gLog ^ "BillboardInterface ERROR: Could not lock vertex buffer [" ^ CurrentBuffer ^ "].\n";
			return false;
		}

		NxVec3 Pos;
		Billboard b;
		for(UINT i = 0; i < min(Count,mBillboardsPerBuffer); i++) {
			memcpy(&Pos,Stride*mBillboardCount + VertexPositionData + Stride*i,sizeof(NxVec3));
			b.Make(Pos,ViewUp,ViewRight,Width,Height);
			memcpy(ptr,&b,sizeof(Billboard));
			//VertexPositionData += Stride;
			ptr += sizeof(Billboard);
		}
		Buffer->Unlock();

		mBillboardCount += min(Count,mBillboardsPerBuffer);
		Count -= min(Count,mBillboardsPerBuffer);

		CurrentBuffer++;
	}
	UINT Stopped = CurrentBuffer;
	while(CurrentBuffer < BufferCount()) {
		SAFE_RELEASE(mVB[CurrentBuffer]);
		CurrentBuffer++;
	}

	return true;
}

bool BillboardInterface::Render(LPDIRECT3DDEVICE9 Device)
{

	Device->SetTransform(D3DTS_WORLD,&gMatIdent);
	Device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	Device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
	Device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
	Device->SetRenderState(D3DRS_ZWRITEENABLE,false);
	UINT NumBill = mBillboardCount;
	HRESULT hr;
	for(UINT i = 0; i < BufferCount(); i++)
	{
		if(mVB[i])
		{
			Device->SetStreamSource(0,mVB[i],0,sizeof(BillboardVertex));
			if(FAILED(hr = Device->DrawPrimitive(D3DPT_TRIANGLELIST,0,min(NumBill,mBillboardsPerBuffer)*2))) {
				LogDxErr(hr,"BillboardInterface::Render::DrawPrimitive");
			}
			NumBill -= mBillboardsPerBuffer;
		}
	}
	return true;
}

void BillboardInterface::Destroy()
{
	gLog << "BillboardInterface: Destroying.\n";
	for(UINT i = 0; i < BufferCount(); i++) {
		SAFE_RELEASE(mVB[i]);
	}
	mVB.clear();
}