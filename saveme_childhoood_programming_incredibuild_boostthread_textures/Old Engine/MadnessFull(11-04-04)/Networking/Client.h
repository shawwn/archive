#ifndef _CLIENT_H
#define _CLIENT_H

#include <string>
#include "Bsd.h"
#include "Types.h"
using namespace std;

class Client
{
protected:
	SOCKET mSocket;
	string mIP;
	int mPort;

	time_t mTimeout;
	time_t mIdle;
	string mInBuffer;
	string mOutBuffer;
	bool mClose;
	bool mRemove;
public:
	Client();
	~Client();

	bool Make(SOCKET Socket);
	bool IsTimedOut();
	void Touch();
	void Destroy();
	void CloseWhenClear();
	void RemoveWhenClear();

	void RecvData(const Byte *data, int len);
	void SendData(const Byte *data, int len);
	
	const time_t &GetTimeout() { return mTimeout; }
	string &GetInBuffer() { return mInBuffer; }
	string &GetOutBuffer() { return mOutBuffer; }
	SOCKET GetSocket() { return mSocket; }
	const bool &GetClose() { return mClose; }
	const bool &GetRemove() { return mRemove; }
	const string &GetIP() { return mIP; }
	DoubleWord GetNetworkIP() { return (DoubleWord)inet_addr(mIP.c_str()); }
	const int &GetPort() { return mPort; }

	void SetTimeout(const int &Timeout) { mTimeout = Timeout; }	
	
	const Client &operator =(Client c);
};

#endif
