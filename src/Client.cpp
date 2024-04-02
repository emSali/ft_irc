#include "../lib/Client.hpp"

Client::Client() {}

Client::~Client() {}

int Client::get_fd() {
    return _fd;
}

void Client::set_fd(int fd) {
    _fd = fd;
}

std::string Client::get_ipAddress() {
    return _ipAddress;
}

void Client::set_ipAddress(std::string ipAddress) {
    _ipAddress = ipAddress;
}