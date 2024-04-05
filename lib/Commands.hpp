#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Client.hpp"
#include "Channel.hpp"

class Client;
class Channel;

void kick(Client client, Channel channel);
void invite(Client client, Channel channel);
std::string topic(Channel channel);
void topic(Client client, Channel channel, std::string topic);
void modeI(Client client, Channel channel);
void modeT(Client client, Channel channel);
void modeK(Client client, Channel channel);
void modeO(Client client, Channel channel, std::string nickname);
void modeL(Client client, Channel channel);

#endif