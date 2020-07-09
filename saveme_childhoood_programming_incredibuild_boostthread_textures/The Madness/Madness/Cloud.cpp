#include "Cloud.h"
#include "LoggingInterface.h"
#include "GameInterface.h"
#include "EffectInterface.h"
#include "TextureInterface.h"

bool CloudParticle::DebugRender(LPDIRECT3DDEVICE9 Device, LPD3DXMESH Sphere)
{
	//static D3DXMATRIX Mat;
	//D3DXMatrixIdentity(&Mat);
	//D3DXMatrixTranslation(&Mat,Position.x,Position.y,Position.z);z
	//Device->SetTransform(D3DTS_WORLD,&Mat);
	//Device->SetFVF(D3DFVF_XYZ | D3DFVF_PSIZE);
	//return(!FAILED(Sphere->DrawSubset(0)));

	return !FAILED(Device->DrawPrimitiveUP(D3DPT_POINTLIST,1,&Position,16));
}


bool Cloud::Initialize(int Width, int Height, int Depth)
{
	/*int i, j, k;
	float fi, fj, fk;
	mNodes = new CloudParticle*** [Width];
	for(i = 0, fi = 0; i < Width; i++, fi++) {
		mNodes[(mIndices[i] = new CloudParticle** [Height];
		for(j = 0, fj = 0; j < Height; j++, fj++) {
			mNodes[(mIndices[i][j] = new CloudParticle* [Depth];
			for(k = 0, fk = 0; k < Depth; k++, fk++) {
				mNodes[(mIndices[i][j][k] = new CloudParticle;
				mNodes[(mIndices[i][j][k])].Position = NxVec3(fi,fj,fk);
				mNodes[(mIndices[i][j][k])].Velocity = NxVec3(0,0,0);
			}
			//mNodes[(mIndices[i][j][0])].WaterVapor = 1.0f;
			if(j > Height-3)
				mNodes[(mIndices[i][Height-2][j])].Temperature = 90.0f;
		}
	}*/

	/*if(!mShader.Load(gD3D.GetDevice(),"Data/Shaders/cloud.fx")) {
		gLog ^"Cloud ERROR: Effect file loading failed.\n";
		return false;
	}*/

	mShaderIndex = gEffects.Load("Data/Shaders/cloud.fx");
	mTextureIndex = gTextures.LoadMultiple("Data/Textures/cloud.jpg",mTextureNames);

	mSize = Width*Height*Depth;

	if(mSize > 4294967296) {
		gLog ^ "Cloud ERROR: Cloud entity too large, indexing will exceed the capacity of an integer.\n";
		mSize = 0;
		return false;
	}

	mWidth = Width;
	mHeight = Height;
	mDepth = Depth;

	mNodes = new CloudParticle[mSize];
	mNext = new CloudParticle[mSize];
	mIndices = new int** [Depth];
	for(int i = 0; i < mDepth; i++) {
		mIndices[i] = new int* [Height];
		for(int j = 0; j < mHeight; j++) {
			mIndices[i][j] = new int [Width];
			for(int k = 0; k < mWidth; k++) {
				mIndices[i][j][k] = I(i,j,k);
				mNodes[(mIndices[i][j][k])].Velocity = NxVec3(0,0,0);
			}
		}
	}
	for(int i = 1 ; i < mDepth-1; i++) {
		for(int j = 1; j < mHeight-1; j++) {
			for(int k = 1; k < mWidth-1; k++) {
				mNodes[(mIndices[i][j][k])].WaterVapor = 50.0f;
				mNodes[(mIndices[i][j][k])].Position = NxVec3(5.0f*i,1.0f*j,5.0f*k);
			}
		}
		
	}

	//mNodes[(mIndices[5][5][5])].WaterVapor = 50.0f;


	//mNodes[(mIndices[5][5][5])].WaterVapor = 50.0f;
	mDiffusionCoeff = 0.9f;
	mPressureCoeff = 0.3f;
	mThermalCoeff = 0.9f;
	mViscosityRatio = 0.4f;
	mBuoyancyCoeff = 1.0f;
	
/*	for(i = 0, fi = 0; fi < Width; fi++, i++) {
		for(j = 0, fj = 0; fj < Height; fj++, j++) {
			for(k = 0, fk = 0; fk < Depth; fk++, k++) {
			}
		}
	}*/

	memcpy(mNext,mNodes,mSize*sizeof(CloudParticle));
	return true;
}

static DWORD asdf = 0;
#include "ClientSettingsInterface.h"

