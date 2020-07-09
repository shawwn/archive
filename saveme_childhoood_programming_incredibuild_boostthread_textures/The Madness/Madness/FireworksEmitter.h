#ifndef FIREWORKSEMITTER_H
#define FIREWORKSEMITTER_H

#include "Particle.h"
#include <d3dx9.h>
#include <string>
#include "LightingInterface.h"
#include "BillboardEmitter.h"
#include "LightingInterface.h"

class LightParticle : public Particle
{
public:
	Light *l;
	LightParticle() : l(NULL) { }
};


class FireworksShell : public LightParticle
{
public:
	float Life;
};

class FireworksParticle : public LightParticle
{
public:
	D3DXVECTOR3 Color;
	float Life;
};

class FireworksParticleEmitter : public BillboardEmitter<FireworksParticle>
{
private:
	Light Avg;
	UINT mAlphaTextureIndex;
	UINT mDecalTextureIndex;
private:
	//void Contacted(LPNXACTOR Particle, LPNXACTOR ContactedWIth, const NxVec3 &Where)
	void Contacted(LPNXSCENE Scene, Particle *Part, Particle *With, const NxVec3 &Where);
	void Contacted(LPNXSCENE Scene, Particle *Part, LPNXACTOR NotParticle, const NxVec3 &Where);

	void ParticleSpawn(FireworksParticle *part);
	void ParticleDeath(FireworksParticle *part);
	bool ParticleUpdate(float fDeltaTimeInSeconds, FireworksParticle *part);
	bool ParticleIsDead(const FireworksParticle *part);

	bool EmitterInitialize();
	bool EmitterUpdate(float fDeltaTimeInSeconds);
	void EmitterDestroy();

	void LoadEffect();

	bool StartEffect();
	bool EndEffect();

	UINT mEffectIndex;
public:
	FireworksParticleEmitter() {  mPosition = NxVec3(0,200,0); }

};

class FireworksShellEmitter : public BillboardEmitter<FireworksShell>
{
private:
	FireworksParticleEmitter mSparks;

	//void Contacted(LPNXACTOR Particle, LPNXACTOR ContactedWIth, const NxVec3 &Where)
	void Contacted(LPNXSCENE Scene, Particle *Part, Particle *With, const NxVec3 &Where);
	void Contacted(LPNXSCENE Scene, Particle *Part, LPNXACTOR NotParticle, const NxVec3 &Where);

	void ParticleSpawn(FireworksShell *part);
	void ParticleDeath(FireworksShell *part);
	bool ParticleUpdate(float fDeltaTimeInSeconds, FireworksShell *part);
	bool ParticleIsDead(const FireworksShell *part);

	bool EmitterInitialize();
	bool EmitterUpdate(float fDeltaTimeInSeconds);
	void EmitterDestroy();
	bool EmitterRender(float fDeltaTimeInSeconds, LPDIRECT3DDEVICE9 Device, const D3DXMATRIX &MatView, const NxVec3 &VecView);
public:
	FireworksShellEmitter() { mPosition = NxVec3(100,10,0); }
};

#endif