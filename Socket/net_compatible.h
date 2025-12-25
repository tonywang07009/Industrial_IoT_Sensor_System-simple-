#ifndef _NET_COMPAT_H
#define _NET_COMPAT_H

#if defined(_WIN32) || defined(_WIN64)

#include <winsock2.h>
#include <ws2tcpip.h>

typedef SOCKET net_socket_t;

#define net_close closesocket

static inline int net_init(void) // The sock close
{
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2, 2), &wsa);
}

static inline void net_cleanup(void)
{
    WSACleanup();
}
#else // for linux mac

#include <sys/types.h>
#include <sys/time.h> // the sys time.h for timeval
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef int net_socket_t;

#define net_close close

static inline int net_init(void)
{
    return 0;
}
static inline void net_cleanup(void)
{
}

#endif

#endif