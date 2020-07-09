#ifndef PLAYER_H
#define PLAYER_H

#include "Client.h"
#include "Object.h"
#define NOMINMAX
#include "NxPhysics.h"
#include "Types.h"

class Player : public Object
{
private:
	Client *mClient;
public:
	UINT mAnimation;
	Player(QuadWord GUID) : Object(GUID), mAnimation(-1) { }

	inline void SetClient(Client *cli) { mClient = cli; }

	inline Client *GetClient() const { return mClient; }
};

#endif