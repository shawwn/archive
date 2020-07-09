#ifndef SERVERMACROS_H
#define SERVERMACROS_H

// Winsock versus berk sockets
#ifdef WIN32

#define netclose( socket ) closesocket( socket )
#define netlisten( socket ) listen( socket, SOMAXCONN )
#define netnonblock( socket ) unsigned long arg = 2; ioctlsocket( socket, FIONBIO, &arg )
#define netsocklen int

#include <winsock2.h>

#else

#define netclose( socket ) close(socket )
#define netlisten( socket ) listen( socket, 20 )
#define netnonblock( socket ) unsigned long arg = 2; ioctl( socket FIONBIO, &arg)
#define netsocklen socklen_t

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define SOCKET int
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#endif

#endif