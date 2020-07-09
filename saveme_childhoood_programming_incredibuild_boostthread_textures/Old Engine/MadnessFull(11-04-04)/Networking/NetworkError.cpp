#include "stdafx.h"
#include "NetworkError.h"
#include "LoggingInterface.h"

int N(const char *Function, const int &Value)
{
#ifdef WIN32
	switch(Value)
	{
	case 0:
		break;
	case SOCKET_ERROR:
		return N(Function,WSAGetLastError());
		break;
	case WSAEINTR:
		gLog->Stream << Function << ": WSAEINTR.  A blocking operation was interrupted by a call to WSACancelBlockingCall.";
		gLog->Print();
		break;
	case WSAEACCES:
		gLog->Stream << Function << ": WSAEACCES.  An attempt was made to access a socket in a way forbidden by its access permissions. An example is using a broadcast address for sendto without broadcast permission being set using setsockopt(SO_BROADCAST).  Another possible reason for the WSAEACCES error is that when the bind function is called (on Windows NT 4 SP4 or later), another application, service, or kernel mode driver is bound to the same address with exclusive access. Such exclusive access is a new feature of Windows NT 4 SP4 and later, and is implemented by using the SO_EXCLUSIVEADDRUSE option.";
		gLog->Print();
		break;
	case WSAEFAULT:
		gLog->Stream << Function << ": WSAEFAULT.  The system detected an invalid pointer address in attempting to use a pointer argument of a call. This error occurs if an application passes an invalid pointer value, or if the length of the buffer is too small. For instance, if the length of an argument, which is a sockaddr structure, is smaller than the sizeof(sockaddr).";
		gLog->Print();
		break;
	case WSAEINVAL:
		gLog->Stream << Function << ": WSAEINVAL.  Some invalid argument was supplied (for example, specifying an invalid level to the setsockopt function). In some instances, it also refers to the current state of the socket—for instance, calling accept on a socket that is not listening.";
		gLog->Print();
		break;
	case WSAEMFILE:
		gLog->Stream << Function << ": WSAEMFILE.  Too many open sockets. Each implementation may have a maximum number of socket handles available, either globally, per process, or per thread.";
		gLog->Print();
		break;
	case WSAEWOULDBLOCK:
		gLog->Stream << Function << ": WSAEWOULDBLOCK.  This error is returned from operations on nonblocking sockets that cannot be completed immediately, for example recv when no data is queued to be read from the socket. It is a nonfatal error, and the operation should be retried later. It is normal for WSAEWOULDBLOCK to be reported as the result from calling connect on a nonblocking SOCK_STREAM socket, since some time must elapse for the connection to be established.";
		gLog->Print();
		break;
	case WSAEINPROGRESS:
		gLog->Stream << Function << ": WSAEINPROGRESS.  A blocking operation is currently executing. Windows Sockets only allows a single blocking operation—per- task or thread—to be outstanding, and if any other function call is made (whether or not it references that or any other socket) the function fails with the WSAEINPROGRESS error.";
		gLog->Print();
		break;
	case WSAEALREADY:
		gLog->Stream << Function << ": WSAEALREADY.  An operation was attempted on a nonblocking socket with an operation already in progress—that is, calling connect a second time on a nonblocking socket that is already connecting, or canceling an asynchronous request (WSAAsyncGetXbyY) that has already been canceled or completed.";
		gLog->Print();
		break;
	case WSAENOTSOCK:
		gLog->Stream << Function << ": WSAENOTSOCK.  An operation was attempted on something that is not a socket. Either the socket handle parameter did not reference a valid socket, or for select, a member of an fd_set was not valid.";
		gLog->Print();
		break;
	case WSAEDESTADDRREQ:
		gLog->Stream << Function << ": WSAEDESTADDRREQ.  A required address was omitted from an operation on a socket. For example, this error is returned if sendto is called with the remote address of ADDR_ANY.";
		gLog->Print();
		break;
	case WSAEMSGSIZE:
		gLog->Stream << Function << ": WSAEMSGSIZE.  A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram was smaller than the datagram itself.";
		gLog->Print();
		break;
	case WSAEPROTOTYPE:
		gLog->Stream << Function << ": WSAEPROTOTYPE.  A protocol was specified in the socket function call that does not support the semantics of the socket type requested. For example, the ARPA Internet UDP protocol cannot be specified with a socket type of SOCK_STREAM.";
		gLog->Print();
		break;
	case WSAENOPROTOOPT:
		gLog->Stream << Function << ": WSAENOPROTOOPT.  An unknown, invalid or unsupported option or level was specified in a getsockopt or setsockopt call.";
		gLog->Print();
		break;
	case WSAEPROTONOSUPPORT:
		gLog->Stream << Function << ": WSAEPROTONOSUPPORT.  The requested protocol has not been configured into the system, or no implementation for it exists. For example, a socket call requests a SOCK_DGRAM socket, but specifies a stream protocol.";
		gLog->Print();
		break;
	case WSAESOCKTNOSUPPORT:
		gLog->Stream << Function << ": WSAESOCKTNOSUPPORT.  The support for the specified socket type does not exist in this address family. For example, the optional type SOCK_RAW might be selected in a socket call, and the implementation does not support SOCK_RAW sockets at all.";
		gLog->Print();
		break;
	case WSAEOPNOTSUPP:
		gLog->Stream << Function << ": WSAEOPNOTSUPP.  The attempted operation is not supported for the type of object referenced. Usually this occurs when a socket descriptor to a socket that cannot support this operation is trying to accept a connection on a datagram socket.";
		gLog->Print();
		break;
	case WSAEPFNOSUPPORT:
		gLog->Stream << Function << ": WSAEPFNOSUPPORT.  The protocol family has not been configured into the system or no implementation for it exists. This message has a slightly different meaning from WSAEAFNOSUPPORT. However, it is interchangeable in most cases, and all Windows Sockets functions that return one of these messages also specify WSAEAFNOSUPPORT.";
		gLog->Print();
		break;
	case WSAEAFNOSUPPORT:
		gLog->Stream << Function << ": WSAEAFNOSUPPORT.  An address incompatible with the requested protocol was used. All sockets are created with an associated address family (that is, AF_INET for Internet Protocols) and a generic protocol type (that is, SOCK_STREAM). This error is returned if an incorrect protocol is explicitly requested in the socket call, or if an address of the wrong family is used for a socket, for example, in sendto.";
		gLog->Print();
		break;
	case WSAEADDRINUSE:
		gLog->Stream << Function << ": WSAEADDRINUSE.  Typically, only one usage of each socket address (protocol/IP address/port) is permitted. This error occurs if an application attempts to bind a socket to an IP address/port that has already been used for an existing socket, or a socket that was not closed properly, or one that is still in the process of closing. For server applications that need to bind multiple sockets to the same port number, consider using setsockopt (SO_REUSEADDR). Client applications usually need not call bind at all— connect chooses an unused port automatically. When bind is called with a wildcard address (involving ADDR_ANY), a WSAEADDRINUSE error could be delayed until the specific address is committed. This could happen with a call to another function later, including connect, listen, WSAConnect, or WSAJoinLeaf.";
		gLog->Print();
		break;
	case WSAEADDRNOTAVAIL:
		gLog->Stream << Function << ": WSAEADDRNOTAVAIL.  The requested address is not valid in its context. This normally results from an attempt to bind to an address that is not valid for the local computer. This can also result from connect, sendto, WSAConnect, WSAJoinLeaf, or WSASendTo when the remote address or port is not valid for a remote computer (for example, address or port 0).";
		gLog->Print();
		break;
	case WSAENETDOWN:
		gLog->Stream << Function << ": WSAENETDOWN.  A socket operation encountered a dead network. This could indicate a serious failure of the network system (that is, the protocol stack that the Windows Sockets DLL runs over), the network interface, or the local network itself.";
		gLog->Print();
		break;
	case WSAENETUNREACH:
		gLog->Stream << Function << ": WSAENETUNREACH.  A socket operation was attempted to an unreachable network. This usually means the local software knows no route to reach the remote host.";
		gLog->Print();
		break;
	case WSAENETRESET:
		gLog->Stream << Function << ": WSAENETRESET.  The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress. It can also be returned by setsockopt if an attempt is made to set SO_KEEPALIVE on a connection that has already failed.";
		gLog->Print();
		break;
	case WSAECONNABORTED:
		gLog->Stream << Function << ": WSAECONNABORTED.  An established connection was aborted by the software in your host computer, possibly due to a data transmission time-out or protocol error.WSAEPROTONOSUPPORT";
		gLog->Print();
		break;
	case WSAECONNRESET:
		gLog->Stream << Function << ": WSAECONNRESET.  An existing connection was forcibly closed by the remote host. This normally results if the peer application on the remote host is suddenly stopped, the host is rebooted, the host or remote network interface is disabled, or the remote host uses a hard close (see setsockopt for more information on the SO_LINGER option on the remote socket). This error may also result if a connection was broken due to keep-alive activity detecting a failure while one or more operations are in progress. Operations that were in progress fail with WSAENETRESET. Subsequent operations fail with WSAECONNRESET.";
		gLog->Print();
		break;
	case WSAENOBUFS:
		gLog->Stream << Function << ": WSAENOBUFS.  An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full.";
		gLog->Print();
		break;
	case WSAEISCONN:
		gLog->Stream << Function << ": WSAEISCONN.  A connect request was made on an already-connected socket. Some implementations also return this error if sendto is called on a connected SOCK_DGRAM socket (for SOCK_STREAM sockets, the to parameter in sendto is ignored) although other implementations treat this as a legal occurrence.";
		gLog->Print();
		break;
	case WSAENOTCONN:
		gLog->Stream << Function << ": WSAENOTCONN.  A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using sendto) no address was supplied. Any other type of operation might also return this error—for example, setsockopt setting SO_KEEPALIVE if the connection has been reset.";
		gLog->Print();
		break;
	case WSAESHUTDOWN:
		gLog->Stream << Function << ": WSAESHUTDOWN.  A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call. By calling shutdown a partial close of a socket is requested, which is a signal that sending or receiving, or both have been discontinued.";
		gLog->Print();
		break;
	case WSAETIMEDOUT:
		gLog->Stream << Function << ": WSAETIMEDOUT:.  A connection attempt failed because the connected party did not properly respond after a period of time, or the established connection failed because the connected host has failed to respond.";
		gLog->Print();
		break;
	case WSAECONNREFUSED:
		gLog->Stream << Function << ": WSAECONNREFUSED.  No connection could be made because the target computer actively refused it. This usually results from trying to connect to a service that is inactive on the foreign host—that is, one with no server application running.";
		gLog->Print();
		break;
	case WSAEHOSTDOWN:
		gLog->Stream << Function << ": WSAEHOSTDOWN.  A socket operation failed because the destination host is down. A socket operation encountered a dead host. Networking activity on the local host has not been initiated. These conditions are more likely to be indicated by the error WSAETIMEDOUT.";
		gLog->Print();
		break;
	case WSAEHOSTUNREACH:
		gLog->Stream << Function << ": WSAEHOSTUNREACH.  A socket operation was attempted to an unreachable host. See WSAENETUNREACH.";
		gLog->Print();
		break;
	case WSAEPROCLIM:
		gLog->Stream << Function << ": WSAEPROCLIM.  A Windows Sockets implementation may have a limit on the number of applications that can use it simultaneously.WSAStartup may fail with this error if the limit has been reached.";
		gLog->Print();
		break;
	case WSASYSNOTREADY:
		gLog->Stream << Function << ": WSASYSNOTREADY.  This error is returned by WSAStartup if the Windows Sockets implementation cannot function at this time because the underlying system it uses to provide network services is currently unavailable. Users should check:  That the appropriate Windows Sockets DLL file is in the current path.  That they are not trying to use more than one Windows Sockets implementation simultaneously. If there is more than one Winsock DLL on your system, be sure the first one in the path is appropriate for the network subsystem currently loaded.  The Windows Sockets implementation documentation to be sure all necessary components are currently installed and configured correctly.";
		gLog->Print();
		break;
	case WSANOTINITIALISED:
		gLog->Stream << Function << ": WSANOTINITIALISED.  Either the application has not called WSAStartup or WSAStartup failed. The application may be accessing a socket that the current active task does not own (that is, trying to share a socket between tasks), or WSACleanup has been called too many times.";
		gLog->Print();
		break;
	case WSAVERNOTSUPPORTED:
		gLog->Stream << Function << ": WSAVERNOTSUPPORTED.  The current Windows Sockets implementation does not support the Windows Sockets specification version requested by the application. Check that no old Windows Sockets DLL files are being accessed.";
		gLog->Print();
		break;
	case WSAEDISCON:
		gLog->Stream << Function << ": WSAEDISCON.  Returned by WSARecv and WSARecvFrom to indicate that the remote party has initiated a graceful shutdown sequence.";
		gLog->Print();
		break;
	case WSATYPE_NOT_FOUND:
		gLog->Stream << Function << ": WSATYPE_NOT_FOUND.  The specified class was not found.";
		gLog->Print();
		break;
	case WSAHOST_NOT_FOUND:
		gLog->Stream << Function << ": WSAHOST_NOT_FOUND.  No such host is known. The name is not an official host name or alias, or it cannot be found in the database(s) being queried. This error may also be returned for protocol and service queries, and means that the specified name could not be found in the relevant database.";
		gLog->Print();
		break;
	case WSATRY_AGAIN:
		gLog->Stream << Function << ": WSATRY_AGAIN.  This is usually a temporary error during host name resolution and means that the local server did not receive a response from an authoritative server. A retry at some time later may be successful.";
		gLog->Print();
		break;
	case WSANO_RECOVERY:
		gLog->Stream << Function << ": WSANO_RECOVERY.  This indicates that some sort of nonrecoverable error occurred during a database lookup. This may be because the database files (for example, BSD-compatible HOSTS, SERVICES, or PROTOCOLS files) could not be found, or a DNS request was returned by the server with a severe error.";
		gLog->Print();
		break;
	case WSANO_DATA:
		gLog->Stream << Function << ": WSANO_DATA.  The requested name is valid and was found in the database, but it does not have the correct associated data being resolved for. The usual example for this is a host name-to-address translation attempt (using gethostbyname or WSAAsyncGetHostByName) which uses the DNS (Domain Name Server). An MX record is returned but no A record—indicating the host itself exists, but is not directly reachable.";
		gLog->Print();
		break;
	case WSA_INVALID_HANDLE:
		gLog->Stream << Function << ": WSA_INVALID_HANDLE.  An application attempts to use an event object, but the specified handle is not valid.";
		gLog->Print();
		break;
	case WSA_INVALID_PARAMETER:
		gLog->Stream << Function << ": WSA_INVALID_PARAMETER.  An application used a Windows Sockets function which directly maps to a Windows function. The Windows function is indicating a problem with one or more parameters.";
		gLog->Print();
		break;
	case WSA_IO_INCOMPLETE:
		gLog->Stream << Function << ": WSA_IO_INCOMPLETE.  The application has tried to determine the status of an overlapped operation which is not yet completed. Applications that use WSAGetOverlappedResult (with the fWait flag set to FALSE) in a polling mode to determine when an overlapped operation has completed, get this error code until the operation is complete.";
		gLog->Print();
		break;
	case WSA_IO_PENDING:
		gLog->Stream << Function << ": WSA_IO_PENDING.  The application has initiated an overlapped operation that cannot be completed immediately. A completion indication will be given later when the operation has been completed.";
		gLog->Print();
		break;
	case WSA_NOT_ENOUGH_MEMORY:
		gLog->Stream << Function << ": WSA_NOT_ENOUGH_MEMORY.  An application used a Windows Sockets function that directly maps to a Windows function. The Windows function is indicating a lack of required memory resources.";
		gLog->Print();
		break;
	case WSA_OPERATION_ABORTED:
		gLog->Stream << Function << ": WSA_OPERATION_ABORTED.  An overlapped operation was canceled due to the closure of the socket, or the execution of the SIO_FLUSH command in WSAIoctl.";
		gLog->Print();
		break;
	case WSASYSCALLFAILURE:
		gLog->Stream << Function << ": WSASYSCALLFAILURE.  Generic error code, returned under various conditions.  Returned when a system call that should never fail does fail. For example, if a call to WaitForMultipleEvents fails or one of the registry functions fails trying to manipulate the protocol/namespace catalogs.  Returned when a provider does not return SUCCESS and does not provide an extended error code. Can indicate a service provider implementation error.";
		gLog->Print();
		break;
	default:
		gLog->Stream << Function << ": Unknown error.";
		gLog->Print();
		break;
	}
	return Value;
#else
	if(Value != 0)
	{
		gLog->Stream << Function << ": Error.  Returned " << Value;
		gLog->Print();
	}
	return Value;
#endif
}

SOCKET NSock(const char *Function, const SOCKET &s)
{
#ifdef WIN32
	if(s == INVALID_SOCKET) {
		N(Function,WSAGetLastError());
		return INVALID_SOCKET;
	}
	else
	{
		return s;
	}
#else
	return s;
#endif
}
