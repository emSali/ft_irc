#ifndef SERVER_HPP
#define SERVER_HPP

#include "ircserver.hpp"
#include "Client.hpp"
#include "Channel.hpp"

// _clients keeps track of all the clients and manage their requests
class Server {
    public:
        Server(char *port, char *password);
		~Server() {
            closePollFds();
        };
        void serverInit();
		void serverStart();
        void closePollFds();

		void Handlemsg(std::string msg, int fd);

	 	static void signal_to_close(int sig) {
			std::cout << "\nSignal to close received!" << std::endl;
			_signal = false;
			(void)sig;
		};

    private:
        void acceptNewClient();
        void receiveNewData(int fd);
		
        void clearClient(int fd);


        int _port;
        int _serverSocketFd;
        static bool _signal;
        std::vector<Client> _clients; 
        std::vector<Channel> _channels; 
        std::vector<struct pollfd> _pollFds; // The struct pollfd comes from poll.h, it contains: int fd; short events; short revents;

		struct sockaddr_in serv_addr;
		void populate_sockaddr_in();
		std::string _password;
};

#endif
