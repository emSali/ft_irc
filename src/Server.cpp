#include "../lib/Server.hpp"

// globally define _signal bool
Server::Server(char *port, char *password)
{
	char *s;
	long int port_int = std::strtol(port, &s, 10);
	if (port_int < 1024 || port_int > 49151 || s[0] != '\0')
		throw std::string("Invalid port number: " + std::string(port));
	else
		this->_port = port_int;

	this->_password = password;
}

bool Server::_signal = false;

void Server::populate_sockaddr_in() {
	this->serv_addr.sin_family = AF_INET; // TCP
	this->serv_addr.sin_port = htons(this->_port); // short, network byte order
	this->serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	std::memset(this->serv_addr.sin_zero, '\0', sizeof (this->serv_addr.sin_zero));
}
// Initialise the server
void Server::serverInit() {

	// We use AF_INET because it's the only compatible with IPv4
	// We use stream socket because it's the only compatible with TCP
	// We use IPPROTO_TCP because it's the only compatible with TCP (same as getprotobyname("tcp"))
	this->_serverSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->_serverSocketFd == -1)
		throw std::string("Error creating socket");
	else
		std::cout << "Socket created : " << this->_serverSocketFd << std::endl;

	fcntl(this->_serverSocketFd, F_SETFL, O_NONBLOCK); // Makes the socket non-blocking, as the subject requires

	// We bind the socket to the port
	populate_sockaddr_in();
	int b = bind(this->_serverSocketFd, (struct sockaddr *)&this->serv_addr, sizeof(this->serv_addr));
	if (b != 0)
		throw std::string("Error binding socket");
	else
		std::cout << "Socket binded to port : " << this->_port << std::endl;
	
	// We listen to the socket
	int l = listen(this->_serverSocketFd, 2);
	if (l != 0)
		throw std::string("Error listening to socket");
	else
		std::cout << "Socket listening : " << this->_port << std::endl;
}

void Server::serverAccept() {

	std::cout << "Waiting for connection..." << std::endl;

	socklen_t addr_size = (socklen_t) sizeof this->serv_addr;
	int p = poll(_pollFds.data(), _pollFds.size(), -1);
	if (p == -1)
		throw std::string("Error poll");
	else
		std::cout << "Poll accepted : " << p << std::endl;

	int a = accept(this->_serverSocketFd, (struct sockaddr *)&this->serv_addr, &addr_size);
	if (a == -1)
		throw std::string("Error accepting connection");
	else
		std::cout << "Socket accepted : " << a << std::endl;
	
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
