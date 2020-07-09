#ifndef CONTACTREPORT_H
#define CONTACTRERPOT_H

#include "NovodexInterface.h"

class ContactReport : public ExtendedUserContactReport
{
private:
public:
	void SetActorGroupFlags(NxPhysicsSDK *SDK, NxScene *Scene);
	void onContactNotify(NxContactPair &pair, NxU32 events);
};
extern ContactReport gContact;

#endif