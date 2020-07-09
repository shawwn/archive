#include "FireworksEmitter.h"
#include <time.h>
#include "EffectInterface.h"
#include "GameInterface.h"
#include "ClientSettingsInterface.h"

void FireworksParticleEmitter::Contacted(LPNXSCENE Scene, Particle *Part, Particle *With, const NxVec3 &Where)
{
	
}

void FireworksParticleEmitter::Contacted(LPNXSCENE Scene, Particle *Part, LPNXACTOR NotParticle, const NxVec3 &Where)
{
	FireworksParticle *f = (FireworksParticle *)Part;
	f->Life = 0.0f;
}

#define sign( x ) (x < 0 ? -1 : 1 )

void FireworksParticleEmitter::ParticleSpawn(FireworksParticle *part)
{
	srand((UINT)time(NULL));
	part->Life = gSettings.FireworksParticleLife;
	part->Position = mPosition;
	float xrand = (float)(rand() % 256);
	float yrand = (float)(rand() % 256);
	float zrand = (float)(rand() % 256);

	part->l = gLights.AddLight(NV3TODV3(part->Position),1.0f,D3DXVECTOR3(xrand/255.0f,yrand/255.0f,zrand/255.0f),100.0f,D3DXVECTOR3(0,1,0));

	srand((UINT)time(NULL)+GetNumParticles());
	part->Position.x += rand() % 100;
	part->Position.z += rand() % 100;
	
	NxReal xdir = (float)(rand() % 2) * 2 - 1;
	NxReal ydir = (float)(rand() % 100);
	NxReal zdir = (float)(rand() % 2) * 2 - 1;
	part->Velocity = NxVec3(10*xdir,30*(ydir/100.0f),5*zdir);
}

void FireworksParticleEmitter::ParticleDeath(FireworksParticle *part)
{
	gLights.ReleaseLight(part->l);
}

bool FireworksParticleEmitter::ParticleUpdate(float fDeltaTimeInSeconds, FireworksParticle *part)
{
	part->Life -= fDeltaTimeInSeconds;
	part->l->Position = NV3TODV3(part->Position);
	part->l->Intensity = part->Life / gSettings.FireworksParticleLife;
	return true;
}

bool FireworksParticleEmitter::ParticleIsDead(const FireworksParticle *part)
{
	return (part->Life <= 0);
}

bool FireworksParticleEmitter::EmitterInitialize()
{
	srand((UINT)time(NULL));
	NxActorDesc ActorDesc;
	NxBodyDesc BodyDesc;
	ActorDesc.density = gSettings.FireworksParticleDensity;
	ActorDesc.group = gSettings.FireworksParticleGroup;
	BodyDesc.linearDamping = gSettings.FireworksParticleLinearDampening;
	BodyDesc.angularDamping = gSettings.FireworksParticleAngularDampening;
	NxSphereShapeDesc ShapeDesc;
	ShapeDesc.radius = gSettings.FireworksParticleSphereRadius;

	mAlphaTextureIndex = gTextures.Load("Data/Textures/fireworks/alpha.jpg");
	mDecalTextureIndex = gTextures.Load("Data/Textures/fireworks/sticks.jpg");

	SetDesc(ActorDesc,BodyDesc,ShapeDesc,sizeof(NxSphereShapeDesc));

	LoadEffect();

	return true;
}

bool FireworksParticleEmitter::EmitterUpdate(float fDeltaTimeInSeconds)
{
	Avg.Intensity = 0.0f;
	Avg.Color = D3DXVECTOR3(0,0,0);

	for(Iterator it = Begin(); it != End(); it++) {
		FireworksParticle *part = (FireworksParticle*)(*it);
		Avg.Intensity += part->l->Intensity;
		Avg.Color += part->l->Color;
	}

	Avg.Intensity /= (float)GetNumParticles();
	Avg.Color /= (float)GetNumParticles();

	return true;
}

void FireworksParticleEmitter::EmitterDestroy()
{

}

bool FireworksParticleEmitter::StartEffect()
{
	LPD3DXEFFECT Eff = gEffects.FindEffect(mEffectIndex);
	if(Eff)
	{
		UINT Passes;
		HRESULT hr;
		if(FAILED(hr = Eff->Begin(&Passes,0))) 
			LogDxErr(hr,"Fireworks");
		D3DXVECTOR3 v(1,1,1);
		if(FAILED(hr = Eff->SetFloatArray("Light.lightColor",&Avg.Color.x,3))) 
			LogDxErr(hr,"Fireworks");
		if(FAILED(hr = Eff->SetFloat("Light.lightIntensity",Avg.Intensity)))
			LogDxErr(hr,"Fireworks");
		if(FAILED(hr = Eff->SetTexture("alphaTexture",gTextures.Find(mAlphaTextureIndex)->Texture)))
			LogDxErr(hr,"Fireworks");
		if(FAILED(hr = Eff->SetTexture("decalTexture",gTextures.Find(mDecalTextureIndex)->Texture)))
			LogDxErr(hr,"Fireworks");
		if(FAILED(hr = Eff->SetMatrix("matWVP",&gEngine.GetMatVP())))
			LogDxErr(hr,"Fireworks");
		if(FAILED(hr = Eff->BeginPass(0))) {
			LogDxErr(hr,"Fireworks");
		}
	}
	return true;
}

