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
		PRIVMSG(c, split_string(msg, ' '), s);
	else if (command == "KICK")
		KICK(c, split_string(msg, ' '), s);
	else if (command == "INVITE")
		INVITE(c, split_string(msg, ' '), s);
	else if (command == "TOPIC")
		TOPIC(c, split_string(msg, ' '), s);
	else if (command == "MODE")
		MODE(c, split_string(msg, ' '), s);
	else if (command == "CAP")
		IRCsend(c.getFd(), std::string("CAP * LS :server\r\n"))
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
		std::string msg = "Your password has been set to "; msg.append(c.getPassword());
		IRCsend(c.getFd(), GEN_MSG("NOTICE", msg, to_string(c.getFd())))

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
			if (!isalnum(args[1][i]) && args[1][i] != '_' && args[1][i] != '-' && args[1][i] != ':') {
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
		std::string msg = "Your nickname has been set to "; msg.append(c.getNickname());
		IRCsend(c.getFd(), GEN_MSG("NOTICE", msg, to_string(c.getFd())))

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
		// c.setHostname(args[2]); // Already defined in accepting
		c.setMode(args[3]); // Kinda useless

		std::string New_realname;
		for (size_t i = 4; i < args.size(); i++)
			New_realname += args[i] + " ";
		if (New_realname[0] == ':' && New_realname.size() > 1)
			New_realname = New_realname.substr(1, New_realname.size() - 1);
		c.setRealname(New_realname);
		std::cout << CMD_SET(c.getFd(), "USER", args[1]) << std::endl;
		std::cout << CMD_SET(c.getFd(), "HOST", c.getHostname()) << std::endl;
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
			Channel::joinChannel(channel, c, s, false);		
	}


}

// sending a msg in a channel --> PRIVMSG #bitcoin :hey
// @time=2024-04-12T13:55:42.526Z :loris!~lorislori@2001:8a0:7ac8:2300:20bc:455c:deab:d375 PRIVMSG #okokok :ola
// sending a msg to a user --> PRIVMSG user :hey
// @time=2024-04-12T14:07:23.350Z :loris!~lorislori@2001:8a0:7ac8:2300:20bc:455c:deab:d375 PRIVMSG lolo :hey dude
void PRIVMSG(Client &client, std::vector<std::string> args, Server &serv)
{
	print_cmd(args[0], args);

	(void)client;
	(void)args;
	(void)serv;
}

// KICK #channel nickname
void KICK(Client &client, std::vector<std::string> args, Server &serv)
{
	print_cmd(args[0], args);
	std::string channelName = args[1];
	Channel channel = serv.getChannel(channelName);

	// get the nickname of the client to kick
	std::string nickname = args[2];
	Client clientToKick = serv.getClient(nickname);

	if (!channel.isOperator(client)) {
		// @time=2024-04-12T13:31:42.528Z :calcium.libera.chat 482 loris #ubuntu :You're not a channel operator
		// print: #ubuntu :You're not a channel operator
		IRCsend(client.getFd(), GEN_MSG(ERR_CHANOPRIVSNEEDED, channelName + " :You're not a channel operator", client.getNickname()));
		return;
	}
	if (!channel.isClient(clientToKick)) {
		// @time=2024-04-12T13:42:22.833Z :calcium.libera.chat 401 loris nickname :No such nick/channel
		// print nothing
		return;
	}
	// @time=2024-04-12T13:43:19.153Z :loris!~lorislori@2001:8a0:7ac8:2300:20bc:455c:deab:d375 KICK #jiofdsnog loris :loris
	channel.removeClient(clientToKick);
	// print : You have been kicked from #jiofdsnog by loris (loris)
	IRCsend(clientToKick.getFd(), GEN_MSG("KICK", channelName + " :" + "You have been kicked from " + channelName + " by " + client.getNickname(), client.getNickname()));
	// print to the channel: client has kicked clientToKick from #okokok (clientToKick)
	GEN_MSG("KICK", client.getNickname() + " has kicked " + clientToKick.getNickname() + " from " + channelName, client.getNickname());
}

