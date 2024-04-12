#ifndef COMMANDS_HPP
#define COMMANDS_HPP

# include "ircserver.hpp"
# include "Channel.hpp"
# include "Server.hpp"

class Client;
class Server;

bool isCommand(std::string &msg, Client &c, Server &s);

void PASS(Client &c, std::vector<std::string> args);
bool NICK(Client &c, std::vector<std::string> args, std::vector<Client> &clients);
void USER(Client &c, std::vector<std::string> args, Server &s);


void JOIN(Client &client, std::vector<std::string> args, Server &serv);
void PRIVMSG(Client &client, std::vector<std::string> args, Server &serv);
void KICK(Client &client, std::vector<std::string> args, Server &serv);
void INVITE(Client &client, std::vector<std::string> args, Server &serv);
void TOPIC(Client &client, std::vector<std::string> args, Server &serv);
void MODE(Client &client, std::vector<std::string> args, Server &serv);
void modePlusT(Client &client, std::vector<std::string> args, Server &serv);
void modeMinusT(Client &client, std::vector<std::string> args, Server &serv);


#endif