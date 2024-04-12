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


void JOIN(Client &c, std::vector<std::string> args, Server &s);
void PRIVMSG(Client &c, std::vector<std::string> args, Server &s);
void KICK(Client &c, std::vector<std::string> args, Server &s);
void INVITE(Client &c, std::vector<std::string> args, Server &s);
void TOPIC(Client &c, std::vector<std::string> args, Server &s);
void MODE(Client &c, std::vector<std::string> args, Server &s);


#endif