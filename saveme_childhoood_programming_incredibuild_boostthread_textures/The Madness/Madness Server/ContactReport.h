#ifndef CONTACTREPORT_H
#define CONTACTRERPOT_H

#include "NovodexInterface.h"
#include "Object.h"

class ContactReport : public ExtendedUserContactReport
{
private:
	NxActor *mActors[2];
	Object *mObjects[2];
	bool TypesContacted(UINT Type1, UINT Type2);
public:
	void SetActorGroupFlags(NxPhysicsSDK *SDK, NxScene *Scene);
	void onContactNotify(NxContactPair &pair, NxU32 events);
};
extern ContactReport gContact;

#endif