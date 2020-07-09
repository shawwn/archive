#include "ServerInterface.h"
#include "ContactReport.h"
#include "LoggingInterface.h"
#include "TimeInterface.h"

ContactReport gContact;

bool ContactReport::TypesContacted(UINT Type1, UINT Type2)
{
	if(mObjects[0]->GetType() == Type1 && mObjects[1]->GetType() == Type2)
		return true;
	if(mObjects[0]->GetType() == Type2 && mObjects[0]->GetType() == Type1)
		return true;
	return false;
}

void ContactReport::SetActorGroupFlags(NxPhysicsSDK *SDK, NxScene *Scene)
{
	if(!SDK || !Scene)
		return;
	
	SDK->setActorGroupPairFlags(1,1,NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_TOUCH | NX_NOTIFY_ON_END_TOUCH);

}

void ContactReport::onContactNotify(NxContactPair &pair, NxU32 events)
{

	mActors[0] = pair.actors[0];
	mActors[1] = pair.actors[1];

	if(!mActors[0]->userData || !mActors[1]->userData)
		return;

	mObjects[0] = (Object *)mActors[0]->userData;
	mObjects[1] = (Object *)mActors[1]->userData;
	/*
	static float Tick = 0.0f;
	float Rate = 0.5f;
	while(Tick > Rate || (events & NX_NOTIFY_ON_START_TOUCH) || (events & NX_NOTIFY_ON_END_TOUCH)) {

		gServer.SendToPlayersInRange(&mObjects[0]->GetKinematicPacket(),mObjects[0]);
		gServer.SendToPlayersInRange(&mObjects[1]->GetKinematicPacket(),mObjects[1]);
		Tick -= Rate;

		if((events & NX_NOTIFY_ON_START_TOUCH) || (events & NX_NOTIFY_ON_END_TOUCH))
			break;
	}
	Tick += gTime.GetDeltaTimeInSeconds();
	*/


}