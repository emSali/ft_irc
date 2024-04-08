#include "../lib/Server.hpp"
#include "../lib/Commands.hpp"

// globally define _signal bool
Server::Server(char *port, char *password) : _password(password), _hostname(HOSTNAME)
{
	long int port_int = std::strtol(port, NULL, 10);
	this->_port = port_int;
	this->_serverSocketFd = -1;
}

bool Server::_signal = true;

void Server::populate_sockaddr_in() {
	this->serv_addr.sin_family = AF_INET; // IPv4
	this->serv_addr.sin_port = htons(this->_port); // short, network byte order
	this->serv_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
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
	if (setsockopt(this->_serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0)
		throw std::string("Error setting socket options");
	else
		std::cout << "Socket options set : " << this->_serverSocketFd << std::endl;

	// We set the socket to non-blocking
	if (fcntl(this->_serverSocketFd, F_SETFL, O_NONBLOCK) != 0)
		throw std::string("Error setting socket to non-blocking");
	else
		std::cout << "Socket set to non-blocking : " << this->_serverSocketFd << std::endl;

	// We bind the socket to the port
	populate_sockaddr_in();
	if (bind(this->_serverSocketFd, (struct sockaddr *)&this->serv_addr, sizeof(this->serv_addr)) != 0)
		throw std::string("Error binding socket");
	else
		std::cout << "Socket binded to port : " << this->_port << std::endl;

	// We listen to the socket
	if (listen(this->_serverSocketFd, MAX_CLIENTS) != 0)
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
	std::cout << "Server Started!" << std::endl;
	while (this->_signal)
	{
		if (poll(&_pollFds[0], _pollFds.size(), -1) != 0 && this->_signal)
		{
			for (size_t i = 0; i < _pollFds.size(); i++) // Check for all fd's do see new info
			{
				if (_pollFds[i].revents & POLLIN) // If there is new info
				{
					if (_pollFds[i].fd == this->_serverSocketFd) // If the new info is from the server socket (therefor is a new client)
					{
						try {
							acceptNewClient();
						} catch (std::string &e) {
							std::cerr << e << std::endl;
						}
					}
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

	// We set the new client to non-blocking
	if (fcntl(newClientFd, F_SETFL, O_NONBLOCK) != 0)
		throw std::string("Error setting new client to non-blocking");

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
	std::vector<Client>::iterator i = findClient(fd);
	char buffer[1024] = {0};
	int bytes = recv(fd, buffer, 1024, 0);
	std::string msg = buffer;
	if (bytes == 0) {
		std::cout << "Client " << fd << " disconnected" << std::endl;
		clearClient(fd);
		close(fd);
	}
	else if (bytes == -1)
		std::cerr << "Error receiving data from client: " << fd << std::endl;
	else if (msg.find("\r\n") == std::string::npos) {
		if (msg.find("\n") != std::string::npos)
			msg = msg.substr(0, msg.find("\n"));
		std::cout << "Client " << fd << " sent: \"" << msg  << "\" sent to buffer" << std::endl;
		i->appendBuffer(msg);
	}
	else {
		std::string all = i->getBuffer() + msg;
		all = all.substr(0, all.find("\r\n"));
		i->clearBuffer();
		Handlemsg(all, i);
	}
	msg.clear();
}

void Server::Handlemsg(std::string msg, std::vector<Client>::iterator i)
{
	Commands cmds;

	if (msg.empty() || cmds.isCommand(msg, *i, this->_clients))
		return;

	std::string response = "<" + i->getNickname() + "> " + msg + "\r\n";
	for (std::vector<Client>::iterator i2 = _clients.begin(); i2 != _clients.end(); i2++)

	{
			if (send(i2->getFd(), response.c_str(), response.size(), 0) == -1)
				std::cerr << "Error sending message to client: " << i2->getFd() << std::endl;
	}

	std::cout << "[" << i->getFd() << "]" << "<"  << i->getNickname() << ">" << msg << std::endl;
}

// Close the files descriptors. Clients and server socket.
void Server::closePollFds() {

	for (std::vector<Client>::iterator i = this->_clients.begin(); i != this->_clients.end(); i++)
	{
		int c = close(i->getFd());
		std::cout << "Client " << i->getFd() << " closed with status: " << c << std::endl;
	}
	if (this->_serverSocketFd != -1)
	{
		int c = close(this->_serverSocketFd);
		std::cout << "Server Socket: " << this->_serverSocketFd << " closed with status: " << c << std::endl;
	}
}

// Remove the client with fd from _clients and _pollFds

void Server::clearClient(int fd)
{
	for (std::vector<Client>::iterator i = this->_clients.begin(); i != this->_clients.end(); i++)
	{
		if (i->getFd() == fd)
		{
			int c = close(fd);
			std::cout << "Client " << fd << " closed with status: " << c << std::endl;
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
}