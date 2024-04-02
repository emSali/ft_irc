#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserver.hpp"
#include "Client.hpp"

// struct pollfd {
// 	int fd;
// };

// There's already a struct pollfd in poll.h, which has:
// int fd; // file descriptor
// short events; // requested events
// short revents; // returned events

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
