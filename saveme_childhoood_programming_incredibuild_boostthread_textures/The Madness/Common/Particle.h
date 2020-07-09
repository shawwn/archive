#ifndef PARTICLE_H
#define PARTICLE_H

#define NOMINMAX
#include <list>
#include <functional>
#include <algorithm>
#include "Macros.h"
#include "Block.h"
#include "NovodexInterface.h"
#include "Types.h"

class ParticleEmitterBase;

class Particle
{
public:
	NxVec3 Position;
	NxVec3 Velocity;	
	float DistanceFromViewer;
	LPNXACTOR Actor;
	ParticleEmitterBase *Emitter;
};

inline bool ParticlePred(const Particle *&p1, const Particle *&p2) { return p1->DistanceFromViewer > p2->DistanceFromViewer; }
struct ParticleSorter : std::binary_function<bool,Particle*,Particle*> {
};

class ParticleEmitterBase
{
public:
	virtual void Contacted(LPNXSCENE Scene, Particle *Part, Particle *With, const NxVec3 &Where) { }
	virtual void Contacted(LPNXSCENE Scene, Particle *Part, LPNXACTOR NotParticle, const NxVec3 &Where) { }
};

template<class T>
class ParticleEmitter : public ParticleEmitterBase
{
protected:	
	NxVec3 mPosition;
	NxVec3 mFacing;

	NxActorDesc mActorDesc;
	NxBodyDesc mBodyDesc;
	BYTE *mShapeDesc;
	UINT mShapeSize;
	NxActorFlag mActorFlags;
	
	float mRadius;

	BlockManager *mMemory;

	LPNXSCENE mScene;

	std::list<Particle *> mActiveParticles;

	virtual void ParticleSpawn(T *part) = 0;
	virtual void ParticleDeath(T *part) = 0;
	virtual bool ParticleUpdate(float fDeltaTimeInSeconds, T *part) = 0;
	virtual bool ParticleIsDead(const T *part) = 0; 

	virtual bool EmitterInitialize() { return true; }
	virtual bool EmitterUpdate(float fDeltaTimeInSeconds) { return true; }
	virtual void EmitterDestroy() { }


	bool AddToPhysics(T *Part, LPNXSCENE Scene)
	{
		mActorDesc.globalPose.t = Part->Position;

		Part->Actor = Scene->createActor(mActorDesc);
		Part->Actor->raiseActorFlag(mActorFlags);
		Part->Actor->userData = Part;
		return (Part->Actor != NULL);
	}

	void RemoveFromPhysics(T *Part)
	{
		if(Part->Actor && mScene)
			mScene->releaseActor(*Part->Actor);
	}

	T *AllocateParticle(unsigned short count = 1) { return mMemory->Construct<T>(count); }
	void DeallocateParticle(T *part) { mMemory->Deconstruct<T>(part); }
	T* AddParticle(T* part) { 
		if(!part) { return NULL; } 
		ParticleSpawn(part);
		if(mScene)
			AddToPhysics(part,mScene);
		else
			part->Actor = NULL;
		mActiveParticles.push_back(part); 
		return part; }
public:
	typedef std::list<Particle *>::iterator Iterator;

	ParticleEmitter() : mPosition(NxVec3(0,0,0)), mFacing(NxVec3(0,1,0)), mRadius(5), mMemory(NULL), mShapeDesc(NULL), mShapeSize(0), mScene(NULL)
	{ 
	
	}
	~ParticleEmitter() { EmitterDestroy(); SAFE_DELETE(mMemory); SAFE_DELETE(mShapeDesc); }

	void SetScene(LPNXSCENE Scene) { mScene = Scene; }
	
	T* AddParticle(unsigned short count = 1) {
		if(count == 0)
			count = 1;
		if(count > mMemory->GetNumElementsPerBlock())
			count = mMemory->GetNumElementsPerBlock();
		T *Particles = AllocateParticle(count);
		if(!Particles)
			return NULL;
		for(int i = 0; i < count; i++) {
			Particles[i].Actor = NULL;
			Particles[i].Emitter = this;

			ParticleSpawn(&Particles[i]);
			if(mScene)
				AddToPhysics(&Particles[i],mScene);
			else
				Particles[i].Actor = NULL;
			mActiveParticles.push_back(&Particles[i]);
		}
		return Particles;
	}
	void RemoveParticle(T *part) { RemoveFromPhysics(part); ParticleDeath(part); DeallocateParticle(part); }

	void SetDesc(const NxActorDesc &ActorDesc, const NxBodyDesc &BodyDesc, const NxShapeDesc &ShapeDesc, UINT ShapeSize, NxActorFlag ActorFlags = (NxActorFlag)0) {	
																				memcpy(&mActorDesc,&ActorDesc,sizeof(NxActorDesc)); 
																				memcpy(&mBodyDesc,&BodyDesc,sizeof(NxBodyDesc));
																				mShapeDesc = new BYTE [ShapeSize];
																				memcpy(mShapeDesc,&ShapeDesc,ShapeSize);
																				mActorDesc.body = &mBodyDesc;
																				mActorDesc.shapes.push_back((NxShapeDesc *)mShapeDesc); 
																				mActorDesc.group = 2; 
																				mActorFlags = ActorFlags; }
	void Initialize() {
		SAFE_DELETE(mMemory);
		mMemory = new BlockManager(sizeof(T),1024*2);
		EmitterInitialize();
	}

	bool Update(float fDeltaTimeInSeconds) {
		for(Iterator it = Begin(); it != End(); it++) {
			T *part = (T*)(*it);
			if(part->Actor)
				part->Position = part->Actor->getGlobalPose().t;
			
			if(part->Velocity.magnitude() > 0) {
				if(part->Actor) {
					part->Actor->addForce(part->Velocity,NX_VELOCITY_CHANGE);
					part->Velocity.zero();
				}
				else
				{
					part->Position += part->Velocity * fDeltaTimeInSeconds;
				}
			}
			if(!ParticleUpdate(fDeltaTimeInSeconds,part) || ParticleIsDead(part)) {
				RemoveParticle(part);
				it = mActiveParticles.erase(it);
			}
		}
		if(!EmitterUpdate(fDeltaTimeInSeconds)) {
			SAFE_DELETE(mMemory);
			mActiveParticles.clear();
			return false;
		}
		return true;
	}

	inline Iterator Begin() { return mActiveParticles.begin(); }
	inline Iterator End() { return mActiveParticles.end(); }
	inline size_t GetNumParticles() { return mActiveParticles.size(); }
	inline UINT GetParticleSizeInBytes() { return (UINT)mMemory->GetElementSize(); }
	inline void SetPosition(const NxVec3 &Pos) { mPosition = Pos; }
};

/*
class BlankEmitter : public ParticleEmitter
{
private:
	void ParticleSpawn(T *part) { }
	void ParticleDeath(T *part) { }
	void KillParticle(T *part) { }
	bool ParticleUpdate(float fDeltaTimeInSeconds, T *part) { }
	bool ParticleIsDead(const T *part) { }

	bool EmitterInitialize() { return true; }
	bool EmitterUpdate(float fDeltaTimeInSeconds, LPNXSCENE Scene) { return true; }
	void EmitterDestroy() { }
public:
};
*/

#endif