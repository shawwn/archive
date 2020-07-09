#include "DatabaseInterface.h"
#include "LoggingInterface.h"
#include "Parsing.h"
#include <iostream>
using namespace std;

DatabaseInterface gDatabase;

void DatabaseInterface::Process(std::string &Line)
{
	if(Line == "") return;

	string Playername;
	string Username;
	string Password;

	FirstToken(Line,Playername);
	FirstToken(Line,Username);
	FirstToken(Line,Password);
	DatabaseEntry *Entry = new DatabaseEntry(Playername,Username,Password);
	mEntries.push_back(Entry);
}

bool DatabaseInterface::Load(const std::string &Filename)
{
	ifstream in;
	in.open(Filename.c_str(),ios::binary);
	if(!in.is_open())
	{
		ERR("Database","Failed to open database file [" << Filename << "]");
		return false;
	}

	mFilename = Filename;

	string Line;
	while(!in.eof())
	{
		getline(in,Line,'\n');
		Process(Trim(Line));
	}

	in.close();
	in.clear();

	return true;
}

DatabaseEntry *DatabaseInterface::FindPlayername(const std::string &Playername)
{
	for(UINT i = 0; i < NumEntries(); i++) 
	{
		if(Upper(mEntries[i]->Playername) == Upper(Playername))
			return mEntries[i];
	}
	return NULL;
}

DatabaseEntry *DatabaseInterface::FindUsername(const std::string &Username)
{
	for(UINT i = 0; i < NumEntries(); i++) 
	{
		if(Upper(mEntries[i]->Username) == Upper(Username))
			return mEntries[i];
	}
	return NULL;
}

DatabaseEntry *DatabaseInterface::FindPassword(const std::string &Password)
{
	for(UINT i = 0; i < NumEntries(); i++) 
	{
		if(mEntries[i]->Password == Password)
			return mEntries[i];
	}
	return NULL;
}

bool DatabaseInterface::AddEntry(const std::string &Playername, const std::string &Username, const std::string &Password)
{
	if(FindPlayername(Playername) || FindUsername(Username)) return false;
	DatabaseEntry *Ent = new DatabaseEntry(Playername,Username,Password);
	mEntries.push_back(Ent);
	return true;
}

void DatabaseInterface::Save()
{
	if(mFilename == "") return;

	ofstream out;
	out.open(mFilename.c_str(),ios::binary);
	for(UINT i = 0; i < NumEntries(); i++) {
		out << mEntries[i]->Playername << "/" << mEntries[i]->Username << "/" << mEntries[i]->Password << "\r\n";
	}
	out.close();
	out.clear();
}