bool Cloud::Update(DWORD TimeInMiliseconds)
{
	NxVec3 VelDiff;
	NxVec3 Grad;

	asdf += TimeInMiliseconds;

	static int count = 0;
	static int i,j,k, ip1,im1,jp1,jm1,kp1,km1;

	srand(TimeInMiliseconds);

	for(i= 0; i < mDepth; i++) 
	{
		for(k = 0; k < mWidth; k++) 
		{
			if(asdf % 20000 < 10000) {
				mNext[(mIndices[i][rand()%16][k])].Temperature = mNodes[(mIndices[i][rand()%16][k])].Temperature + 25.0f/32 * (TimeInMiliseconds/1000.0f);//*(1-abs((i-mDepth/2)/mDepth+(k-mWidth/2)/mWidth));			
					//mNext[(mIndices[i][0][k])].WaterVapor = mNodes[(mIndices[i][0][k])].WaterVapor + 20.0f * (TimeInMiliseconds/1000.0f);
				//mNext[(mIndices[i][14][k])].Velocity.x = mNodes[(mIndices[i][14][k])].Velocity.x + 0.5f * (TimeInMiliseconds/1000.0f);
				//mNext[(mIndices[i][1][k])].Velocity.x = mNodes[(mIndices[i][1][k])].Velocity.x + 0.2f * (TimeInMiliseconds/1000.0f);
				count++;
			}
		}
	}

	memcpy(mNodes,mNext,mSize*sizeof(CloudParticle));
	

	for(i = 1; i < mDepth-1; i++) {
		for(j = 1; j < mHeight-1; j++) {
			for(k = 1; k < mWidth-1; k++) {
				ip1 = (i+1>mDepth-1?i:i+1);		im1 = (i-1<1?1:i-1);
				jp1 = (j+1>mHeight-1?j:j+1);	jm1 = (j-1<1?1:j-1);
				kp1 = (k+1>mWidth-1?k:k+1);		km1 = (k-1<1?1:k-1);
				ip1 = i+1; im1 = i-1;
				jp1 = j+1; jm1 = j-1;
				kp1 = k+1; km1 = k-1;

				
				/*
				if(ip1 >= mDepth-1)		mNodes[(mIndices[ip1][j  ][k  ])].Velocity = mNodes[(mIndices[i][j][k])].Velocity;
				if(jp1 >= mHeight-1)	mNodes[(mIndices[i  ][jp1][k  ])].Velocity = mNodes[(mIndices[i][j][k])].Velocity;
				if(kp1 >= mWidth-1)		mNodes[(mIndices[i  ][j  ][kp1])].Velocity = mNodes[(mIndices[i][j][k])].Velocity;
				if(im1 < 1)	mNodes[(mIndices[im1][j  ][k  ])].Velocity = mNodes[(mIndices[i][j][k])].Velocity;
				if(jm1 < 1)	mNodes[(mIndices[i  ][jm1][k  ])].Velocity = mNodes[(mIndices[i][j][k])].Velocity;
				if(km1 < 1)	mNodes[(mIndices[i  ][j  ][km1])].Velocity = mNodes[(mIndices[i][j][k])].Velocity;*/


				// You can ignore the if statement block and the next line, this is just for adding water vapor / wind to the cloud
	
				

				mNext[(mIndices[i][j][k])].Position = mNodes[(mIndices[i][j][k])].Position + mNodes[(mIndices[i][j][k])].Velocity * (TimeInMiliseconds/1000.0f);

				////STEP1//// Viscosity and Pressure Effect
				VelDiff = (		mNodes[(mIndices[ip1][j  ][k  ])].Velocity + mNodes[(mIndices[im1][j  ][k  ])].Velocity + 
								mNodes[(mIndices[i  ][jp1][k  ])].Velocity + mNodes[(mIndices[i  ][jm1][k  ])].Velocity +
								mNodes[(mIndices[i  ][j  ][kp1])].Velocity + mNodes[(mIndices[i  ][j  ][km1])].Velocity -
								(6.0f*mNodes[(mIndices[i][j][k])].Velocity)) / 6.0f;

				Grad.x = ( mNodes[(mIndices[ip1][j  ][k  ])].Velocity.x + mNodes[(mIndices[im1][j  ][k  ])].Velocity.x - 2*mNodes[(mIndices[i][j][k])].Velocity.x) / 2.0f + 
						 ( mNodes[(mIndices[ip1][jp1][k  ])].Velocity.y + mNodes[(mIndices[im1][jm1][k  ])].Velocity.y - mNodes[(mIndices[im1][jp1][k  ])].Velocity.y - mNodes[(mIndices[ip1][jm1][k  ])].Velocity.y + 
						   mNodes[(mIndices[ip1][j  ][km1])].Velocity.z + mNodes[(mIndices[im1][j  ][km1])].Velocity.z - mNodes[(mIndices[im1][j  ][kp1])].Velocity.z - mNodes[(mIndices[ip1][j  ][km1])].Velocity.z) / 4.0f;

				Grad.y = ( mNodes[(mIndices[ip1][j  ][k  ])].Velocity.y + mNodes[(mIndices[im1][j  ][k  ])].Velocity.y - 2*mNodes[(mIndices[i][j][k])].Velocity.y) / 2.0f + 
						 ( mNodes[(mIndices[ip1][jp1][k  ])].Velocity.z + mNodes[(mIndices[im1][jm1][k  ])].Velocity.z - mNodes[(mIndices[im1][jp1][k  ])].Velocity.z - mNodes[(mIndices[ip1][jm1][k  ])].Velocity.z + 
						   mNodes[(mIndices[ip1][j  ][km1])].Velocity.x + mNodes[(mIndices[im1][j  ][km1])].Velocity.x - mNodes[(mIndices[im1][j  ][kp1])].Velocity.x - mNodes[(mIndices[ip1][j  ][km1])].Velocity.x) / 4.0f;

				Grad.z = ( mNodes[(mIndices[ip1][j  ][k  ])].Velocity.z + mNodes[(mIndices[im1][j  ][k  ])].Velocity.z - 2*mNodes[(mIndices[i][j][k])].Velocity.z) / 2.0f + 
						 ( mNodes[(mIndices[ip1][jp1][k  ])].Velocity.x + mNodes[(mIndices[im1][jm1][k  ])].Velocity.x - mNodes[(mIndices[im1][jp1][k  ])].Velocity.x - mNodes[(mIndices[ip1][jm1][k  ])].Velocity.x + 
						   mNodes[(mIndices[ip1][j  ][km1])].Velocity.y + mNodes[(mIndices[im1][j  ][km1])].Velocity.y - mNodes[(mIndices[im1][j  ][kp1])].Velocity.y - mNodes[(mIndices[ip1][j  ][km1])].Velocity.y) / 4.0f;

				if(Grad.magnitude() > 1.0f) {
					Grad.normalize(); Grad *= mPressureCoeff;
				}
				
				mNext[(mIndices[i][j][k])].Velocity = mNodes[(mIndices[i][j][k])].Velocity + (mViscosityRatio * VelDiff + mPressureCoeff * Grad) * (NxReal)(TimeInMiliseconds/1000.0f);


			}
		}
	}
	memcpy(mNodes,mNext,mSize*sizeof(CloudParticle));

	for(i = 1; i < mDepth-1; i++) {
		for(j = 1; j < mHeight-1; j++) {
			for(k = 1; k < mWidth-1; k++) {
				ip1 = i+1; im1 = i-1;
				jp1 = j+1; jm1 = j-1;
				kp1 = k+1; km1 = k-1;

				/*
				if(ip1 > mDepth-1)	mNodes[(mIndices[ip1][j  ][k  ])].Temperature = mNodes[(mIndices[i][j][k])].Temperature / 1.1f;
				if(jp1 > mHeight-1) mNodes[(mIndices[i  ][jp1][k  ])].Temperature = mNodes[(mIndices[i][j][k])].Temperature / 1.1f;
				if(kp1 > mWidth-1)	mNodes[(mIndices[i  ][j  ][kp1])].Temperature = mNodes[(mIndices[i][j][k])].Temperature / 1.1f;
				if(im1 < 1)	mNodes[(mIndices[im1][j  ][k  ])].Temperature = mNodes[(mIndices[i][j][k])].Temperature / 1.1f;
				if(jm1 < 1) mNodes[(mIndices[i  ][jm1][k  ])].Temperature = mNodes[(mIndices[i][j][k])].Temperature / 1.1f;
				if(km1 < 1)	mNodes[(mIndices[i  ][j  ][km1])].Temperature = mNodes[(mIndices[i][j][k])].Temperature / 1.1f;*/

				////STEP2//// Advection
				
				////STEP3//// Diffusion of Water Vapor
				mNext[(mIndices[i][j][k])].WaterVapor = mNodes[(mIndices[i][j][k])].WaterVapor + (mDiffusionCoeff * (
					mNodes[(mIndices[ip1][j  ][k  ])].WaterVapor + mNodes[(mIndices[im1][j  ][k  ])].WaterVapor + 
					mNodes[(mIndices[i  ][jp1][k  ])].WaterVapor + mNodes[(mIndices[i  ][jm1][k  ])].WaterVapor + 
					mNodes[(mIndices[i  ][j  ][kp1])].WaterVapor + mNodes[(mIndices[i  ][j  ][km1])].WaterVapor -
					(6.0f*mNodes[(mIndices[i][j][k])].WaterVapor)) / 6.0f) * TimeInMiliseconds/1000.0f;
				if(mNodes[(mIndices[i][j][k])].WaterVapor < 0) { mNodes[(mIndices[i][j][k])].WaterVapor = 0; }


				////STEP4//// Thermal Diffusion
				mNext[(mIndices[i][j][k])].Temperature = mNodes[(mIndices[i][j][k])].Temperature + mThermalCoeff * (
					mNodes[(mIndices[ip1][j  ][k  ])].Temperature + mNodes[(mIndices[im1][j  ][k  ])].Temperature + 
					mNodes[(mIndices[i  ][jp1][k  ])].Temperature + mNodes[(mIndices[i  ][jm1][k  ])].Temperature + 
					mNodes[(mIndices[i  ][j  ][kp1])].Temperature + mNodes[(mIndices[i  ][j  ][km1])].Temperature -
					(6*mNodes[(mIndices[i][j][k])].Temperature)) / 6.0f;


			}
		}
	}
	memcpy(mNodes,mNext,mSize*sizeof(CloudParticle));	

	for(i = 1; i < mDepth-1; i++) {
		for(j = 1; j < mHeight-1; j++) {
			for(k = 1; k < mWidth-1; k++) {
				ip1 = i+1; im1 = i-1;
				jp1 = j+1; jm1 = j-1;
				kp1 = k+1; km1 = k-1;


				////STEP5//// Buoyancy
				mNext[(mIndices[i][j][k])].Velocity.y = mNodes[(mIndices[i][j][k])].Velocity.y + (mBuoyancyCoeff/6.0f)*(6.0f*mNodes[(mIndices[i][j][k])].Temperature - 
					mNodes[(mIndices[ip1][j  ][k  ])].Temperature - mNodes[(mIndices[im1][j  ][k  ])].Temperature - 
					mNodes[(mIndices[i  ][jp1][k  ])].Temperature - mNodes[(mIndices[i  ][jm1][k  ])].Temperature - 
					mNodes[(mIndices[i  ][j  ][kp1])].Temperature - mNodes[(mIndices[i  ][j  ][km1])].Temperature);


			}
		}
	}
				
	memcpy(mNodes,mNext,mSize*sizeof(CloudParticle));

	return true;
}

