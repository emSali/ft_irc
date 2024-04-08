#include "../lib/Commands.hpp"

bool Commands::isCommand(std::string &msg, Client &c)
{
	std::string command = msg.substr(0, msg.find(" "));

	if (command == "PASS")
		PASS(c, command, split_string(msg, ' '));
	else if (command == "NICK")
		NICK(c, command, split_string(msg, ' '));
	else if (command == "USER")
		USER(c, command, split_string(msg, ' '));
	else if (command == "JOIN")
		JOIN(c, command, split_string(msg, ' '));
	else if (command == "PRIVMSG")
		PRIVMSG(c, command, split_string(msg, ' '));
	else if (command == "KICK")
		KICK(c, command, split_string(msg, ' '));
	else if (command == "INVITE")
		INVITE(c, command, split_string(msg, ' '));
	else if (command == "TOPIC")
		TOPIC(c, command, split_string(msg, ' '));
	else if (command == "MODE")
		MODE(c, command, split_string(msg, ' '));
	else
		return false;
	return true;
}


void Commands::PASS(Client &c, std::string &cmd, std::vector<std::string> args)
{
	if (c.getRegistred())
	{
		std::cout << CMD_ERR(c.getFd(), cmd, ALREADY_REGISTRED);

		std::string msg = GEN_ERR(ERR_ALREADYREGISTRED, ALREADY_REGISTRED, c.getNickname());
		if (send(c.getFd(), msg.c_str(), msg.size(), 0) == -1)
			std::cerr << "Error: send" << std::endl;
	}
	else if (args.size() == 1)
	{
		std::cout << CMD_ERR(c.getFd(), cmd, NEED_MORE_PARAMS) << std::endl;
		std::string msg = GEN_ERR(ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS, c.getNickname());
		if (send(c.getFd(), msg.c_str(), msg.size(), 0) == -1)
			std::cerr << "Error: send" << std::endl;
	}
	else
	{
		c.setPassword(args[1]);
		std::cout << CMD_SET(c.getFd(), cmd, args[1]) << std::endl;
	}

}

void Commands::NICK(Client &c, std::string &cmd, std::vector<std::string> args)
{
	if (c.getRegistred())
	{
		std::cout << CMD_ERR(c.getFd(), cmd, ALREADY_REGISTRED);

		std::string msg = GEN_ERR(ERR_ALREADYREGISTRED, ALREADY_REGISTRED, c.getNickname());
		if (send(c.getFd(), msg.c_str(), msg.size(), 0) == -1)
			std::cerr << "Error: send" << std::endl;
	}
	else if (args.size() == 1)
	{
		std::cout << CMD_ERR(c.getFd(), cmd, NEED_MORE_PARAMS) << std::endl;
		std::string msg = GEN_ERR(ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS, c.getNickname());
		if (send(c.getFd(), msg.c_str(), msg.size(), 0) == -1)
			std::cerr << "Error: send" << std::endl;
	}
	else
	{
		std::string new_nick = args[1];
		if (new_nick.size() > MAX_NICK_NAME)
			new_nick = new_nick.substr(0, MAX_NICK_NAME);

		// Missing check if nick is in use,
		// Missing check if nick is erroneus
		// Missing check if nick is colliding
		// Missing check if nick is already set
		
		c.setNickname(new_nick);
		std::cout << CMD_SET(c.getFd(), cmd, new_nick) << std::endl;
	}

}

void Commands::USER(Client &c, std::string &cmd, std::vector<std::string> args)
{
	std::cout << "USER COMMAND" << std::endl;
	(void)c;
	(void)cmd;
	(void)args;
}

void Commands::JOIN(Client &c, std::string &cmd, std::vector<std::string> args)
{
	std::cout << "JOIN COMMAND" << std::endl;
	(void)c;
	(void)cmd;
	(void)args;
}

void Commands::PRIVMSG(Client &c, std::string &cmd, std::vector<std::string> args)
{
	std::cout << "PRIVMSG COMMAND" << std::endl;
	(void)c;
	(void)cmd;
	(void)args;
}

void Commands::KICK(Client &c, std::string &cmd, std::vector<std::string> args)
{
	std::cout << "KICK COMMAND" << std::endl;
	(void)c;
	(void)cmd;
	(void)args;
}	

void Commands::INVITE(Client &c, std::string &cmd, std::vector<std::string> args)
{
	std::cout << "INVITE COMMAND" << std::endl;
	(void)c;
	(void)cmd;
	(void)args;
}

void Commands::TOPIC(Client &c, std::string &cmd, std::vector<std::string> args)
{
	std::cout << "TOPIC COMMAND" << std::endl;
	(void)c;
	(void)cmd;
	(void)args;
}

void Commands::MODE(Client &c, std::string &cmd, std::vector<std::string> args)
{
	std::cout << "MODE COMMAND" << std::endl;
	(void)c;
	(void)cmd;
	(void)args;
}