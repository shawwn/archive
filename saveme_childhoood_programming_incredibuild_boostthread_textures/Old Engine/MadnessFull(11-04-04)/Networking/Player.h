#ifndef _PLAYER_H
#define _PLAYER_H

#include "Client.h"
#include "Account.h"
#include "Bsd.h"

class Player
{
private:
	Account *mAccount;
	string mName;
	QuadWord mGuid;
public:
	Player();
	~Player();

	SOCKET GetSocket() { if(!mAccount) { return INVALID_SOCKET; } return GetClient()->GetSocket(); }
	Client *GetClient() { if(!mAccount) { return NULL; } return mAccount->GetClient(); }
	Account *GetAccount() { return mAccount; }
	string GetName() { return mName; }
	QuadWord GetGuid() { return mGuid; }

	void SetAccount(Account *acct) { if(!acct) { return; } if(!acct->GetClient()) { return; } mAccount = acct; }
	void SetName(const string &Name) { mName = Name; }
	void SetGuid(const QuadWord &guid) { mGuid = guid; }

	const Player &operator =(const Player &p);
};

#endif