void INVITE(Client &client, std::vector<std::string> args, Server &serv)
{
	print_cmd(args[0], args);
	(void)client;
	(void)args;
	(void)serv;
}

void TOPIC(Client &client, std::vector<std::string> args, Server &serv)
{
	print_cmd(args[0], args);
	std::string channelName = args[1];
	Channel channel = serv.getChannel(channelName);

	// print topic
	if (args.size() == 2) {
		IRCsend(client.getFd(), GEN_MSG(RPL_TOPIC, channelName + " :" + channel.getTopic(), client.getNickname()));
		return;
	}

	// set topic
	if (channel.isRestrictedTopicActive() && !channel.isOperator(client)) {
		IRCsend(client.getFd(), GEN_MSG(ERR_CHANOPRIVSNEEDED, channelName + " :You're not a channel operator", client.getNickname()));
		return;
	}
	// newTopic is equal to all args after the channel name
	std::string newTopic = "";
	for (size_t i = 2; i < args.size(); i++) {
		newTopic += args[i] + " ";
	}
	// remove the first character ':'
	newTopic = newTopic.substr(1, newTopic.size() - 1);
	// remove the last space
	newTopic = newTopic.substr(0, newTopic.size() - 1);

	channel.setTopic(newTopic);
	// print: loris has changed the topic to: new topic is this..
	GEN_MSG("TOPIC", client.getNickname() + " has changed the topic to: " + newTopic, client.getNickname());
}

// Log --> print in channel

// MODE #okokok +i --> loris sets mode +i on #okokok
// MODE #okokok -i --> loris sets mode -i on #okokok

// MODE #okokok +t --> loris sets mode +t on #okokok
// MODE #okokok -t --> loris sets mode -t on #okokok

// MODE #okokok +k ok hey --> loris sets channel keyword to ok
// MODE #okokok -k --> loris removes channel keyword

// MODE #okokok +o lolo --> loris gives channel operator status to lolo
// MODE #okokok -o lolo --> loris removes channel operator status from lolo

// MODE #okokok +l // do nothing
// MODE #okokok +l 100 --> loris sets channel limit to 100
// MODE #okokok -l --> loris removes user limit
void MODE(Client &client, std::vector<std::string> args, Server &serv)
{
	print_cmd(args[0], args);

	std::string channelName = args[1];
	Channel channel = serv.getChannel(channelName);

	if (!channel.isOperator(client)) {
		IRCsend(client.getFd(), GEN_MSG(ERR_CHANOPRIVSNEEDED, channel.getName() + " :You're not a channel operator", client.getNickname()));
		return;
	}

	if (args.size() >= 3 && args[2] == "+i") {
		modePI(client, channel);
	} else if (args.size() >= 3 && args[2] == "-i") {
		modeMI(client, channel);
	} else if (args.size() >= 3 && args[2] == "+t") {
		modePT(client, channel);
	} else if (args.size() >= 3 && args[2] == "-t") {
		modeMT(client, channel);
	} else if (args.size() >= 3 && args[2] == "+k") {
		modePK(client, channel, args);
	} else if (args.size() >= 3 && args[2] == "-k") {
		modeMK(client, channel);
	} else if (args.size() >= 3 && args[2] == "+o") {
		modePO(client, serv, channel, args);
	} else if (args.size() >= 3 && args[2] == "-o") {
		modeMO(client, serv, channel, args);
	} else if (args.size() >= 3 && args[2] == "+l") {
		modePL(client, channel, args);
	} else if (args.size() >= 3 && args[2] == "-l") {
		modeML(client, channel);
	}
}

