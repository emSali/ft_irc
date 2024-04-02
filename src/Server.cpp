#include "../lib/Server.hpp"

Server::Server() {
    (void)_port;
    _serverSocketFd = -1;
}

Server::~Server() {}

void Server::serverInit() {}

void Server::serverSocket() {}

void Server::acceptNewClient() {}

void Server::receiveNewData(int fd) {
    (void)fd;
}

void Server::closePollFds() {}

//remove remove client with fd from _clients and _pollFds
void Server::clearClients(int fd) {
	for(size_t i = 0; i <_pollFds.size(); i++){
		if (_pollFds[i].fd == fd) {
			_pollFds.erase(_pollFds.begin() + i);
			break;
		}
	}
	for(size_t i = 0; i < _clients.size(); i++){
		if (_clients[i].get_fd() == fd){
			_clients.erase(_clients.begin() + i);
			break;
		}
	}
}
