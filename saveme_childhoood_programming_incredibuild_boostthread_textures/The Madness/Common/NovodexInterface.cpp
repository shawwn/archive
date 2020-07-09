#include "NovodexInterface.h"
#include "LoggingInterface.h"
#include "Direct3DInterface.h"
#include "Particle.h"

NovodexInterface gPhysics;

inline D3DXVECTOR3 NV3TODV3(NxVec3 V) { return D3DXVECTOR3(V.x,V.y,V.z); }
inline NxVec3 DV3TONV3(D3DXVECTOR3 V) { return NxVec3(V.x,V.y,V.z); }
/*
class ContactReport : public NxUserContactReport
{
public:
	void onContactNotify(NxContactPair &pair, NxU32 events)
	{
		LPNXACTOR Actor[2];
		int ActorGroup[2];
		Actor[0] = pair.actors[0];
		Actor[1] = pair.actors[1];
		ActorGroup[0] = Actor[0]->getGroup();
		ActorGroup[1] = Actor[1]->getGroup();

		Particle *Part[2];
		ParticleEmitterBase *Emitter[2];

		if(ActorGroup[0] == 2) {
			Part[0] = (Particle *)Actor[0]->userData;
			Emitter[0] = Part[0]->Emitter;
		}

		if(ActorGroup[1] == 2) {
			Part[1] = (Particle *)Actor[1]->userData;
			Emitter[1] = Part[1]->Emitter;
		}

		NxContactStreamIterator i(pair.stream);
		i.goNextPair();
		i.goNextPatch();
		i.goNextPoint();
		
		if((ActorGroup[0] == 2) && (ActorGroup[1] == 2)) { // They are both particles.
			Emitter[0]->Contacted(gPhysics.GetScene(),Part[0],Part[1],i.getPoint());
			Emitter[1]->Contacted(gPhysics.GetScene(),Part[1],Part[0],i.getPoint());
		}
		else if(ActorGroup[0] == 2) {
			Emitter[0]->Contacted(gPhysics.GetScene(),Part[0],Actor[1],i.getPoint());
		}
		else if(ActorGroup[1] == 2) {
			Emitter[1]->Contacted(gPhysics.GetScene(),Part[1],Actor[0],i.getPoint());
		}
	}
} gNotify;
*/

bool NovodexInterface::Initialize(ExtendedUserContactReport *Contact)
{
	mSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION,0,NULL);

	if(!mSDK) {
		gLog ^ "ERROR: Could not create physics SDK.  Bad version?\n";
		return false;
	}

	mSDK->setParameter(NX_MIN_SEPARATION_FOR_PENALTY, -0.05f);
	mSDK->setActorGroupPairFlags(2,2,NX_NOTIFY_ON_START_TOUCH);
	mSDK->setActorGroupPairFlags(2,1,NX_NOTIFY_ON_START_TOUCH);

	NxMaterial	defaultMaterial;
	defaultMaterial.restitution		= 0.0f;
	defaultMaterial.staticFriction	= 0.5f;
	defaultMaterial.dynamicFriction	= 0.5f;
	mSDK->setMaterialAtIndex(0, &defaultMaterial);

	NxSceneDesc SceneDesc;
	SceneDesc.gravity = mDefaultGravity;
	SceneDesc.broadPhase = NX_BROADPHASE_COHERENT;
	SceneDesc.collisionDetection = true;
	SceneDesc.userContactReport = Contact;
	mScene = mSDK->createScene(SceneDesc);
	if(!mScene) {
		gLog ^ "ERROR: Could not create physics scene.\n";
		return false;
	}

	if(Contact)
		Contact->SetActorGroupFlags(mSDK, mScene);

	NxPlaneShapeDesc PlaneDesc;
	NxActorDesc ActorDesc;
	ActorDesc.shapes.pushBack(&PlaneDesc);
	ActorDesc.group = 0;
	LPNXACTOR act = mScene->createActor(ActorDesc);
	

	/*D3DXCreateBox(Device,10,10,10,&mBox,NULL);

	NxBoxShapeDesc BoxDesc;
	BoxDesc.dimensions = NxVec3(10,10,10);
	NxBodyDesc BodyDesc;
	BodyDesc.angularDamping = 0.5f;

	NxActorDesc ActorDesc2;
	ActorDesc2.shapes.push_back(&BoxDesc);
	ActorDesc2.body = &BodyDesc;
	ActorDesc2.density = 10.0f;
	ActorDesc2.globalPose.t = NxVec3(0,100,0);

	mBoxActor = mScene->createActor(ActorDesc2);*/

	return true;
}