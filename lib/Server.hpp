#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserver.hpp"
#include "Client.hpp"

class Server {
    public:
        Server() {_serverSocketFd = -1;};

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
        std::vector<struct pollfd> _pollFds; // The struct pollfd comes from poll.h, it contains: int fd; short events; short revents;
};

#endif
