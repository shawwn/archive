#ifndef _PACKET_H
#define _PACKET_H

#include <vector>
#include "Client.h"
#include "Types.h"
#include "Bsd.h"
using namespace std;

class Packet
{
private:
	DoubleWord mOpCode;
	Byte mData[1024*32];
	Word mLength;
public:
	Packet();

	void Build(DoubleWord OpCode);
	void AddByte(Byte b);
	void AddBytes(Byte *bs, Word len);
	void AddWord(Word w);
	void AddDoubleWord(DoubleWord dw);
	void AddQuadWord(QuadWord qw);
	void AddFloat(float f);
	bool FromInput(Client *cli);

	Word GetDataLength() { return mLength; }
	DoubleWord GetOpCode();
	Byte GetByte(Word Offset);
	Byte *GetBytes(Word Offset = 0);
	Word GetWord(Word Offset);
	DoubleWord GetDoubleWord(Word Offset);
	QuadWord GetQuadWord(Word Offset);
	float GetFloat(Word Offset);
	const char *OpCodeName(DoubleWord op);
};

enum OpCodes 
{
	MSG_NULL_ACTION = 0x0000,

	CMSG_AUTH_SESSION = 0x0001,
	// Word (Version)
	// String (Username)
	// String (Password)

	SMSG_AUTH_RESPONSE = 0x0002,
	// byte (Authenticated?)
	//  01 - Authorized
	//  02 - Username invalid
	//  03 - Password invalid
	//  04 - Bad version
	//  05 - Already logged in
	//  06 - Banned
	//  07 - No worldservers to connect to
	// If Authorized 
	//	byte (Number of worldservers available)
	//	foreach worldserver
	//   DoubleWord (Network IP of worldserver)
	//   Word (Port worldserver is listening on)
	//   String (Name of worldserver)
	//  end for
	// end if

	CMSG_PICK_WORLDSERVER = 0x0003,

	SMSG_PICK_WORLDSERVER = 0x0004,
	// byte (Worldserver prepared connection?)
	//  01 - Prepared
	//  02 - Client not authorized
	//  03 - No worldserver with that IP and port
	//  04 - Worldserver could not prepare connection
	// if Prepared
	//  DoubleWord IP of Worldserver
	//  Word Port of Worldserver
	// end if

	CMSG_PREP_CONNECTION = 0x0005,
	// string (Account name)
	// DoubleWord (Network IP)

	SMSG_PREP_CONNECTION = 0x0006,
	// byte (Prepared?)
	//  01 - Prepared
	// string (Name of account prepared)

	CMSG_AUTH_WORLD = 0x0007,
	// string (Username)

	SMSG_AUTH_WORLD = 0x0008,
	// byte (Authorized?)
	//  01 - Authorized and account logged in
	//  02 - Worldserver was not prepared for the connection

	CMSG_CHAT_SAY = 0x0009,

	SMSG_CHAT_SAY = 0x000A,

	CMSG_CHAR_CREATE = 0x0003,
	// string (Name)

	SMSG_CHAR_CREATE  = 0x0004,
	// byte (Created?)
	//  01 - Created
	//  02 - Username too short
	//  03 - Username too long
	
	CMSG_CHAR_ENUM = 0x0005,
	// null

	SMSG_CHAR_ENUM = 0x0006,
	// byte (Number of characters)
	// Foreach Player Then:
	// {
	//		QuadWord (GUID of character)
	//		string (Player name)
	// }

	CMSG_CHAR_DELETE = 0x0007,
	// QuadWord (GUID of character)
	
	SMSG_CHAR_DELETE = 0x0008,
	// byte (Deleted?)
	//  00 - Deleted
	//  01 - Account doesn't exist
	//  02 - Character GUID doesn't exist

	CMSG_CHAR_PICK = 0x0009,
	// QuadWord (GUID of character)

	
	SMSG_CHAR_PICK = 0x000A
	// byte (Authorized?)
	//  0 - Authorized
	//  1 - Not Authorized.  Terminate packet.
	// DoubleWord (Network IP the client needs to connect to for the worldserver)

};

extern const char *OpCodeNames[];

#endif
