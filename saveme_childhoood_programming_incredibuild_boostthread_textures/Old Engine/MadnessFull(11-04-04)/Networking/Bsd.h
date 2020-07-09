#ifndef _LINUX_H
#define _LINUX_H

#ifdef WIN32
#include <winsock2.h>
#else
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
#define stricmp strcasecmp
#endif

#endif
