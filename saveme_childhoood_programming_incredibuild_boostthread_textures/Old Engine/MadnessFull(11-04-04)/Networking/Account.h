#ifndef _ACCOUNT_H
#define _ACCOUNT_H


#include "PlayerManager.h"
#include "Bsd.h"

#include <string>
using namespace std;

class Account
{
private:
	string mUsername;
	string mPassword;
	Client *mClient;
	long mIP;

	PlayerManager mPlayers;
public:
	Account();
	~Account();

	const string &GetName() { return mUsername; }
	const string &GetPassword() { return mPassword; }
	Client *GetClient() { return mClient; }
	const long &GetIP() { return mIP; }

	void SetClient(Client *cli) { mClient = cli; }
	void SetIP(const long &IP) { mIP = IP; }
	void SetName(const char *name) { mUsername = name; }
	void SetPassword(const char *password) { mPassword = password; }

	PlayerManager &Players() { return mPlayers; }
};

#endif
