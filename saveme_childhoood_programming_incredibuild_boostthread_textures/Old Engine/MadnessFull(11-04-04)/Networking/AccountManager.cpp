#include "stdafx.h"
#include "AccountManager.h"

AccountManager::AccountManager()
{

}

AccountManager::~AccountManager()
{

}


void AccountManager::Add(Account *acct)
{
	mAccounts[acct->GetName()] = acct;
}

void AccountManager::Remove(Account *acct)
{
	mAccounts.erase(acct->GetName());
}

void AccountManager::Clear(void)
{
	mAccounts.clear();
}

Account *AccountManager::Find(Client *cli)
{
	Iterator it;
	for(it = mAccounts.begin(); it != mAccounts.end(); it++)
	{
		if((*it).second->GetClient() == cli)
			return (*it).second;
	}

	return NULL;
}

Account *AccountManager::Find(const char *name)
{
	Iterator it;
	it = mAccounts.find(name);
	if(it == mAccounts.end())
		return NULL;
	return (*it).second;
}