void modePI(Client &client, Channel &channel) {
	if (!channel.isInviteOnlyActive()) {
		channel.activateInviteOnly();		
		GEN_MSG("MODE", client.getNickname() + " sets mode +i on " + channel.getName(), client.getNickname());
		return;
	}
}
void modeMI(Client &client, Channel &channel) {
	if (channel.isInviteOnlyActive()) {
		channel.deactivateInviteOnly();
		GEN_MSG("MODE", client.getNickname() + " sets mode -i on " + channel.getName(), client.getNickname());
		return;
	}
}

void modePT(Client &client, Channel &channel) {
	if (!channel.isRestrictedTopicActive()) {
		channel.activateRestrictedTopic();
		GEN_MSG("MODE", client.getNickname() + " sets mode +t on " + channel.getName(), client.getNickname());
		return;
	}
}
void modeMT(Client &client, Channel &channel) {
	if (channel.isRestrictedTopicActive()) {
		channel.deactivateRestrictedTopic();
		GEN_MSG("MODE", client.getNickname() + " sets mode -t on " + channel.getName(), client.getNickname());
		return;
	}
}

void modePK(Client &client, Channel &channel, std::vector<std::string> args) {
	if (args.size() < 4) {
		IRCsend(client.getFd(), GEN_MSG(ERR_NEEDMOREPARAMS, channel.getName() + " :Not enough parameters", client.getNickname()));
		return;
	}
	std::string key = args[3];
	channel.setKey(key);
	channel.activateKey();
	GEN_MSG("MODE", client.getNickname() + " sets channel keyword to " + key, client.getNickname());
}
void modeMK(Client &client, Channel &channel) {
	if (channel.isKeyActive()) {
		channel.deactivateKey();
		GEN_MSG("MODE", client.getNickname() + " removes channel keyword", client.getNickname());
		return;
	}
}

void modePO(Client &client, Server &serv, Channel &channel, std::vector<std::string> args) {
	if (args.size() < 4) {
		IRCsend(client.getFd(), GEN_MSG(ERR_NEEDMOREPARAMS, channel.getName() + " :Not enough parameters", client.getNickname()));
		return;
	}
	std::string nickname = args[3];
	Client clientToOp = serv.getClient(nickname);
	// if the client does not exist
	if (channel.isClient(clientToOp) == false){
		IRCsend(client.getFd(), GEN_MSG(ERR_NOSUCHNICK, nickname + " :No such nick/channel", client.getNickname()));
		return;
	}
	// if the client is not an operator, give operator status
	if (!channel.isOperator(clientToOp)) {
		channel.addOperator(clientToOp);
		GEN_MSG("MODE", client.getNickname() + " gives channel operator status to " + nickname, client.getNickname());
	}
}
void modeMO(Client &client, Server &serv, Channel &channel, std::vector<std::string> args) {
	if (args.size() < 4) {
		IRCsend(client.getFd(), GEN_MSG(ERR_NEEDMOREPARAMS, channel.getName() + " :Not enough parameters", client.getNickname()));
		return;
	}
	std::string nickname = args[3];
	Client clientToOp = serv.getClient(nickname);
	// if the client does not exist
	if (channel.isClient(clientToOp) == false){
		IRCsend(client.getFd(), GEN_MSG(ERR_NOSUCHNICK, nickname + " :No such nick/channel", client.getNickname()));
		return;
	}
	// if the client is an operator, remove operator status
	if (channel.isOperator(clientToOp)) {
		channel.removeOperator(clientToOp);
		GEN_MSG("MODE", client.getNickname() + " removes channel operator status from " + nickname, client.getNickname());
	}
}

void modePL(Client &client, Channel &channel, std::vector<std::string> args) {
	if (args.size() < 4) {
		IRCsend(client.getFd(), GEN_MSG(ERR_NEEDMOREPARAMS, channel.getName() + " :Not enough parameters", client.getNickname()));
		return;
	}
	if (!channel.isUserLimitActive()) {
		std::string limit = args[3];
		// convert limit to double then int
		char* endPtr;
		double userLimit = strtod(limit.c_str(), &endPtr);
		// compare original string to endPtr to see if something was parsed
		if (endPtr == limit.c_str()) {
			return;
		}
		channel.setUserLimit((int)userLimit);
		channel.activateUserLimit();
		GEN_MSG("MODE", client.getNickname() + " sets channel limit to " + limit, client.getNickname());
	}
}
void modeML(Client &client, Channel &channel) {
	if (channel.isUserLimitActive()) {
		channel.deactivateUserLimit();
		GEN_MSG("MODE", client.getNickname() + " removes user limit", client.getNickname());
	}
}


