#include "stdafx.h"
#include "Player.h"
#include "Account.h"
#include "Client.h"

Player::Player()
{
	mAccount = NULL;
}

Player::~Player()
{

}
/*
bool Player::Make(SOCKET s)
{
	return mClient.Make(s);
}

bool Player::Make(Client *cli)
{
	if(cli) {
		mClient = *cli;
		return true;
	}
	return false;
}
*/
const Player &Player::operator =(const Player &p)
{
	mAccount = p.mAccount;
	return *this;
}
