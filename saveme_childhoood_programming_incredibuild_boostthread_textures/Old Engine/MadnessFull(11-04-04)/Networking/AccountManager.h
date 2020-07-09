#ifndef _ACCOUNTMANAGER_H
#define _ACCOUNTMANAGER_H

#include <map>
#include "Account.h"
#include "Bsd.h"
using namespace std;

class AccountManager
{
private:
	map<string, Account *> mAccounts;
public:
	typedef map<string, Account *>::iterator Iterator;

	AccountManager();
	~AccountManager();

	void Add(Account *acct);
	void Remove(Account *acct);
	void Clear(void);
	size_t Count(void) { return mAccounts.size(); };

	Iterator Begin(void) { return mAccounts.begin(); };
	Iterator End(void) { return mAccounts.end(); };

	Account *Find(Client *cli);
	Account *Find(const char *name);
};

#endif
