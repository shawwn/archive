#ifndef PACKET_H
#define PACKET_H

#include "Types.h"
#include <vector>

class Packet
{
private:
	Word mLength;
	DoubleWord mOpCode;
	std::vector<Byte> mData;

	void Add(Byte *Data, UINT SizeInBytes);
	void Get(Byte *Data, UINT SizeInBytes);
public:
	Packet() : mLength(0), mOpCode(0) { mData.reserve(1000); }
	Packet(DoubleWord OpCode) { Build(OpCode);  mData.reserve(1000); }

	Packet &operator <<(const char *c) {
		*this << (std::string)c; return *this; }

	Packet &operator <<(const std::string &str) {
		Add((Byte *)str.c_str(),(UINT)str.length());
		mData.push_back(0); mLength++;
		return *this; }

	Packet &operator >>(std::string &str) {
		if(!mLength) { str = ""; return *this; }
		for(UINT i = 0; mData[i] != 0 && i < mLength; i++) { }
		if(mData[i] != 0) { str = ""; return *this; }
		std::copy(mData.begin(),mData.begin()+i,back_inserter(str));
		mData.erase(mData.begin(),mData.begin()+i+1);
		mLength -= (UINT)str.length()+1;
		return *this;
	}

	template<class T>
		Packet &operator <<(const T &elem) {
			Add((Byte *)&elem,sizeof(T)); return *this; }

	template<class T>
		Packet &operator >>(T &elem) {
			Get((Byte *)&elem,sizeof(T)); return *this;	}

	bool FromInput(std::string &Data);
	void Clear();
	Packet &Build(DoubleWord OpCode);

	inline Word GetLength() const { return mLength; }
	inline DoubleWord GetOpCode() const { return mOpCode; }
	inline const Byte *Access() { return &mData[0]; }
};

enum OpCodes
{
	MSG_NULL = 0x0000,
	CMSG_DISCONNECT = 0x0001, 
	SMSG_SHUTDOWN,
	CMSG_LOGIN,
	SMSG_LOGIN,
	SMSG_NEWPLAYER,
	CMSG_ADJUSTVIEW,
	CMSG_PLAYER_INPUT,
	SMSG_OBJECT_UPDATE,
	SMSG_PLAYERDISCONNECT,
	SMSG_PING,
	CMSG_PONG,
	CMSG_OBJECT_CREATE,
	CMSG_OBJECT_CLEARALL,
	SMSG_OBJECT_CREATE,
	SMSG_OBJECT_DESTROY,
	SMSG_PLAYERANIMATION,
	SMSG_TICK_SYNC,
	OPCODEMAX
};

static const std::string strOpCodes[OPCODEMAX] = {
	"MSG_NULL",
	"CMSG_DISCONNECT",
	"SMSG_SHUTDOWN",
	"CMSG_LOGIN",
	"SMSG_LOGIN",
	"SMSG_NEWPLAYER",
	"CMSG_ADJUSTVIEW",
	"CMSG_PLAYER_INPUT",
	"SMSG_OBJECT_UPDATE",
	"SMSG_PLAYERDISCONNECT",
	"SMSG_PING",
	"CMSG_PONG",
	"CMSG_OBJECT_CREATE",
	"CMSG_OBJECT_CLEARALL",
	"SMSG_OBJECT_CREATE",
	"SMSG_OBJECT_DESTROY",
	"SMSG_PLAYERANIMATION",
	"SMSG_TICK_SYNC"
};

#endif