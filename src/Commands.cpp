#include "../lib/Commands.hpp"

bool isCommand(std::string &msg, Client &c, Server &s)
{
	std::string command = msg.substr(0, msg.find(" "));

	if (command == "PASS")
		PASS(c, split_string(msg, ' '));
	else if (command == "NICK")
		NICK(c, split_string(msg, ' '), s.getClients());
	else if (command == "USER")
		USER(c, split_string(msg, ' '), s);
	else if (command == "JOIN")
		JOIN(c, split_string(msg, ' '), s);
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
	else if (command == "CAP")
		IRCsend(c.getFd(), std::string("CAP * LS :server"))
	else
		return false;
	return true;
}

void print_cmd(std::string cmd, std::vector<std::string> args)
{
	std::cout << "Command: " << cmd << " ";
	for (size_t i = 1; i < args.size(); i++)
		std::cout << "'" << args[i] << "' ";
	std::cout << std::endl;
}

void PASS(Client &c, std::vector<std::string> args)
{
	print_cmd(args[0], args);
	if (c.HasRegistred())
		CommandInfo(c, args, ERR_ALREADYREGISTRED, ALREADY_REGISTRED);
	else if (args.size() == 1)
		CommandInfo(c, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
	else
	{
		c.setPassword(args[1]);
		std::cout << CMD_SET(c.getFd(), args[0], args[1]) << std::endl;
		if (c.HasNick() && c.HasUser() && c.HasPass())
			c.RegisterClient(c.getFd());
	}

}

bool NICK(Client &c, std::vector<std::string> args, std::vector<Client> &clients)
{
	print_cmd(args[0], args);

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
			c.RegisterClient(c.getFd());
		return true;
	}
	return false;
}

void USER(Client &c, std::vector<std::string> args, Server &s)
{
	print_cmd(args[0], args);

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

		std::string New_realname;
		for (size_t i = 4; i < args.size(); i++)
			New_realname += args[i] + " ";
		if (New_realname[0] == ':' && New_realname.size() > 1)
			New_realname = New_realname.substr(1, New_realname.size() - 1);
		c.setRealname(New_realname);
		std::cout << CMD_SET(c.getFd(), "USER", args[1]) << std::endl;
		std::cout << CMD_SET(c.getFd(), "HOST", args[2]) << std::endl;
		std::cout << CMD_SET(c.getFd(), "MODE", args[3]) << std::endl;
		std::cout << CMD_SET(c.getFd(), "REAL_NAME", New_realname) << std::endl;
		
		if (!c.HasPass() || c.getPassword() != s.getPassword())
		{
			CommandInfo(c, args, ERR_BADPASSWORD, BAD_PASSWORD);
			std::string end_con = "ERROR :Closing Link: localhost (Bad Password)\r\n";
			IRCsend(c.getFd(), end_con);
			s.clearClient(c.getFd());
			return ;
		}
		c.RegisterClient(c.getFd());
	}
}

void JOIN(Client &c, std::vector<std::string> args, Server &s)
{
	print_cmd(args[0], args);

	if (c.HasRegistred() == false)
		CommandInfo(c, args, ERR_BANNEDFROMCHAN, std::string(BANNED_FROM_CHAN)  + std::string(" (You must be registered to join a channel)"));
	else if (args.size() == 1)
		CommandInfo(c, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
	else
	{
		std::string channel = args[1];
		if (channel[0] != '#')
			channel = "#" + channel;

		if (s.findChannel(channel) == false)
			Channel::newChannel(channel, c, s);
		else
			Channel::joinChannel(channel, c, s);		
	}


}

void PRIVMSG(Client &c, std::vector<std::string> args)
{
	print_cmd(args[0], args);
	(void)c;
	(void)args;
}

void KICK(Client &c, std::vector<std::string> args)
{
	print_cmd(args[0], args);
	(void)c;
	(void)args;
}	

void INVITE(Client &c, std::vector<std::string> args)
{
	print_cmd(args[0], args);
	(void)c;
	(void)args;
}

void TOPIC(Client &c, std::vector<std::string> args)
{
	print_cmd(args[0], args);
	(void)c;
	(void)args;
}

void MODE(Client &c, std::vector<std::string> args)
{
	print_cmd(args[0], args);
	(void)c;
	(void)args;
}