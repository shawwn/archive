#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include <string>
#include <vector>
#include "Types.h"
#include "Macros.h"

struct DatabaseEntry
{
public:
	DatabaseEntry(const std::string &Player, const std::string &User, const std::string &Pass) : Playername(Player), Username(User), Password(Pass) { }
	std::string Playername;
	std::string Username;
	std::string Password;
};

class DatabaseInterface
{
private:
	std::string mFilename;
	std::vector<DatabaseEntry *> mEntries;

	void Process(std::string &Line);
public:
	DatabaseInterface() { }
	~DatabaseInterface() { 
		Save(); for(UINT i = 0; i < NumEntries(); i++) { SAFE_DELETE(mEntries[i]); } }

	bool Load(const std::string &Filename);
	DatabaseEntry *FindPlayername(const std::string &Playername);
	DatabaseEntry *FindUsername(const std::string &Username);
	DatabaseEntry *FindPassword(const std::string &Password);

	bool AddEntry(const std::string &Playername, const std::string &Username, const std::string &Password);
	void Save();

	inline DatabaseEntry *Find(UINT Index) const { return (Index < NumEntries() ? mEntries[Index] : NULL); }
	inline UINT NumEntries() const { return (UINT)mEntries.size(); }
};
extern DatabaseInterface gDatabase;

#endif