bool FireworksParticleEmitter::EndEffect()
{
	LPD3DXEFFECT Eff = gEffects.FindEffect(mEffectIndex);
	if(Eff)
	{
		Eff->EndPass();
		Eff->End();
	}
	return true;
}

void FireworksParticleEmitter::LoadEffect()
{
	mEffectIndex = gEffects.Load("Data/Shaders/spark.fx");
}


void FireworksShellEmitter::Contacted(LPNXSCENE Scene, Particle *Part, Particle *With, const NxVec3 &Where)
{
	
}

void FireworksShellEmitter::Contacted(LPNXSCENE Scene, Particle *Part, LPNXACTOR NotParticle, const NxVec3 &Where)
{

}

void FireworksShellEmitter::ParticleSpawn(FireworksShell *part)
{
	part->Position = mPosition;
	part->Velocity = NxVec3(0,0,0);
	part->Life = 5.0f;
	part->l = gLights.AddLight(NV3TODV3(part->Position),2.0f,D3DXVECTOR3(1,0.5f,0.5f),40);	
}

void FireworksShellEmitter::ParticleDeath(FireworksShell *part)
{
	mSparks.AddParticle(gSettings.NumFireworksParticles);	
	FireworksShell *p = AddParticle(1);
	if(gSettings.FireworksShellChain) {
		p->Position = part->Position;
		p->Position.y += 15.0f;
		p->Actor->setGlobalPosition(p->Position);
	}
	p->Velocity = NxVec3(0,gSettings.FireworksShellInitialVelocity,0);
	gLights.ReleaseLight(part->l);
}

static float Tick = 0;
bool FireworksShellEmitter::ParticleUpdate(float fDeltaTimeInSeconds, FireworksShell *part)
{
	Tick += fDeltaTimeInSeconds;
	if(Tick > 5.0f && Tick < 10.0f) {
		part->Velocity = NxVec3(0,150,0);
		Tick = 10.0f;
	}
	part->Life -= fDeltaTimeInSeconds;
	part->l->Position = NV3TODV3(part->Position);
	mSparks.SetPosition(part->Position);
	return true;
}

bool FireworksShellEmitter::ParticleIsDead(const FireworksShell *part)
{
	if(part->Actor->getLinearVelocityVal().magnitude() > 0.1f) {
		return (part->Actor->getLinearVelocityVal().magnitude() < 10.0f);
	}
	return false;
}

bool FireworksShellEmitter::EmitterInitialize()
{
	mPosition = NxVec3(0,0,20);
	mSparks.SetScene(mScene);
	mSparks.Initialize();
	NxActorDesc ActorDesc;
	NxBodyDesc BodyDesc;
	ActorDesc.density = gSettings.FireworksShellDensity;
	ActorDesc.group = gSettings.FireworksShellGroup;
	BodyDesc.linearDamping = gSettings.FireworksShellLinearDampening;
	BodyDesc.angularDamping = gSettings.FireworksShellAngularDampening;
	NxSphereShapeDesc ShapeDesc;
	ShapeDesc.radius = gSettings.FireworksShellSphereRadius;

	SetDesc(ActorDesc,BodyDesc,ShapeDesc,sizeof(NxSphereShapeDesc));

	FireworksShell *part = AddParticle(1);
	return true;
}

bool FireworksShellEmitter::EmitterUpdate(float fDeltaTimeInSeconds)
{
	return mSparks.Update(fDeltaTimeInSeconds);
}

void FireworksShellEmitter::EmitterDestroy()
{

}

#ifndef min
#define min( x, y ) (x < y ? x : y)
#endif

bool FireworksShellEmitter::EmitterRender(float fDeltaTimeInSeconds, LPDIRECT3DDEVICE9 Device, const D3DXMATRIX &MatView, const NxVec3 &VecView)
{ 
	static float SizeMod = 0;
	if(mSparks.Begin() != mSparks.End()) {
		SizeMod += fDeltaTimeInSeconds;
		return mSparks.Render(fDeltaTimeInSeconds,Device,MatView,VecView,0,(UINT)(sin(min(SizeMod,1)*3.1415926/2)*gSettings.FireworksParticleSize),(UINT)(sin(min(SizeMod,1)*3.1415926/2)*gSettings.FireworksParticleSize));	
	}
	return true;
}