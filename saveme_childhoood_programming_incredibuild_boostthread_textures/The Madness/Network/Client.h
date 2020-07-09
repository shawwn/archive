#ifndef CLIENT_H
#define CLIENT_H

#include "Winsock2.h"
#include "Packet.h"
#include <string>

class Client
{
private:
	SOCKET mSocket;
	sockaddr_in mAddr;
	std::string mInBuffer;
	std::string mOutBuffer;

	UINT mTotalSentBytes;
	UINT mTotalRecvBytes;
	UINT mSentBytesPerSec;
	UINT mRecvBytesPerSec;
	UINT mSentBPSCount;
	UINT mRecvBPSCount;

	bool mClosing;
public:
	Client(SOCKET Socket, sockaddr_in *Addr) : 
	  mClosing(false), mSocket(Socket), mTotalSentBytes(0), mTotalRecvBytes(0), mSentBytesPerSec(0), mRecvBytesPerSec(0), mSentBPSCount(0), mRecvBPSCount(0)
	  { if(Addr) { memcpy(&mAddr,Addr,sizeof(sockaddr_in)); } mInBuffer.reserve(1000); mOutBuffer.reserve(1000); }
	~Client() { 
#ifdef WIN32
		closesocket(mSocket);
#else
		close(mSocket);
#endif
	}

	void Send(Packet *pack);
	void Close(const std::string &str = "");
	
	inline SOCKET GetSocket() const { return mSocket; }
	inline std::string &GetInBuffer() { return mInBuffer; }
	inline std::string &GetOutBuffer() { return mOutBuffer; }
	inline UINT GetTotalSentBytes() const { return mTotalSentBytes; }
	inline UINT GetTotalRecvBytes() const { return mTotalRecvBytes; }
	inline UINT GetSentBytesPerSecond() const { return mSentBytesPerSec; }
	inline UINT GetRecvBytesPerSecond() const { return mRecvBytesPerSec; }
	void AddTotalSentBytes(UINT NumBytes);
	void AddTotalRecvBytes(UINT NumBytes);
	inline std::string AddressString() const { return inet_ntoa(mAddr.sin_addr); }
	inline in_addr AddressNetwork() const { return mAddr.sin_addr; }
	inline sockaddr_in &GetSockaddr() { return mAddr; }
	inline bool IsClosing() const { return mClosing; }
};

#endif