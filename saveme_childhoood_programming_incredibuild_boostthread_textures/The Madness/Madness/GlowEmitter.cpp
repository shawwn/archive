#include "GlowEmitter.h"
#include "LoggingInterface.h"
#include "ClientSettingsInterface.h"
#include "TextureInterface.h"
#include <time.h>

void GlowEmitter::Contacted(LPNXSCENE Scene, Particle *Part, Particle *With, const NxVec3 &Where) 
{ 
	MyParticle *reinterp = (MyParticle *)Part;
	reinterp->Velocity.x += gSettings.ParticleResponse.x;
	reinterp->Velocity.y += gSettings.ParticleResponse.y;
	reinterp->Velocity.z += gSettings.ParticleResponse.z;
	//reinterp->mLife = 0;	
}
void GlowEmitter::Contacted(LPNXSCENE Scene, Particle *Part, LPNXACTOR NotParticle, const NxVec3 &Where) 
{ 
	MyParticle *Glow = (MyParticle *)Part;
	Glow->l->Color.y = 0.0f;
	Glow->l->Color.z = 0.0f;

	if(NotParticle->isDynamic()) {
		NotParticle->addForce(NxVec3(NotParticle->getGlobalPosition() - Part->Position) * 100);
	}
}

void GlowEmitter::ParticleSpawn(MyParticle *part) 
{ 
	part->mLife = 10.0f; part->Velocity = mFacing*3; part->Position = mPosition; 
	part->l = gLights.AddLight(NV3TODV3(part->Position),1.0f,D3DXVECTOR3(0.92f,0.95f,1.0f),30.0f,D3DXVECTOR3(0.0f,0.5f,0.0f));
}
void GlowEmitter::ParticleDeath(MyParticle *part) 
{ 
	gLights.ReleaseLight(part->l); part->l = NULL; 
}

bool GlowEmitter::ParticleUpdate(float fDeltaTimeInSeconds, MyParticle *part) 
{ 
	part->mLife -= fDeltaTimeInSeconds; 
	//part->Velocity += NxVec3(0,-9.8f,0) * fDeltaTimeInSeconds;
	part->l->Position = NV3TODV3(part->Position);
	part->l->Intensity = part->mLife / 10.0f;
	part->l->Color.y += 0.4f * fDeltaTimeInSeconds;
	part->l->Color.z += 0.4f * fDeltaTimeInSeconds;
	//part->Position += part->Velocity * fDeltaTimeInSeconds; 
	return true; 
}
bool GlowEmitter::ParticleIsDead(const MyParticle *part) 
{ 
	return (part->mLife <= 0); 
}

bool GlowEmitter::EmitterInitialize() 
{ 
	mPositions = new BlockManager(sizeof(NxVec3),1024); 
	mNumPositions = 1024; 
	
	NxActorDesc ActorDesc;
	ActorDesc.body = NULL;

	NxBodyDesc BodyDesc;
	BodyDesc.angularDamping = 0.7f;
	BodyDesc.linearDamping = 0.5f;

	NxSphereShapeDesc Sphere;
	Sphere.radius = 5.0f;

	ActorDesc.density = 10.0f;

	SetDesc(ActorDesc,BodyDesc,Sphere,sizeof(NxSphereShapeDesc));
	return true;
}
void GlowEmitter::EmitterDestroy() 
{ 
	SAFE_DELETE(mPositions); 
}


bool GlowEmitter::EmitterUpdate(float fDeltaTimeInSeconds) { 
	//static float Angle = 0.0f;
	//Angle += fDeltaTimeInSeconds / 2.0f;
	//NxQuat quat(Angle,mFacing.cross(NxVec3(1,0,0)));
	NxQuat quaty(gSettings.XRot,NxVec3(0,1,0));
	NxQuat quatx(gSettings.YRot,NxVec3(1,0,0));
	quatx.rotate(mFacing);
	quaty.rotate(mFacing);
	Tick += fDeltaTimeInSeconds; 
	//for(int i = 0; i < Tick / 0.0002f; i++) { 
	//	AddParticle(AllocateParticle()); }
	srand((unsigned int)time(NULL));
	if(Tick > (gSettings.Rate > 0 ? gSettings.Rate : 0.1f)) {
		MyParticle* Part = AddParticle((gSettings.Particles > 0 ? gSettings.Particles : 1));
		//AddParticle(1,Scene);
		Tick = 0;
	}
	return true; 
}

const std::string &GlowEmitter::Stats()
 { 
	int NumBlocks = 0, NumAllocated = 0, NumFragments = 0, NumFree = 0, NumTotal = 0;
	mMemory->GetStats(NumBlocks,NumAllocated,NumFragments,NumFree,NumTotal);
	gLog << "NumBlocks[" << NumBlocks << "] NumAllocated[" << NumAllocated << "] NumFragments[" << NumFragments << "] NumFree[" << NumFree << "] NumTotal[" << NumTotal << "]";
	mStats = gLog.GetStream().str();
	gLog.GetStream().str(std::string());
	return mStats;
}

 /*bool GlowEmitter::Render(float fDeltaTimeInSeconds, LPDIRECT3DDEVICE9 Device, const D3DXMATRIX &MatView, const NxVec3 &VecView) {
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

	if(mActiveParticles.size())
	{
		NxVec3 *Positions = mPositions->Construct<NxVec3>((unsigned short)mActiveParticles.size());

		UINT i;
		for(it = mActiveParticles.begin(), i = 0; it != mActiveParticles.end(); it++, i++) {
			Positions[i] = (*it)->Position;
		}

		mBillboards.Make(Device,(char *)Positions,(UINT)mActiveParticles.size(),sizeof(NxVec3),NxVec3(MatView._12,MatView._22,MatView._32),NxVec3(MatView._11,MatView._21,MatView._31),10,10);
		Device->SetTexture(0,gTextures.Find(0)->Texture);
		return mBillboards.Render(Device);
	}
	return true;
}*/