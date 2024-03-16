#include "Server.hpp"

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

void Server::clearClients(int fd) {
    (void)fd;
}