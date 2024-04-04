#include "../lib/Server.hpp"

// globally define _signal bool
bool Server::_signal = false;

// Initialise the server
void Server::serverInit() {
	serverSocket();
	closePollFds();
}

// Creates the socket
void Server::serverSocket() {
	// We use AF_INET because it's the only compatible with IPv4
	// We use stream socket because it's the only compatible with TCP
	// We use IPPROTO_TCP because it's the only compatible with TCP (same as getprotobyname("tcp"))
	_serverSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_serverSocketFd == -1) {
		throw(std::runtime_error("Failed to create socket"));
	}
	fcntl(_serverSocketFd, F_SETFL, O_NONBLOCK); // Makes the socket non-blocking, as the subject requires
	
	std::cout << "socket fd created: " << _serverSocketFd << std::endl;
}

// Accept a new client
void Server::acceptNewClient() {

}

// Receive new data from a registered client
void Server::receiveNewData(int fd) {
    (void)fd;
}

// Close the files descriptors. Clients and server socket.
void Server::closePollFds() {
	for (size_t i = 0; i < _clients.size(); i++) {
		close(_clients[i].get_fd());
	}
	if (_serverSocketFd != -1) {
		std::cout << "socket fd deleted: " << _serverSocketFd << std::endl;
		close(_serverSocketFd);
	}
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
