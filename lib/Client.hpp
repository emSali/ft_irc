#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ircserver.hpp"

// Contains the information about a specific client inside the server.

class Client {
    public:
        Client() {};

        int get_fd() {return _fd;};
        void set_fd(int fd) {_fd = fd;};
        std::string get_ipAddress() {return _ipAddress;};
        void set_ipAddress(std::string ipAddress) {_ipAddress = ipAddress;};

    private:
        int _fd;
        std::string _ipAddress;
};

#endif