bool Cloud::DebugRender(LPDIRECT3DDEVICE9 Device)
{
	if(!mSphere) {
		if(FAILED(D3DXCreateBox(Device,0.5f,0.5f,0.5f,&mSphere,NULL)))
			return false;
	}

	static Vertex v(1,0,1);
//	if(FAILED(Device->SetVertexDeclaration(v.GetVertexDeclaration(Device))))
//		gLog ^ "gwarg\n";

	Device->SetRenderState(D3DRS_POINTSPRITEENABLE,true);
	Device->SetRenderState(D3DRS_POINTSCALEENABLE,true);
	//Device->SetRenderState(D3DRS_POINTSCALE_A,0);
	//Device->SetRenderState(D3DRS_POINTSCALE_B,0);
	//Device->SetRenderState(D3DRS_POINTSCALE_C,0);
	//if(FAILED(Device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX0)))
		//gLog ^ "Blah\n";

	UINT Passes = 0;
	if(!gEffects.FindEffectInterface(mShaderIndex)->Begin(&Passes,0)) 
		return false;

	gEffects.FindEffect(mShaderIndex)->SetMatrix("matWVP",&gEngine.GetMatVP());
	gEffects.FindEffect(mShaderIndex)->SetTexture("Decal",gTextures.Find(mTextureIndex)->Texture);
	if(FAILED(gEffects.FindEffectInterface(mShaderIndex)->BeginPass(0)))
		return false;
	if(FAILED(Device->DrawPrimitiveUP(D3DPT_POINTLIST,mSize,mNodes,sizeof(CloudParticle)))) {
		gLog ^ "Cloud ERROR: Render failure, DrawPrimitiveUP() failed.\n";
		return false;
	}
	gEffects.FindEffect(mShaderIndex)->EndPass();
	gEffects.FindEffect(mShaderIndex)->End();
	/*for(int i = 0; i < mWidth; i++) {
		for(int j = 0; j < mHeight; j++) {
			for(int k = 0; k < mDepth; k++) {
				if(mNodes[(mIndices[i][j][k])].WaterVapor > 0.0f)
					mNodes[(mIndices[i][j][k])].DebugRender(Device,mSphere);
			}
		}
	}*/

	return true;
}