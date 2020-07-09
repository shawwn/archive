#ifndef CLOUD_H
#define CLOUD_H

#include <vector>
#include <string>
#include "NovodexInterface.h"
#include "Macros.h"
#include "Effect.h"
#include "Particle.h"

class CloudParticle : public Particle
{
public:
	CloudParticle() : WaterVapor(0), Temperature(0) { }
	float WaterVapor;
	float Temperature;

	bool DebugRender(LPDIRECT3DDEVICE9 Device, LPD3DXMESH Sphere); 
};
/*
class CloudEmitter : public ParticleEmitter<CloudParticle>
{
private:
	void ParticleSpawn(T *part) { }
	void ParticleDeath(T *part) { }
	void KillParticle(T *part) { }
	bool ParticleUpdate(float fDeltaTimeInSeconds, T *part) { }
	bool ParticleIsDead(const T *part) { }

	bool EmitterUpdate(float fDeltaTimeInSeconds) { }
public:
};*/

class Cloud
{
private:
	float mViscosityRatio;
	float mPressureCoeff;
	float mDiffusionCoeff;
	float mThermalCoeff;
	float mBuoyancyCoeff;

	CloudParticle *mNodes;
	CloudParticle *mNext;
	int ***mIndices;

	int mSize;
	int mWidth;
	int mHeight;
	int mDepth;

	LPDIRECT3DVERTEXDECLARATION9 mVertDec;
	LPD3DXMESH mSphere;

	UINT mShaderIndex;
	UINT mTextureIndex;

	std::vector<std::string> mTextureNames;

	inline int I(int i, int j, int k) { return i*mWidth*mHeight+j*mWidth+k; }

public:
	Cloud() : mSphere(NULL), mVertDec(NULL), mNodes(NULL), mWidth(0), mHeight(0), mDepth(0), mSize(0), mViscosityRatio(1), mPressureCoeff(1), mDiffusionCoeff(1), mThermalCoeff(1), mBuoyancyCoeff(1) { }
	//~Cloud() { SAFE_RELEASE(mSphere); for(int i = 0; i < mWidth; i++) { for(int j = 0; j < mHeight; j++) { for(int k = 0; k < mDepth; k++) { SAFE_DELETE(mNodes[i][j][k]); } SAFE_DELETE_ARRAY(mNodes[i][j]); } SAFE_DELETE_ARRAY(mNodes[i]); } SAFE_DELETE_ARRAY(mNodes) }
	~Cloud() { SAFE_DELETE_ARRAY(mNodes); SAFE_DELETE_ARRAY(mNext); for(int i = 0; i < mWidth; i++) { for(int j = 0; j < mHeight ;j++) { SAFE_DELETE_ARRAY(mIndices[i][j]); } SAFE_DELETE_ARRAY(mIndices[i]); } SAFE_DELETE_ARRAY(mIndices); }

	bool Initialize(int Width, int Height, int Depth);
	bool Update(DWORD TimeInMiliseconds);

	bool DebugRender(LPDIRECT3DDEVICE9 Device);

};

