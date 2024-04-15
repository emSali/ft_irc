#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

// Libraries
# include <iostream>
# include <iomanip>
# include <map>
# include <vector>
# include <unistd.h>
# include <cstdlib>
# include <cstring>
# include <string>
# include <cerrno> 
# include <sstream>
# include <csignal>
# include <climits>

// Socket Libraries
# include <poll.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <netdb.h>

// Config
# define MAX_LISTEN_QUEUE 5
# define HOSTNAME "OurIRCServer"
# define MAX_NICKNAME 10
# define MAX_USERNAME 10
# define MAX_CHANNELS_PER_USER 5

// Password
# define ERR_NEEDMOREPARAMS "461"
# define NEED_MORE_PARAMS "Need more parameters"

// Nickname
# define ERR_NONICKNAMEGIVEN "431"
# define NO_NICKNAME_GIVEN "No nickname given"
# define ERR_NICKNAMEINUSE "433"
# define NICKNAME_IN_USE "Nickname is already in use"
# define ERR_ERRONEUSNICKNAME "432"
# define ERRONEUS_NICKNAME "Nickname is erroneus"

// User
# define ERR_REREGISTER "462"
# define REREGISTER "You may not reregister"

// Authentication
# define ERR_ALREADYREGISTRED "462"
# define ALREADY_REGISTRED "Already registred"
# define ERR_BADPASSWORD "464"
# define BAD_PASSWORD "Password incorrect"
# define RPL_WELCOME "001"
# define WELCOME "Welcome to the OurIRC :D"

// Join
# define RPL_TOPIC "332"
# define RPL_NOTOPIC "331"

# define RPL_TOPICTIME "333"
# define RPL_NAMREPLY "353"
# define ERR_NOSUCHCHANNEL "403"
# define ERR_NOTONCHANNEL "442"
# define ERR_USERONCHANNEL "443"

# define RPL_LISTSTART "321"
# define RPL_LIST "322"
# define RPL_LISTEND "323"

# define ERR_TOOMANYCHANNELS "405"
# define TOO_MANY_CHANNELS "You have joined too many channels"
# define ERR_CHANNELISFULL "471"
# define CHANNEL_IS_FULL "Channel is full"
# define ERR_INVITEONLYCHAN "473"
# define INVITE_ONLY_CHAN "Invite only channel"
# define ERR_BADCHANNELKEY "475"
# define BAD_CHANNEL_KEY "Bad channel key"

# define ERR_CHANOPRIVSNEEDED "482"
# define ERR_NOSUCHNICK "401"
# define ERR_BANNEDFROMCHAN "474"
# define BANNED_FROM_CHAN "Banned from channel"

// Msg to server
# define CMD_ERR(c_fd, cmd, err) "[SERVER] <" + to_string(c_fd) + "> "+ cmd +" ERROR: " + err + "\n"
# define CMD_SET(c_fd, cmd, arg) "[SERVER] <" + to_string(c_fd) + "> Has set " + cmd + " to " + arg

// Send message to client
# define MSG_END "\r\n"
# define IRCsend(fd, s) {if (send(fd, s.c_str(), s.size(), 0) == -1) std::cerr << "Error sending message to client: " << fd << std::endl;}
# define GEN_MSG(err, msg, nick) std::string(":" + std::string(HOSTNAME) + " " + err + " " + nick + " :" + msg + MSG_END)
# define CHL_MSG(code,nick) std::string(":" + std::string(HOSTNAME) + " " + code + " " + nick + " ")
# define PRIV_MSG(sender, destination, msg) std::string(":" + sender + " PRIVMSG " + destination + " :" + msg + MSG_END)

# include "Client.hpp"
std::vector<std::string> split_string(const std::string& input_string, char delimiter);
std::string to_string(int value);
void CommandInfo(Client &c, std::vector<std::string> args, std::string err_nb, std::string err_msg);

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