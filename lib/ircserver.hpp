#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

# include <iostream>
# include <map>
# include <vector>
# include <unistd.h>
# include <cstdlib>
# include <cstring>
# include <string>
# include <cerrno> 
# include <sstream>
# include <poll.h>
# include <csignal>

# include <netinet/in.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <netdb.h>

std::vector<std::string> split_string(const std::string& input_string, char delimiter);
std::string to_string(int value);

# define MAX_CLIENTS 5

// Structs already declared in library
/*
/ Strcut sockaddr_in6 is for Ipv6, the below is for Ipv4
struct sockaddr_in {
	short int sin_family; // Address family, AF_INET
	unsigned short int sin_port; // Port number
	struct in_addr sin_addr; // Internet address
	unsigned char sin_zero[8]; // Same size as struct sockaddr
};


/ Internet address (a structure for historical reasons)
struct in_addr {
	uint32_t s_addr; // that's a 32-bit int (4 bytes)
};


struct pollfd {
	int fd; // file descriptor
	short events; // requested events
	short revents; // returned events
};

struct sockaddr {
	unsigned short sa_family; // address family, AF_xxx (we want AF_INET for IPv4)
	char sa_data[14]; // 14 bytes of protocol address 
};

struct addrinfo {
	int ai_flags; // AI_PASSIVE, AI_CANONNAME, etc. 
	int ai_family; // AF_INET, AF_INET6, AF_UNSPEC (we want AF_INET for IPv4)
	int ai_socktype; // SOCK_STREAM, SOCK_DGRAM (we want SOCK_STREAM for TCP)
	int ai_protocol; // use 0 for "any" (we want TCP/IP)
	size_t ai_addrlen; // size of ai_addr in bytes
	struct sockaddr *ai_addr; // struct sockaddr_in or _in6 
	char *ai_canonname; // full canonical hostname
	struct addrinfo *ai_next; // linked list, next node
};
We can free the above linked list using freeaddrinfo() i.e.:
struct addrinfo *serverinfo;
freeaddrinfo(serverinfo);

struct sockaddr_storage {
	sa_family_t ss_family; // address family
	// all this is padding, implementation specific, ignore it:
	char __ss_pad1[_SS_PAD1SIZE];
	int64_t __ss_align;
	char __ss_pad2[_SS_PAD2SIZE];
};

*/

#endif