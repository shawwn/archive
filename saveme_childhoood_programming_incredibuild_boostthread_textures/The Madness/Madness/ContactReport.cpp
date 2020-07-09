#include "ContactReport.h"

ContactReport gContact;

void ContactReport::SetActorGroupFlags(NxPhysicsSDK *SDK, NxScene *Scene)
{
	if(!Scene)
		return;
}

void ContactReport::onContactNotify(NxContactPair &pair, NxU32 events)
{
}