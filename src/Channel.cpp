#include "../lib/Channel.hpp"

// Before adding client, check if channel is full? _userLimit
void Channel::addClient(Client &client) {
    _clients[client.getFd()] = &client;
}
void Channel::removeClient(Client &client) {
    _clients.erase(_clients.find(client.getFd()));
}
bool Channel::isClient(Client &client) {
    return (_clients.find(client.getFd()) != _clients.end());
}

void Channel::addOperator(Client &client) {
    _operators[client.getFd()] = &client;
}
void Channel::removeOperator(Client &client) {
    _operators.erase(_operators.find(client.getFd()));
}
bool Channel::isOperator(Client &client) {
    return (_operators.find(client.getFd()) != _clients.end());
}