#ifndef COMMANDS_HPP
#define COMMANDS_HPP

# include "ircserver.hpp"
# include "Channel.hpp"
# include "Server.hpp"

class Client;
class Server;

bool isCommand(std::string &msg, Client &c, Server &s);

void PASS(Client &c, std::vector<std::string> args, Server &s);
bool NICK(Client &c, std::vector<std::string> args, Server &s, int justChecking);
void USER(Client &c, std::vector<std::string> args, Server &s);


void JOIN(Client &client, std::vector<std::string> args, Server &serv);
void WHO(Client &client, std::vector<std::string> args, Server &serv);
void PART(Client &client, std::vector<std::string> args, Server &serv);
void PRIVMSG(Client &client, std::vector<std::string> args, Server &serv);
void KICK(Client &client, std::vector<std::string> args, Server &serv);
void INVITE(Client &client, std::vector<std::string> args, Server &serv);
void TOPIC(Client &client, std::vector<std::string> args, Server &serv);
void MODE(Client &client, std::vector<std::string> args, Server &serv);
void modePI(Client &client, std::vector<Channel>::iterator &channel);
void modeMI(Client &client, std::vector<Channel>::iterator &channel);
void modePT(Client &client, std::vector<Channel>::iterator &channel);
void modeMT(Client &client, std::vector<Channel>::iterator &channel);
void modePK(Client &client, std::vector<Channel>::iterator &channel, std::vector<std::string> args);
void modeMK(Client &client, std::vector<Channel>::iterator &channel);
void modePO(Client &client, Server &serv, std::vector<Channel>::iterator &channel, std::vector<std::string> args);
void modeMO(Client &client, Server &serv, std::vector<Channel>::iterator &channel, std::vector<std::string> args);
void modePL(Client &client, std::vector<Channel>::iterator &channel, std::vector<std::string> args);
void modeML(Client &client, std::vector<Channel>::iterator &channel);


#endif