#include "../lib/Commands.hpp"

bool Commands::isCommand(std::string &msg, Client &c, std::vector<Client> &clients)
{
	std::string command = msg.substr(0, msg.find(" "));

	if (command == "PASS")
		PASS(c, split_string(msg, ' '));
	else if (command == "NICK")
		NICK(c, split_string(msg, ' '), clients);
	else if (command == "USER")
		USER(c, split_string(msg, ' '));
	else if (command == "JOIN")
		JOIN(c, split_string(msg, ' '));
	else if (command == "PRIVMSG")
		PRIVMSG(c, split_string(msg, ' '));
	else if (command == "KICK")
		KICK(c, split_string(msg, ' '));
	else if (command == "INVITE")
		INVITE(c, split_string(msg, ' '));
	else if (command == "TOPIC")
		TOPIC(c, split_string(msg, ' '));
	else if (command == "MODE")
		MODE(c, split_string(msg, ' '));
	else
		return false;
	return true;
}


void Commands::PASS(Client &c, std::vector<std::string> args)
{
	if (c.HasRegistred())
		CommandInfo(c, args, ERR_ALREADYREGISTRED, ALREADY_REGISTRED);
	else if (args.size() == 1)
		CommandInfo(c, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
	else
	{
		c.setPassword(args[1]);
		std::cout << CMD_SET(c.getFd(), args[0], args[1]) << std::endl;
	}

}

void Commands::NICK(Client &c, std::vector<std::string> args, std::vector<Client> &clients)
{
	if (c.HasRegistred()) // if user has registered
		CommandInfo(c, args, ERR_ALREADYREGISTRED, ALREADY_REGISTRED);
	else if (args.size() == 1) // if no nick provided
		CommandInfo(c, args, ERR_NONICKNAMEGIVEN, NO_NICKNAME_GIVEN);
	else
	{
	std::string new_nick = args[1];
	if (new_nick.size() > MAX_NICK_NAME)
		new_nick = new_nick.substr(0, MAX_NICK_NAME);

	// if nick is erroneus
	for (size_t i = 0; i < new_nick.size(); i++){
		if (!isalnum(new_nick[i]) && new_nick[i] != '_' && new_nick[i] != '-') {
			CommandInfo(c, args, ERR_ERRONEUSNICKNAME, ERRONEUS_NICKNAME);
			return ;
		}

	}

	// if nick is in use,
	for (size_t i = 0; i < clients.size(); i++){
		if (clients[i].getNickname() == new_nick){
			CommandInfo(c, args, ERR_NICKNAMEINUSE, NICKNAME_IN_USE);
			return ;
		}
	}
	
	c.setNickname(new_nick);
	std::cout << CMD_SET(c.getFd(), args[0], new_nick) << std::endl;

	}
}

void Commands::USER(Client &c, std::vector<std::string> args)
{
	std::cout << "USER COMMAND" << std::endl;
	(void)c;
	(void)args;
}

void Commands::JOIN(Client &c, std::vector<std::string> args)
{
	std::cout << "JOIN COMMAND" << std::endl;
	(void)c;
	(void)args;
}

void Commands::PRIVMSG(Client &c, std::vector<std::string> args)
{
	std::cout << "PRIVMSG COMMAND" << std::endl;
	(void)c;
	(void)args;
}

void Commands::KICK(Client &c, std::vector<std::string> args)
{
	std::cout << "KICK COMMAND" << std::endl;
	(void)c;
	(void)args;
}	

void Commands::INVITE(Client &c, std::vector<std::string> args)
{
	std::cout << "INVITE COMMAND" << std::endl;
	(void)c;
	(void)args;
}

void Commands::TOPIC(Client &c, std::vector<std::string> args)
{
	std::cout << "TOPIC COMMAND" << std::endl;
	(void)c;
	(void)args;
}

void Commands::MODE(Client &c, std::vector<std::string> args)
{
	std::cout << "MODE COMMAND" << std::endl;
	(void)c;
	(void)args;
}