#include "stdafx.h"
#include "Client.h"
#include "LoggingInterface.h"
#include <time.h>

Client::Client()
{
	Destroy();
}
Client::~Client()
{
	Destroy();
}

bool Client::Make(SOCKET Socket)
{
	if(Socket == INVALID_SOCKET)
		return false;
	mSocket = Socket;
	sockaddr_in Name;
#ifdef WIN32
	int size;
#else
	socklen_t size;
#endif
	size = sizeof(sockaddr_in);
	getpeername(Socket,(sockaddr *)&Name,&size);

	mIP = inet_ntoa(Name.sin_addr);
	mPort = ntohs(Name.sin_port);
	return true;
}

void Client::RecvData(const Byte *data, int len)
{
	for(int i = 0; i < len; i++)
		mInBuffer += data[i];
}

void Client::SendData(const Byte *data, int len)
{
	for(int i = 0; i < len; i++)
		mOutBuffer += data[i];
}

bool Client::IsTimedOut()
{
	if(time(NULL)-mIdle > mTimeout)
		return true;
	return false;
}

void Client::Touch()
{
	mIdle = time(NULL);
}

void Client::Destroy()
{
	mClose = false;
	mRemove = false;
	mSocket = 0;
	mIdle = time(NULL);
	mTimeout = 10000;
	mInBuffer = "";
	mOutBuffer = "";
}

void Client::CloseWhenClear()
{
	mClose = true;
}

void Client::RemoveWhenClear()
{
	mRemove = true;
}

const Client &Client::operator =(Client c)
{
	mSocket = c.GetSocket();
	mIdle = time(NULL);
	mTimeout = 10000;
	mInBuffer = c.GetInBuffer();
	mOutBuffer = c.GetOutBuffer();

	return *this;
}
