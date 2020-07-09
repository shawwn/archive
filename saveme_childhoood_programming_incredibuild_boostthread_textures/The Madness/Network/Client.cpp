#include "Client.h"
#include "TimeInterface.h"
using namespace std;

void Client::Send(Packet *pack)
{
	if(!pack)
		return;

	Word len = pack->GetLength() + sizeof(Word) + sizeof(DoubleWord);
	size_t currentsize;
	DoubleWord op = pack->GetOpCode();

	currentsize = mOutBuffer.length();
	mOutBuffer.resize(currentsize + len);

	copy((char *)&len,(char *)&len+sizeof(Word),mOutBuffer.begin() + currentsize);
	currentsize += sizeof(Word);
	copy((char *)&op,(char *)&op+sizeof(DoubleWord),mOutBuffer.begin() + currentsize);
	currentsize += sizeof(DoubleWord);
	copy(pack->Access(),pack->Access()+pack->GetLength(),mOutBuffer.begin() + currentsize);
	currentsize += pack->GetLength();
}

void Client::Close(const std::string &str)
{
	if(mClosing) return;
	Packet pack(SMSG_SHUTDOWN);
	pack << str;
	Send(&pack);
	mClosing = true;
}
void Client::AddTotalSentBytes(UINT NumBytes)
{
	static QuadWord Now = gTime.GetNow();
	static QuadWord Prev = gTime.GetNow();
	static QuadWord Tick = 0;

	Prev = Now;
	Now = gTime.GetNow();
	mTotalSentBytes += NumBytes;
	mSentBPSCount += NumBytes;
	Tick += Now - Prev;
	while(Tick > gTime.GetFrequency())
	{
		mSentBytesPerSec = (UINT)(mSentBPSCount);
		mSentBPSCount = 0;
		Tick -= gTime.GetFrequency();
	}
}
void Client::AddTotalRecvBytes(UINT NumBytes)
{
	static QuadWord Now = gTime.GetNow();
	static QuadWord Prev = gTime.GetNow();
	static QuadWord Tick = 0;

	Prev = Now;
	Now = gTime.GetNow();
	mTotalRecvBytes += NumBytes;
	mRecvBPSCount += NumBytes;
	Tick += Now - Prev;
	while(Tick > gTime.GetFrequency())
	{
		mRecvBytesPerSec = (UINT)(mRecvBPSCount);
		mRecvBPSCount = 0;
		Tick -= gTime.GetFrequency();
	}
}