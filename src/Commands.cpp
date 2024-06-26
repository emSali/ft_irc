#include "../lib/Commands.hpp"

bool isCommand(std::string &msg, Client &c, Server &s)
{
	std::string command = msg.substr(0, msg.find(" "));

	if (command == "PASS")
		PASS(c, split_string(msg, ' '), s);
	else if (command == "NICK")
		NICK(c, split_string(msg, ' '), s, 0);
	else if (command == "USER")
		USER(c, split_string(msg, ' '), s);
	else if (command == "JOIN")
		JOIN(c, split_string(msg, ' '), s);
	else if (command == "WHO")
		WHO(c, split_string(msg, ' '), s);
	else if (command == "PART")
		PART(c, split_string(msg, ' '), s);
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

void PASS(Client &c, std::vector<std::string> args, Server &s)
{
	print_cmd(args[0], args);
	if (c.HasRegistred())
		CommandInfo(c, args, ERR_ALREADYREGISTRED, ALREADY_REGISTRED);
	else if (args.size() == 1) {
		CommandInfo(c, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
	}
	else
	{
		c.setPassword(args[1]);
		std::string msg = "Your password has been set to "; msg.append(c.getPassword());
		IRCsend(c.getFd(), GEN_MSG("NOTICE", msg, to_string(c.getFd())))

		std::cout << CMD_SET(c.getFd(), args[0], args[1]) << std::endl;
		if (c.HasNick() && c.HasUser() && c.HasPass())
		{
			c.RegisterClient(c.getFd());
			s.informChannels(c);
		}
	}

}

bool NICK(Client &c, std::vector<std::string> args, Server &s, int justChecking)
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
		std::vector<Client> clients = s.getClients();
		for (size_t i = 0; i < clients.size(); i++) {
			if (clients[i].getNickname() == args[1] && clients[i].getFd() != c.getFd()) {
				CommandInfo(c, args, ERR_NICKNAMEINUSE, NICKNAME_IN_USE);
				return false;
			}
		}

		c.setNickname(args[1]);
		std::cout << CMD_SET(c.getFd(), args[0], args[1]) << std::endl;
		if (!justChecking)
		{
			std::string msg = "Your nickname has been set to "; msg.append(c.getNickname());
			IRCsend(c.getFd(), GEN_MSG("NOTICE", msg, to_string(c.getFd())))
		}

		if (c.HasNick() && c.HasUser() && c.HasPass())
		{
			c.RegisterClient(c.getFd());
			s.informChannels(c);
		}

		return true;
	}
	return false;
}