#endif

	/*for(int i = 0; i < mWidth-1; i++) {
		for(int j = 1; j < mHeight-1; j++) {
			for(int k = 1; k < mDepth-1; k++) {

				for(int x = i
				
				Index = I(i,j,k);

				mNodes[i][j][k]->Position += mNodes[i][j][k]->Velocity * (TimeInMiliseconds/1000.0f);

				////STEP1//// Viscosity and Pressure Effect
				VelDiff = (		mNodes[i+1][j  ][k  ]->Velocity + mNodes[i-1][j  ][k  ]->Velocity + 
								mNodes[i  ][j+1][k  ]->Velocity + mNodes[i  ][j-1][k  ]->Velocity +
								mNodes[i  ][j  ][k+1]->Velocity + mNodes[i  ][j  ][k-1]->Velocity -
								(6*mNodes[i][j][k]->Velocity)) / 6.0f;

				Grad.x = ( mNodes[i+1][j  ][k  ]->Velocity.x + mNodes[i-1][j  ][k  ]->Velocity.x - 2*mNodes[i][j][k]->Velocity.x) / 2.0f + 
						 ( mNodes[i+1][j+1][k  ]->Velocity.y + mNodes[i-1][j-1][k  ]->Velocity.y - mNodes[i-1][j+1][k  ]->Velocity.y - mNodes[i+1][j-1][k  ]->Velocity.y + 
						   mNodes[i+1][j  ][k-1]->Velocity.z + mNodes[i-1][j  ][k-1]->Velocity.z - mNodes[i-1][j  ][k+1]->Velocity.z - mNodes[i+1][j  ][k-1]->Velocity.z) / 4.0f;

				Grad.y = ( mNodes[i+1][j  ][k  ]->Velocity.y + mNodes[i-1][j  ][k  ]->Velocity.y - 2*mNodes[i][j][k]->Velocity.y) / 2.0f + 
						 ( mNodes[i+1][j+1][k  ]->Velocity.z + mNodes[i-1][j-1][k  ]->Velocity.z - mNodes[i-1][j+1][k  ]->Velocity.z - mNodes[i+1][j-1][k  ]->Velocity.z + 
						   mNodes[i+1][j  ][k-1]->Velocity.x + mNodes[i-1][j  ][k-1]->Velocity.x - mNodes[i-1][j  ][k+1]->Velocity.x - mNodes[i+1][j  ][k-1]->Velocity.x) / 4.0f;

				Grad.z = ( mNodes[i+1][j  ][k  ]->Velocity.z + mNodes[i-1][j  ][k  ]->Velocity.z - 2*mNodes[i][j][k]->Velocity.z) / 2.0f + 
						 ( mNodes[i+1][j+1][k  ]->Velocity.x + mNodes[i-1][j-1][k  ]->Velocity.x - mNodes[i-1][j+1][k  ]->Velocity.x - mNodes[i+1][j-1][k  ]->Velocity.x + 
						   mNodes[i+1][j  ][k-1]->Velocity.y + mNodes[i-1][j  ][k-1]->Velocity.y - mNodes[i-1][j  ][k+1]->Velocity.y - mNodes[i+1][j  ][k-1]->Velocity.y) / 4.0f;
				
				mNodes[i][j][k]->Velocity = mNodes[i][j][k]->Velocity + mViscosityRatio * VelDiff + mPressureCoeff * Grad;

				////STEP2//// Advection
				
				////STEP3//// Diffusion of Water Vapor
				mNodes[i][j][k]->WaterVapor = mNodes[i][j][k]->WaterVapor + mDiffusionCoeff * (
					mNodes[i+1][j  ][k  ]->WaterVapor + mNodes[i-1][j  ][k  ]->WaterVapor + 
					mNodes[i  ][j+1][k  ]->WaterVapor + mNodes[i  ][j-1][k  ]->WaterVapor + 
					mNodes[i  ][j  ][k+1]->WaterVapor + mNodes[i  ][j  ][k-1]->WaterVapor -
					(6*mNodes[i][j][k]->WaterVapor)) / 6.0f;

				////STEP4//// Thermal Diffusion
				mNodes[i][j][k]->Temperature = mNodes[i][j][k]->Temperature + mThermalCoeff * (
					mNodes[i+1][j  ][k  ]->Temperature + mNodes[i-1][j  ][k  ]->Temperature + 
					mNodes[i  ][j+1][k  ]->Temperature + mNodes[i  ][j-1][k  ]->Temperature + 
					mNodes[i  ][j  ][k+1]->Temperature + mNodes[i  ][j  ][k-1]->Temperature -
					(6*mNodes[i][j][k]->Temperature)) / 6.0f;

				////STEP5//// Buoyancy
				mNodes[i][j][k]->Velocity.y = mNodes[i][j][k]->Velocity.y + (mBuoyancyCoeff/4.0f)*(4*mNodes[i][j][k]->Temperature - 
					mNodes[i+1][j  ][k  ]->Temperature - mNodes[i-1][j  ][k  ]->Temperature - 
					mNodes[i  ][j+1][k  ]->Temperature - mNodes[i  ][j-1][k  ]->Temperature);


			}
		}*/