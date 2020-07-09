#include "stdafx.h"
#include "Packet.h"

Packet::Packet(void) : mOpCode(0), mLength(0)
{

}

void Packet::Build(DoubleWord OpCode)
{
	mLength = 0;
	mOpCode = OpCode;
}

void Packet::AddByte(Byte d)
{
	mData[mLength++] = d;
}

void Packet::AddBytes(Byte *d, Word len)
{
	Word x;
	
	for (x = 0; x < len; x++)
		mData[mLength++] = d[x];
}

void Packet::AddWord(Word d)
{
	mData[mLength++] = (Byte)((d & 0x00FF) >> 0);
	mData[mLength++] = (Byte)((d & 0xFF00) >> 8);
}

void Packet::AddDoubleWord(DoubleWord d)
{
	mData[mLength++] = (Byte)((d & 0x000000FF) >> 0);
	mData[mLength++] = (Byte)((d & 0x0000FF00) >> 8);
	mData[mLength++] = (Byte)((d & 0x00FF0000) >> 16);
	mData[mLength++] = (Byte)((d & 0xFF000000) >> 24);
}

void Packet::AddQuadWord(QuadWord q)
{
	mData[mLength++] = (Byte)((q & 0x000000FF) >> 0);
	mData[mLength++] = (Byte)((q & 0x0000FF00) >> 8);
	mData[mLength++] = (Byte)((q & 0x00FF0000) >> 16);
	mData[mLength++] = (Byte)((q & 0xFF000000) >> 24);
	q = q >> 32;
	mData[mLength++] = (Byte)((q & 0x000000FF) >> 0);
	mData[mLength++] = (Byte)((q & 0x0000FF00) >> 8);
	mData[mLength++] = (Byte)((q & 0x00FF0000) >> 16);
	mData[mLength++] = (Byte)((q & 0xFF000000) >> 24);
}

void Packet::AddFloat(float d)
{
	Byte *b;

	b = (unsigned char *)&d;
	mData[mLength++] = b[0];
	mData[mLength++] = b[1];
	mData[mLength++] = b[2];
	mData[mLength++] = b[3];
}

bool Packet::FromInput(Client *cli)
{
	if(cli->GetInBuffer().length() < (Word)(2+sizeof(mOpCode)))
		return false;

	mLength = 0;
	memcpy(&mLength,cli->GetInBuffer().c_str(),2);
	mLength = ntohs(mLength) - sizeof(mOpCode);

	if(cli->GetInBuffer().length() < (Word)(2 + sizeof(mOpCode) + mLength))
		return false;

	if(mLength > (Word)sizeof(mData))
		return false;

	mOpCode = 0;
	memcpy(&mOpCode,cli->GetInBuffer().substr(2).c_str(),sizeof(mOpCode));

	if(mLength)
		memcpy(&mData,cli->GetInBuffer().substr(2 + sizeof(mOpCode)).c_str(), mLength);

	cli->GetInBuffer().erase(0,2+sizeof(mOpCode)+mLength);
	return true;
}

DoubleWord Packet::GetOpCode()
{
	return mOpCode;	
}

Byte Packet::GetByte(Word Offset)
{
	return mData[Offset];
}

Byte *Packet::GetBytes(Word Offset)
{
	return mData + Offset;
}

Word Packet::GetWord(Word Offset)
{
	return *((Word *)(mData + Offset));
}

DoubleWord Packet::GetDoubleWord(Word Offset)
{
	return *((DoubleWord *)(mData + Offset));
}

QuadWord Packet::GetQuadWord(Word Offset)
{
	return *((QuadWord *)(mData + Offset));
}

float Packet::GetFloat(Word Offset)
{
	return *((float *)(mData + Offset));
}

const char *OpCodeName(DoubleWord op)
{
	return OpCodeNames[op];
}


const char *OpCodeNames[] = {
	"MSG_NULL_ACTION",
	"CMSG_AUTH_SESSION",
	"SMSG_AUTH_RESPONSE",
	"CMSG_CHAR_CREATE",
	"SMSG_CHAR_CREATE",
	"CMSG_CHAR_ENUM",
	"SMSG_CHAR_ENUM",
	"CMSG_CHAR_DELETE",
	"SMSG_CHAR_DELETE",
	"CMSG_CHAR_PICK",
	"SMSG_CHAR_PICK"
};