void USER(Client &c, std::vector<std::string> args, Server &s)
{
	print_cmd(args[0], args);

	if(c.HasRegistred())
		CommandInfo(c, args, ERR_REREGISTER, REREGISTER);
	else if (args.size() < 5) {
		CommandInfo(c, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
	}
	else
	{
		if (args[1].size() > MAX_USERNAME)
			args[1] = args[1].substr(0, MAX_USERNAME);

		std::vector<std::string> arg_to_nick;
		arg_to_nick.push_back("NICK");
		arg_to_nick.push_back(c.getNickname());

		if (NICK(c, arg_to_nick, s, 1) == false)
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
		s.informChannels(c);
	}
}

void JOIN(Client &c, std::vector<std::string> args, Server &s)
{
	print_cmd(args[0], args);
	if (!c.HasRegistred()) {
		CommandInfo(c, args, ERR_BADPASSWORD, std::string("You must be registered to join a channel"));
	} else if (args.size() == 1) {
		CommandInfo(c, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
	} else if (args[1][0] != '#') {
		CommandInfo(c, args, ERR_NOSUCHCHANNEL, args[1] + " :No such channel");
	} else if (args[1].size() == 1) {
		CommandInfo(c, args, ERR_NOSUCHCHANNEL, args[1] + " :No such channel");
	} else {
		// if channel doesn't exist, create it
		if (s.findChannel(args[1]) == false) {
			Channel::newChannel(args[1], c, s);
		}
		else
		{
			std::vector<Channel>::iterator channel = s.getChannelIterator(args[1]);
			if (channel->isKeyActive() && (args.size() < 3 || args[2] != channel->getKey())) {
				CommandInfo(c, args, ERR_BADCHANNELKEY, channel->getName() + ": " + BAD_CHANNEL_KEY);
			}
			else if (channel->isUserLimitActive() && (int)channel->getClients().size() >= channel->getUserLimit() && !channel->isInvitedClient(c)) {
				CommandInfo(c, args, ERR_CHANNELISFULL, channel->getName() + ": " + CHANNEL_IS_FULL);
			}
			else if (channel->isInviteOnlyActive() && !channel->isInvitedClient(c)) {
				CommandInfo(c, args, ERR_INVITEONLYCHAN, channel->getName() + ": " + INVITE_ONLY_CHAN);
			}
			else
			{
				if (!channel->isClient(c))
				{
					Channel::joinChannel(channel->getName(), c, s, false);
					if (channel->isInvitedClient(c))
						channel->removeInvitedClient(c);
				}
				else
					CommandInfo(c, args, ERR_USERONCHANNEL, args[1] + ": You're already on that channel");
			}
		}
	}
}

void WHO(Client &c, std::vector<std::string> args, Server &s)
{
	print_cmd(args[0], args);
	if (!c.HasRegistred())
		CommandInfo(c, args, ERR_BADPASSWORD, std::string("You must be registered to execute WHO"));
	else if (args.size() < 2)
		CommandInfo(c, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
	else if (args.size() > 2)
		CommandInfo(c, args, "NOTICE", "Too many arguments!");
	else {
		std::vector<Channel>::iterator channel = s.getChannelIterator(args[1]);
		if (channel != s.getChannels().end()) {
			s.getChannelIterator(args[1])->InformCurrentUsers();
		}
		else
			IRCsend(c.getFd(), GEN_MSG(ERR_NOSUCHCHANNEL, NO_SUCH_CHANNEL, c.getNickname()))
	}
		
}

void PART(Client &c, std::vector<std::string> args, Server &s)
{
	print_cmd(args[0], args);
	if (!c.HasRegistred()) {
		CommandInfo(c, args, ERR_BADPASSWORD, std::string("You must be registered to leave a channel"));
	} else if (args.size() < 2) {
		CommandInfo(c, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
	} else if (args[1][0] != '#') {
		IRCsend(c.getFd(), GEN_MSG("NOTICE", "Usage: PART [<channel>]", to_string(c.getFd())))
	} else
	{
		std::vector<Channel>::iterator channel = s.getChannelIterator(args[1]);
		if (channel == s.getChannels().end()) {
			CommandInfo(c, args, ERR_NOSUCHCHANNEL, args[1] + ": No such channel");
		}
		else if (!channel->isClient(c)) {
			CommandInfo(c, args, ERR_NOTONCHANNEL, args[1] + ": You're not on that channel");
		}
		else
		{
			std::string reason = "";
			if (args.size() > 3)
				reason.clear();
			for (size_t i = 2; i < args.size(); i++)
				reason.append(args[i] + " ");
			if (reason[reason.size() - 1] == ' ')
				reason = reason.substr(0, reason.size() - 1);

			std::string to_send = ":" + c.getNickname() + " PART " + channel->getName() + " " + reason + MSG_END;
			channel->InformCurrentUsers();
			channel->broadcast(c, to_send, true);
			channel->removeClient(c);
		}
	}
}

void PRIVMSG(Client &client, std::vector<std::string> args, Server &serv)
{
	print_cmd(args[0], args);

	if (!client.HasRegistred()) {
		CommandInfo(client, args, ERR_BADPASSWORD, std::string("You must be registered to send a private message"));
		return;
	}
	if (args.size() < 3) {
		CommandInfo(client, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
		return;
	}

	std::vector<Client>::iterator clientRecipient = serv.getClientIterator(args[1]);
	if (args[1][0] != '#' && !clientRecipient->HasRegistred()) {
		CommandInfo(client, args, ERR_NOSUCHNICK, args[1] + " :No such nick/channel");
		return;
	}
	// if server or client doesn't exist
	if (serv.findChannel(args[1]) == false && clientRecipient == serv.getClients().end()) {
		CommandInfo(client, args, ERR_NOSUCHCHANNEL, args[1] + " :No such nick/channel");	
		return;
	}
	
	std::string newMsg = "";
	for (size_t i = 2; i < args.size(); i++) {
		newMsg += args[i] + " ";
	}
	// remove the first character ':' if it exists
	if (newMsg[0] == ':') {
		newMsg = newMsg.substr(1, newMsg.size() - 1);
	}
	// remove the last space
	newMsg = newMsg.substr(0, newMsg.size() - 1);

	if (serv.findChannel(args[1])) {
		std::vector<Channel>::iterator channel = serv.getChannelIterator(args[1]);
		if (channel->isClient(client)) {
			// have its own implementation of broadcast to avoid sending the message to the sender
			std::vector<Client> clients = channel->getClients();
			for (size_t i = 0; i < clients.size(); i++) {
				if (clients[i].getNickname() != client.getNickname()) {
					IRCsend(clients[i].getFd(), PRIV_MSG(client.getNickname(), channel->getName(), newMsg))
				}
			}
		}
	} else if (clientRecipient != serv.getClients().end() && clientRecipient->HasRegistred()) {
		IRCsend(clientRecipient->getFd(), PRIV_MSG(client.getNickname(), clientRecipient->getNickname(), newMsg))
	}
}

// KICK #channel nickname
void KICK(Client &client, std::vector<std::string> args, Server &serv)
{
	print_cmd(args[0], args);
	if (!client.HasRegistred()) {
		CommandInfo(client, args, ERR_BADPASSWORD, std::string("You must be registered to kick a user"));
		return;
	}
	if (args.size() < 3) {
		CommandInfo(client, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
		return;
	}
	std::string channelName = args[1];
	std::vector<Channel>::iterator channel = serv.getChannelIterator(channelName);
	if (channel == serv.getChannels().end()) {
		CommandInfo(client, args, ERR_NOSUCHCHANNEL, ": No such channel");
		return;
	}

	// get the nickname of the client to kick
	std::string nickname = args[2];
	std::vector<Client>::iterator clientToKick = serv.getClientIterator(nickname);
	if (clientToKick == serv.getClients().end() || !clientToKick->HasRegistred()) {
		CommandInfo(client, args, ERR_NOSUCHNICK, nickname + ": No such nick");
		return;
	}

	if (!channel->isOperator(client)) {
		CommandInfo(client, args, ERR_CHANOPRIVSNEEDED, channelName + ": You're not a channel operator");
		return;
	}
	if (!channel->isClient(*clientToKick)) {
		CommandInfo(client, args, ERR_NOTONCHANNEL, clientToKick->getNickname() + ": is not on channel");
		return;
	}

	std::string reason = "";
	for (size_t i = 3; i < args.size(); i++)
		reason.append(args[i] + " ");
	if (reason[reason.size() - 1] == ' ')
		reason = reason.substr(0, reason.size() - 1);
	if (reason.empty())
		reason = client.getNickname();

	std::string to_send = ":" + client.getNickname() + " KICK " + channelName + " " + nickname + " " + reason + MSG_END;
	channel->InformCurrentUsers();
	channel->broadcast(client, to_send, true);
	channel->removeClient(*clientToKick);
}

void INVITE(Client &client, std::vector<std::string> args, Server &serv)
{
	print_cmd(args[0], args);
	if (!client.HasRegistred()) {
		CommandInfo(client, args, ERR_BADPASSWORD, std::string("You must be registered to invite to a channel"));
		return;
	}
	if (args.size() < 3) {
		CommandInfo(client, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
		return;
	}
	
	std::string channelName = args[2];
	std::vector<Channel>::iterator channel = serv.getChannelIterator(channelName);
	if (channel == serv.getChannels().end()) {
		IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channelName, channelName + ": No such nick/channel"));
		return;
	}
	
	if (!channel->isOperator(client)) {
		CommandInfo(client, args, ERR_CHANOPRIVSNEEDED, channelName + ": You're not a channel operator");
		return;
	}

	std::string nickname = args[1];
	// check if client exist on the server
	if (serv.getClientIterator(nickname) == serv.getClients().end()) {
		CommandInfo(client, args, ERR_NOSUCHNICK, nickname + ": No such nick");
		return;
	}
	std::vector<Client>::iterator clientToInvite = serv.getClientIterator(nickname);
	if (clientToInvite == serv.getClients().end() || !clientToInvite->HasRegistred()) {
		CommandInfo(client, args, ERR_NOSUCHNICK, nickname + ": No such nick");
		return;
	}
	// check if client is already in the channel or is already invited to the channel
	if (channel->isClient(*clientToInvite)){
		CommandInfo(client, args, ERR_USERONCHANNEL, nickname + ": is already on channel");
		return;
	}
	if (!channel->isInvitedClient(*clientToInvite)) {
		channel->addInvitedClient(*clientToInvite);
	}
	std::string msg = ":" + std::string(HOSTNAME) + " " + RPL_INVITING + " " + client.getNickname() + " " + clientToInvite->getNickname() + " " + channelName + MSG_END;
	IRCsend(client.getFd(), msg);
	std::string msg2 = ":" + client.getNickname() + " INVITE " + clientToInvite->getNickname() + " :" + channelName + MSG_END;
	IRCsend(clientToInvite->getFd(), msg2);
}

void TOPIC(Client &client, std::vector<std::string> args, Server &serv)
{
	print_cmd(args[0], args);
	if (!client.HasRegistred()) {
		CommandInfo(client, args, ERR_BADPASSWORD, std::string("You must be registered to get/change the topic"));
		return;
	}
	if (args.size() < 2) {
		CommandInfo(client, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
		return;
	}
	std::string channelName = args[1];
	std::vector<Channel>::iterator channel = serv.getChannelIterator(channelName);
	if (channel == serv.getChannels().end()) {
		CommandInfo(client, args, ERR_NOSUCHCHANNEL, args[1] + ": No such channel");
		return;
	} else if (!channel->isClient(client)) {
		CommandInfo(client, args, ERR_NOTONCHANNEL, args[1] + ": You're not on that channel");
		return;
	}

	// print topic
	if (args.size() == 2) {
		if (channel->getTopic().empty()) {
			std::string msg = ":" + std::string(HOSTNAME) + " " + RPL_NOTOPIC + " " + client.getNickname() + " " + channel->getName() + " :No topic is set" + MSG_END;
			IRCsend(client.getFd(), msg);
		}
		else {
			std::string msg = ":" + std::string(HOSTNAME) + " " + RPL_TOPIC + " " + client.getNickname() + " " + channel->getName() + " :" + channel->getTopic() + MSG_END;
			IRCsend(client.getFd(), msg);
			std::string msg2 = ":" + std::string(HOSTNAME) + " " + RPL_TOPICWHOTIME + " " + client.getNickname() + " " + channel->getName() + " " + channel->getTopicSetter() + " " + channel->getTopicTime() + MSG_END;
			IRCsend(client.getFd(), msg2);
		}
	}
	else if (channel->isRestrictedTopicActive() && !channel->isOperator(client)) {
		CommandInfo(client, args, ERR_CHANOPRIVSNEEDED, channelName + ": You're not a channel operator");
	}
	else {
		// set topic
		// newTopic is equal to all args after the channel name
		std::string newTopic = "";
		for (size_t i = 2; i < args.size(); i++) {
			newTopic += args[i] + " ";
		}
		// remove the first character ':'
		if (newTopic[0] == ':') {
			newTopic = newTopic.substr(1, newTopic.size() - 1);
		}
		// remove the last space
		newTopic = newTopic.substr(0, newTopic.size() - 1);

		channel->setTopic(newTopic);
		channel->setTopicSetter(client.getNickname() + "!~" + client.getUsername() + "@" + client.getHostname());
		channel->setTopicTime(to_string(time(NULL)));
		std::string to_send = ":" + channel->getTopicSetter() + " TOPIC " + channel->getName() + " :" + newTopic + MSG_END;
		channel->broadcast(client, to_send, true);
	}
}

void MODE(Client &client, std::vector<std::string> args, Server &serv)
{
	print_cmd(args[0], args);

	if (!client.HasRegistred()) {
		CommandInfo(client, args, ERR_BADPASSWORD, std::string("You must be registered execute MODE"));
		return;
	}
	if (args.size() < 2) {
		CommandInfo(client, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
		return;
	}
	std::string channelName = args[1];
	std::vector<Channel>::iterator channel = serv.getChannelIterator(channelName);
	if (channel == serv.getChannels().end()) {
		CommandInfo(client, args, ERR_NOSUCHCHANNEL, args[1] + ": No such channel");
		return;
	}

	if (args.size() < 3) {
		std::string msg = ":" + std::string(HOSTNAME) + " MODE " + channel->getName() + " " + channel->getModes() + MSG_END;
		IRCsend(client.getFd(), msg);
		return;
	} else if (!channel->isOperator(client)) {
		CommandInfo(client, args, ERR_CHANOPRIVSNEEDED, channelName + ": You're not a channel operator");
		return;
	}

	if (args[2] == "+i") {
		modePI(client, channel);
	} else if (args[2] == "-i") {
		modeMI(client, channel);
	} else if (args[2] == "+t") {
		modePT(client, channel);
	} else if (args[2] == "-t") {
		modeMT(client, channel);
	} else if (args[2] == "+k") {
		modePK(client, channel, args);
	} else if (args[2] == "-k") {
		modeMK(client, channel);
	} else if (args[2] == "+o") {
		modePO(client, serv, channel, args);
	} else if (args[2] == "-o") {
		modeMO(client, serv, channel, args);
	} else if (args[2] == "+l") {
		modePL(client, channel, args);
	} else if (args[2] == "-l") {
		modeML(client, channel);
	}
}

void modePI(Client &client, std::vector<Channel>::iterator &channel) {
	if (!channel->isInviteOnlyActive()) {
		channel->activateInviteOnly();
		std::string to_send = ":" + client.getNickname() + " MODE " + channel->getName() + " +i" + MSG_END;
		channel->broadcast(client, to_send, true);
	}
}
void modeMI(Client &client, std::vector<Channel>::iterator &channel) {
	if (channel->isInviteOnlyActive()) {
		channel->deactivateInviteOnly();
		std::string to_send = ":" + client.getNickname() + " MODE " + channel->getName() + " -i" + MSG_END;
		channel->broadcast(client, to_send, true);
	}
}

void modePT(Client &client, std::vector<Channel>::iterator &channel) {
	if (!channel->isRestrictedTopicActive()) {
		channel->activateRestrictedTopic();
		std::string to_send = ":" + client.getNickname() + " MODE " + channel->getName() + " +t" + MSG_END;
		channel->broadcast(client, to_send, true);
	}
}
void modeMT(Client &client, std::vector<Channel>::iterator &channel) {
	if (channel->isRestrictedTopicActive()) {
		channel->deactivateRestrictedTopic();
		std::string to_send = ":" + client.getNickname() + " MODE " + channel->getName() + " -t" + MSG_END;
		channel->broadcast(client, to_send, true);
	}
}

void modePK(Client &client, std::vector<Channel>::iterator &channel, std::vector<std::string> args) {
	if (args.size() < 4) {
		CommandInfo(client, args, ERR_NEEDMOREPARAMS, channel->getName() + ": Not enough parameters to run mode +k");
		return;
	}
	std::string key = args[3];
	channel->setKey(key);
	channel->activateKey();
	std::string to_send = ":" + client.getNickname() + " MODE " + channel->getName() + " +k " + key + MSG_END;
	channel->broadcast(client, to_send, true);
}
void modeMK(Client &client, std::vector<Channel>::iterator &channel) {
	if (channel->isKeyActive()) {
		channel->deactivateKey();
		std::string to_send = ":" + client.getNickname() + " MODE " + channel->getName() + " -k" + MSG_END;
		channel->broadcast(client, to_send, true);
	}
}

void modePO(Client &client, Server &serv, std::vector<Channel>::iterator &channel, std::vector<std::string> args) {
	if (args.size() < 4) {
		CommandInfo(client, args, ERR_NEEDMOREPARAMS, channel->getName() + ": Not enough parameters to run mode +o");
		return;
	}
	std::string nickname = args[3];
	std::vector<Client>::iterator clientToOp = serv.getClientIterator(args[3]);
	if (clientToOp == serv.getClients().end() || !clientToOp->HasRegistred()) {
		CommandInfo(client, args, ERR_NOSUCHNICK, "mode +o: " + nickname + ": No such nick");
		return;
	}
	// if the client does not exist
	if (channel->isClient(*clientToOp) == false){
		CommandInfo(client, args, ERR_NOSUCHNICK, "mode +o: " + nickname + ": No such nick");
		return;
	}
	// if the client is not an operator, give operator status
	if (!channel->isOperator(*clientToOp)) {
		channel->addOperator(*clientToOp);
		std::string to_send = ":" + client.getNickname() + " MODE " + channel->getName() + " +o " + nickname + MSG_END;
		channel->broadcast(client, to_send, true);
	}
}
void modeMO(Client &client, Server &serv, std::vector<Channel>::iterator &channel, std::vector<std::string> args) {
	if (args.size() < 4) {
		CommandInfo(client, args, ERR_NEEDMOREPARAMS, channel->getName() + ": Not enough parameters to run mode -o");
		return;
	}
	std::string nickname = args[3];
	std::vector<Client>::iterator clientToOp = serv.getClientIterator(args[3]);
	if (clientToOp == serv.getClients().end() || !clientToOp->HasRegistred()) {
		CommandInfo(client, args, ERR_NOSUCHNICK, "mode -o: " + nickname + ": No such nick");
		return;
	}
	// if the client does not exist
	if (channel->isClient(*clientToOp) == false){
		CommandInfo(client, args, ERR_NOSUCHNICK, "mode -o: " + nickname + ": No such nick");
		return;
	}
	// if the client is an operator, remove operator status
	if (channel->isOperator(*clientToOp)) {
		channel->removeOperator(*clientToOp);
		std::string to_send = ":" + client.getNickname() + " MODE " + channel->getName() + " -o " + nickname + MSG_END;
		channel->broadcast(client, to_send, true);
	}
}

void modePL(Client &client, std::vector<Channel>::iterator &channel, std::vector<std::string> args) {
	if (args.size() < 4) {
		CommandInfo(client, args, ERR_NEEDMOREPARAMS, channel->getName() + ": Not enough parameters to run mode +l");
		return;
	}
	std::string limit = args[3];
	// convert limit to double then int
	char* endPtr;
	double userLimit = strtod(limit.c_str(), &endPtr);
	// compare original string to endPtr to see if something was parsed
	if (endPtr == limit.c_str() || userLimit < 1 || userLimit > INT_MAX) {
		CommandInfo(client, args, "0", channel->getName() + ": Invalid channel limit");
		return;
	}
	channel->setUserLimit((int)userLimit);
	channel->activateUserLimit();
	std::string to_send = ":" + client.getNickname() + " MODE " + channel->getName() + " +l " + to_string(channel->getUserLimit()) + MSG_END;
	channel->broadcast(client, to_send, true);
}
void modeML(Client &client, std::vector<Channel>::iterator &channel) {
	if (channel->isUserLimitActive()) {
		channel->deactivateUserLimit();
		std::string to_send = ":" + client.getNickname() + " MODE " + channel->getName() + " -l" + MSG_END;
		channel->broadcast(client, to_send, true);
	}
}
