
#ifndef SERVER_HPP
#define SERVER_HPP

# include "ircserver.hpp"
# include "Channel.hpp"

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

	 	static void signal_to_close(int sig) {
			std::cout << "\nSignal to close received!" << std::endl;
			_signal = false;
			(void)sig;
		};

		std::string getHostname() { return _hostname; };
		std::string getPassword() { return _password; };

		void clearClient(int fd);
		std::vector<Client>::iterator getClientIterator(std::string nickname) {
			for (std::vector<Client>::iterator i = _clients.begin(); i != _clients.end(); i++) {
				if (i->getNickname() == nickname) {
					return i;
				}
			}
			return _clients.end();
		};
		Client &getClient(std::string name) {
			for (std::vector<Client>::iterator i = _clients.begin(); i != _clients.end(); i++) {
				if (i->getNickname() == name) {
					return *i;
				}
			}
			return *_clients.end();
		};
		std::vector<Client> &getClients() { return _clients; };

		void addChannel(Channel &c) { _channels.push_back(c); }
		bool findChannel(std::string name) {
			for (std::vector<Channel>::iterator i = _channels.begin(); i != _channels.end(); i++) {
				if (i->getName() == name) {
					return true;
				}
			}
			return false;
		};
		std::vector<Channel>::iterator getChannelIterator(std::string name) {
			for (std::vector<Channel>::iterator i = _channels.begin(); i != _channels.end(); i++) {
				if (i->getName() == name) {
					return i;
				}
			}
			return _channels.end();
		};

		Channel &getChannel(std::string name) {
			for (std::vector<Channel>::iterator i = _channels.begin(); i != _channels.end(); i++) {
				if (i->getName() == name) {
					return *i;
				}
			}
			return *_channels.end();
		};

		std::vector<Channel> &getChannels() { return _channels; };

		void informChannels(Client &s);
		
	private:
		void acceptNewClient();
		void receiveNewData(int fd);

		void Handlemsg(std::string msg, std::vector<Client>::iterator i);

		std::vector<Client> _clients; 
		std::vector<Channel> _channels; 
		std::vector<struct pollfd> _pollFds; // The struct pollfd comes from poll.h, it contains: int fd; short events; short revents;

		void populate_sockaddr_in();
		struct sockaddr_in serv_addr;

		int _port;
		static bool _signal;
		int _serverSocketFd;
		const std::string _password;
		const std::string _hostname;
};

#endif