/*
void invite(Client client, Channel channel) {
    if (channel.isClient(client)) {
        // Already in the channel
        return;
    } else if (channel.isUserLimitActive() && channel.getClients().size() >= channel.getUserLimit()) {
        // Channel is full
        return;
    } else if (channel.isOperator(client)) {
        channel.addClient(client);
        std::cout << client.getNickname() << " has joined " << channel.getName() << std::endl;
    }
}

// only operators can change the topic when it is not restricted. Otherwise, nobody can change the topic. Everyone can see the topic.
std::string topic(Channel channel) {
    return channel.getTopic();
}
void topic(Client client, Channel channel, std::string topic) {
    if (channel.isRestrictedTopicActive()) {
        return;
    } else if (channel.isOperator(client)) {
        channel.setTopic(topic);
        std::cout << client.getNickname() << " has changed the topic to: " << topic << std::endl;
    }
}

void modeI(Client client, Channel channel) {
    if (channel.isOperator(client) && !channel.isInviteOnlyActive()) {
        channel.activateInviteOnly();
        std::cout << channel.getName() << "Invite-only mode activated" << std::endl;
    } else if (channel.isOperator(client) && channel.isInviteOnlyActive()) {
        channel.deactivateInviteOnly();
        std::cout << channel.getName() << "Invite-only mode deactivated" << std::endl;
    }
}

void modeT(Client client, Channel channel) {
    if (channel.isOperator(client) && !channel.isRestrictedTopicActive()) {
        channel.activateRestrictedTopic();
        std::cout << channel.getName() << "Topic restricted mode activated" << std::endl;
    } else if (channel.isOperator(client) && channel.isRestrictedTopicActive()) {
        channel.deactivateRestrictedTopic();
        std::cout << channel.getName() << "Topic restricted mode deactivated" << std::endl;
    }
}

void modeK(Client client, Channel channel) {
    if (channel.isOperator(client) && channel.isKeyActive()) {
        channel.deactivateKey();
        std::cout << channel.getName() << "Key deactivated" << std::endl;
    } else if (channel.isOperator(client) && !channel.isKeyActive()) {
        channel.activateKey();
        std::cout << channel.getName() << "Key activated" << std::endl;
    }
}

void modeO(Client client, Channel channel, std::string nickname) {
    if (channel.isOperator(client)) {
        // if nickname is already an operator, remove operator status
        for (std::map<int, Client *> it = channel.getClients().begin(); it != channel.getClients().end(); it++) {
            if (it->second->getNickname() == nickname) {
                channel.removeOperator(*it->second);
                std::cout << nickname << " is no longer an operator in " << channel.getName() << std::endl;
                return;
            }
        }

        // if nickname is not an operator, add operator status
        for (std::map<int, Client *> it = channel.getClients().begin(); it != channel.getClients().end(); it++) {
            if (it->second->getNickname() == nickname) {
                channel.addOperator(*it->second);
                std::cout << nickname << " is now an operator in " << channel.getName() << std::endl;
                return;
            }
        }
    }
}

void modeL(Client client, Channel channel) {
    if (channel.isOperator(client) && !channel.isUserLimitActive()) {
        channel.activateUserLimit();
        std::cout << channel.getName() << "User limit activated" << std::endl;
    } else if (channel.isOperator(client) && channel.isUserLimitActive()) {
        channel.deactivateUserLimit();
        std::cout << channel.getName() << "User limit deactivated" << std::endl;
    } 
}

*/