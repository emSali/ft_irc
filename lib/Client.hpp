#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ircserver.hpp"

// Contains the information about a specific client inside the server.

class Client {
    public:
        Client(sockaddr_in addr, int fd) : _fd(fd),  client_addr(addr) {};

        int getFd() {return _fd;};
        sockaddr_in getClient_addr() {return client_addr;};

    private:
        int _fd;
		sockaddr_in client_addr;
};

#endif
