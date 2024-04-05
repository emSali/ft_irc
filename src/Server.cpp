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

	this->_signal = true;
}

bool Server::_signal = false;

void Server::populate_sockaddr_in() {
	this->serv_addr.sin_family = AF_INET; // IPv4
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

	// We set the socket to reuse the address
	int opt = 1;
	int s = setsockopt(this->_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (s != 0)
		throw std::string("Error setting socket options");
	else
		std::cout << "Socket options set : " << this->_serverSocketFd << std::endl;

	// We set the socket to non-blocking
	fcntl(this->_serverSocketFd, F_SETFL, O_NONBLOCK);

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

	// We add the server socket to the pollfds
	struct pollfd ServerPoll;
	ServerPoll.fd = this->_serverSocketFd;
	ServerPoll.events = POLLIN;
	ServerPoll.revents = 0;
	_pollFds.push_back(ServerPoll);
}

void Server::serverStart() {


	std::cout << "Waiting for connection..." << std::endl;
	while (this->_signal)
	{
		if (poll(&_pollFds[0], _pollFds.size(), -1) != 0 && this->_signal)
		{
			for (size_t i = 0; i < _pollFds.size(); i++) // Check for all fd's do see new info
			{
				if (_pollFds[i].revents & POLLIN) // If there is new info
				{
					if (_pollFds[i].fd == this->_serverSocketFd) // If the new info is from the server socket (therefor is a new client)
						acceptNewClient();
					else
						receiveNewData(_pollFds[i].fd); // If the new info is from a client
				}
			}
		}
		else
			break ;
	}
	std::cout << "Server closing..." << std::endl;	
}

// Accept a new client
void Server::acceptNewClient() {
	struct sockaddr_in client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
	int newClientFd = accept(this->_serverSocketFd, (struct sockaddr *)&client_addr, &client_addr_size);
	if (newClientFd == -1)
		throw std::string("Error accepting new client");
	else
		std::cout << "New client accepted : " << newClientFd << std::endl;

	// We set the new client to non-blocking
	fcntl(newClientFd, F_SETFL, O_NONBLOCK);

	// We add the new client to the pollfds
	struct pollfd ClientPoll;
	ClientPoll.fd = newClientFd;
	ClientPoll.events = POLLIN;
	ClientPoll.revents = 0;
	_pollFds.push_back(ClientPoll);
	this->_clients.push_back(Client(client_addr, newClientFd));
	std::cout << "New client " << newClientFd << " Connected!" << std::endl;

}

// Receive new data from a registered client
void Server::receiveNewData(int fd) {

	// read from client
	std::string msg;
	char buffer[1024];
	int bytes = 1;
	while ((bytes = recv(fd, buffer, 1024, 0)) > 0)
	{
		msg.append(buffer, bytes);
		if (msg.find("\n") != std::string::npos)
			break;
	}
	if (bytes == 0) {
		std::cout << "Client " << fd << " disconnected" << std::endl;
		clearClient(fd);
		close(fd);
	}
	else if (bytes == -1)
		std::cerr << "Error receiving data from client: " << fd << std::endl;
	else {
		std::cout << "Client " << fd << " says: " << msg;
	}
	msg.clear();
}

// Close the files descriptors. Clients and server socket.
void Server::closePollFds() {

	for (std::vector<Client>::iterator i = _clients.begin(); i != _clients.end(); i++)
	{
		std::cout << "client fd deleted: " << i->get_fd() << std::endl;
		close(i->get_fd());
	}
	if (_serverSocketFd != -1)
	{
		std::cout << "socket fd deleted: " << _serverSocketFd << std::endl;
		close(_serverSocketFd);
	}
}

// Remove the client with fd from _clients and _pollFds

void Server::clearClient(int fd)
{
	for (std::vector<Client>::iterator i = this->_clients.begin(); i != this->_clients.end(); i++)
	{
		if (i->get_fd() == fd)
		{
			close(fd);
			this->_clients.erase(i);
			break;
		}
	}
	for (std::vector<struct pollfd>::iterator i = this->_pollFds.begin(); i != this->_pollFds.end(); i++)
	{
		if (i->fd == fd)
		{
			this->_pollFds.erase(i);
			break;
		}
	}
	std::cout << "Client " << fd << " removed sucessfully" << std::endl;
}

// void Server::clearClient(int fd) {
// 	for(size_t i = 0; i <_pollFds.size(); i++){
// 		if (_pollFds[i].fd == fd) {
// 			_pollFds.erase(_pollFds.begin() + i);
// 			break;
// 		}
// 	}
// 	for(size_t i = 0; i < _clients.size(); i++){
// 		if (_clients[i].get_fd() == fd){
// 			_clients.erase(_clients.begin() + i);
// 			break;
// 		}
// 	}
// 	std::cout << "Closed client: " << fd << std::endl;
// 	close(fd);
// }
