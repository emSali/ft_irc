#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client {
    public:
        Client();
        ~Client();
        int get_fd();
        void set_fd(int fd);
        std::string get_ipAddress();
        void set_ipAddress(std::string ipAddress);

    private:
        int _fd;
        std::string _ipAddress;
};

#endif