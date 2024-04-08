#include "../lib/Commands.hpp"

bool Commands::isCommand(std::string &msg, Client &c, Server &s)
{
	std::string command = msg.substr(0, msg.find(" "));

	if (command == "PASS")
		PASS(c, split_string(msg, ' '));
	else if (command == "NICK")
		NICK(c, split_string(msg, ' '), s.getClients());
	else if (command == "USER")
		USER(c, split_string(msg, ' '), s);
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
		if (c.HasNick() && c.HasUser() && c.HasPass())
			c.setRegistred(true);
	}

}

bool Commands::NICK(Client &c, std::vector<std::string> args, std::vector<Client> &clients)
{
	if (c.HasRegistred()) // if user has registered
		CommandInfo(c, args, ERR_ALREADYREGISTRED, ALREADY_REGISTRED);
	else if (args.size() == 1) // if no nick provided
		CommandInfo(c, args, ERR_NONICKNAMEGIVEN, NO_NICKNAME_GIVEN);
	else
	{
		if (args[1].size() > MAX_NICKNAME)
			args[1] = args[1].substr(0, MAX_NICKNAME);

		// if nick is erroneus
		for (size_t i = 0; i < args[1].size(); i++){
			if (!isalnum(args[1][i]) && args[1][i] != '_' && args[1][i] != '-') {
				CommandInfo(c, args, ERR_ERRONEUSNICKNAME, ERRONEUS_NICKNAME);
				return false;
			}
		}

		// if nick is in use,
		for (size_t i = 0; i < clients.size(); i++) {
			if (clients[i].getNickname() == args[1] && clients[i].getFd() != c.getFd()) {
				CommandInfo(c, args, ERR_NICKNAMEINUSE, NICKNAME_IN_USE);
				return false;
			}
		}

		c.setNickname(args[1]);
		std::cout << CMD_SET(c.getFd(), args[0], args[1]) << std::endl;
		if (c.HasNick() && c.HasUser() && c.HasPass())
			c.setRegistred(true);
		return true;
	}
	return false;
}

void Commands::USER(Client &c, std::vector<std::string> args, Server &s)
{
	if(c.HasRegistred())
		CommandInfo(c, args, ERR_REREGISTER, REREGISTER);
	else if (args.size() < 5)
		CommandInfo(c, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
	else
	{
		if (args[1].size() > MAX_USERNAME)
			args[1] = args[1].substr(0, MAX_USERNAME);

		std::vector<std::string> arg_to_nick;
		arg_to_nick.push_back("NICK");
		arg_to_nick.push_back(c.getNickname());

		if (NICK(c, arg_to_nick, s.getClients()) == false)
			return ;

		c.setUsername(args[1]);
		c.setHostname(args[2]);
		c.setMode(args[3]);
		c.setRealname(args[4]);
		std::cout << CMD_SET(c.getFd(), args[0], args[1]) << std::endl;
		std::cout << CMD_SET(c.getFd(), args[0], args[2]) << std::endl;
		std::cout << CMD_SET(c.getFd(), args[0], args[3]) << std::endl;
		std::cout << CMD_SET(c.getFd(), args[0], args[4]) << std::endl;
		
		if (!c.HasPass() || c.getPassword() != s.getPassword())
		{
			CommandInfo(c, args, ERR_BADPASSWORD, BAD_PASSWORD);
			std::string end_con = "ERROR :Closing Link: localhost (Bad Password)\r\n";
			if (send(c.getFd(), end_con.c_str(), end_con.size(), 0) == -1)
				std::cerr << "Error: send" << std::endl;
			s.clearClient(c.getFd());
			return ;
		}
		CommandInfo(c, args, RPL_WELCOME, WELCOME);
		
	}
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