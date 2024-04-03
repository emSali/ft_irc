#include "../lib/Server.hpp"

// Initialise the server
void Server::serverInit() {

}

// Creates the socket
void Server::serverSocket() {

}

// Accept a new client
void Server::acceptNewClient() {

}

// Receive new data from a registered client
void Server::receiveNewData(int fd) {
    (void)fd;
}

// Close the files descriptors
void Server::closePollFds() {

}

// Remove the client with fd from _clients and _pollFds
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
