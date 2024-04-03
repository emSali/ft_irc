#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserver.hpp"
#include "Client.hpp"

// _clients keeps track of all the clients and manage their requests

class Server {
    public:
        Server() : _serverSocketFd(-1) {
        };
		~Server() {
            if (_serverSocketFd != -1)
				close(_serverSocketFd);
        };

        void serverInit(char *port, char *password);
		void serverAccept();
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

		struct sockaddr_in serv_addr;
		void populate_sockaddr_in();
		std::string _password;
};

#endif
