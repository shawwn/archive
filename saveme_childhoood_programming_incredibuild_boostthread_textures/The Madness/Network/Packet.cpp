#include "Packet.h"
#include <algorithm>
#include "LoggingInterface.h"
using namespace std;

void Packet::Add(Byte *Data, UINT SizeInBytes) 
{
	size_t currentSize = mData.size();
	mData.resize(mData.size() + SizeInBytes);
	copy(Data,Data+SizeInBytes,mData.begin()+currentSize);
	//copy(Data,Data+SizeInBytes,back_inserter(mData));
	mLength += SizeInBytes;
}

void Packet::Get(Byte *Data, UINT SizeInBytes) 
{
	if(SizeInBytes > mLength)
		return;
	memcpy(Data,&mData[0],SizeInBytes);
	mData.erase(mData.begin(),mData.begin()+SizeInBytes);
	mLength -= SizeInBytes;
}

bool Packet::FromInput(std::string &Data)
{
	if(Data.length() < 6)
		return false;
	Word Length;

	memcpy(&Length,Data.c_str(),2);
	if(Data.length() < Length) {
		gLog << "Partial\n";
		return false;
	}

	Clear();
	mLength = 0;
	memcpy(&mOpCode,Data.c_str()+2,4);
	Add((Byte *)(Data.c_str()+6),Length-6);
	return true;
}

void Packet::Clear()
{
	mLength = 0;
	mOpCode = 0;
	mData.clear();
}

Packet &Packet::Build(DoubleWord OpCode)
{
	mLength = 0;
	mOpCode = OpCode;
	mData.clear();
	return *this;
}