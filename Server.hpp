#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>

#include "Client.hpp"

struct pollfd {
};

class Server {
    public:
        Server();
        ~Server();
        void serverInit();
        void serverSocket();
        void acceptNewClient();
        void receiveNewData(int fd);

        static void signalHandler(int signum);

        void closePollFds();
        void clearClients(int fd);

    private:
        int _port;
        int _serverSocketFd;
        static bool _signal;
        std::vector<Client> _clients;
        std::vector<struct pollfd> _pollFds;
};

#endif