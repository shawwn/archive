#ifndef GLOWEMITTER_H
#define GLOWEMITTER_H

#include "Particle.h"
#include <d3dx9.h>
#include <string>
#include "LightingInterface.h"
#include "BillboardInterface.h"
#include "BillboardEmitter.h"


class MyParticle : public Particle
{
public:
	float mLife;
	Light *l;
	MyParticle() : mLife(10000), l(NULL)  { Position = NxVec3(0,0,0); Velocity = NxVec3(20,0,0);  }
};

class GlowEmitter : public BillboardEmitter<MyParticle>
{
private:
	std::string mStats;
	BillboardInterface mBillboards;
	float Tick;
	BlockManager *mPositions;
	int mNumPositions;

	//void Contacted(LPNXACTOR Particle, LPNXACTOR ContactedWIth, const NxVec3 &Where)
	void Contacted(LPNXSCENE Scene, Particle *Part, Particle *With, const NxVec3 &Where);
	void Contacted(LPNXSCENE Scene, Particle *Part, LPNXACTOR NotParticle, const NxVec3 &Where);
	void ParticleSpawn(MyParticle *part);
	void ParticleDeath(MyParticle *part);
	bool ParticleUpdate(float fDeltaTimeInSeconds, MyParticle *part);
	bool ParticleIsDead(const MyParticle *part);

	bool EmitterInitialize();
	bool EmitterUpdate(float fDeltaTimeInSecondse);
	void EmitterDestroy();

	D3DXMATRIX mTranslation;
public:
	GlowEmitter() { mPositions = new BlockManager(sizeof(NxVec3),1024); mNumPositions = 1024; mPosition = NxVec3(0,200,0); Tick = 0; }

	const std::string &Stats();
	//bool Render(float fDeltaTimeInSeconds, LPDIRECT3DDEVICE9 Device, const D3DXMATRIX &MatView, const NxVec3 &VecView);
};

#endif