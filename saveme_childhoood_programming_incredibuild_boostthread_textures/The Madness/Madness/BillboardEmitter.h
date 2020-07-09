#ifndef BILLBOARDEMITTER_H
#define BILLBOARDEMITTER_H

#include "BillboardInterface.h"
#include "LoggingInterface.h"
#include "TextureInterface.h"


template<class T> 
class BillboardEmitter : public ParticleEmitter<T>
{
protected:
	std::string mStats;
	BillboardInterface mBillboards;
	BlockManager *mPositions;
	int mNumPositions;

	virtual bool EmitterRender(float fDeltaTimeInSeconds, LPDIRECT3DDEVICE9 Device, const D3DXMATRIX &MatView, const NxVec3 &VecView) { return true; }
public:
	BillboardEmitter() { mPositions = new BlockManager(sizeof(NxVec3),1024); mNumPositions = 1024; }
	~BillboardEmitter() { SAFE_DELETE(mPositions); }

	const std::string &Stats()
	{ 
		int NumBlocks = 0, NumAllocated = 0, NumFragments = 0, NumFree = 0, NumTotal = 0;
		mMemory->GetStats(NumBlocks,NumAllocated,NumFragments,NumFree,NumTotal);
		gLog << "NumBlocks[" << NumBlocks << "] NumAllocated[" << NumAllocated << "] NumFragments[" << NumFragments << "] NumFree[" << NumFree << "] NumTotal[" << NumTotal << "]";
		mStats = gLog.GetStream().str();
		gLog.GetStream().str(std::string());
		return mStats;
	}

	virtual void LoadEffect() { }

	virtual bool StartEffect() { return false; }
	virtual bool EndEffect() { return false; }

	bool Render(float fDeltaTimeInSeconds, LPDIRECT3DDEVICE9 Device, const D3DXMATRIX &MatView, const NxVec3 &VecView, UINT TextureIndex, UINT Width = 10, UINT Height = 10) {
		Iterator it;
		for(it = Begin(); it != End(); it++) {
			Particle *p = *it;
			p->DistanceFromViewer = VecView.distance(p->Position);
		}

		mActiveParticles.sort(ParticlePred);
		mPositions->Reset();
		size_t size = mActiveParticles.size();
		
		while((int)mActiveParticles.size() > mNumPositions) {
			SAFE_DELETE(mPositions);
			mNumPositions = mNumPositions * 2;
			mPositions = new BlockManager(sizeof(NxVec3),mNumPositions);
		}

		bool HasEffect = StartEffect();
		if(mActiveParticles.size())
		{
			NxVec3 *Positions = mPositions->Construct<NxVec3>((unsigned short)mActiveParticles.size());

			UINT i;
			for(it = mActiveParticles.begin(), i = 0; it != mActiveParticles.end(); it++, i++) {
				Positions[i] = (*it)->Position;
			}

			mBillboards.Make(Device,(char *)Positions,(UINT)mActiveParticles.size(),sizeof(NxVec3),NxVec3(MatView._12,MatView._22,MatView._32),NxVec3(MatView._11,MatView._21,MatView._31),Width,Height);
			if(!HasEffect)
				Device->SetTexture(0,gTextures.Find(TextureIndex)->Texture);

			mBillboards.Render(Device);
		}
		EmitterRender(fDeltaTimeInSeconds, Device,MatView,VecView);
		EndEffect();
		return true;
	}
	void Reset() { mBillboards.Destroy(); }
};



#endif