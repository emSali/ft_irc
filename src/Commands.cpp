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
	else if (args.size() == 1)
		CommandInfo(c, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
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
	else if (args.size() < 5)
		CommandInfo(c, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
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


// When a USER leave a channel it sends: "PART #ok :Leaving"
// we don't hangle it yet. the client is not removed from the channel
void JOIN(Client &c, std::vector<std::string> args, Server &s)
{
	print_cmd(args[0], args);
	if (args.size() == 1)
		CommandInfo(c, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
	else if (args[1][0] != '#')
		CommandInfo(c, args, ERR_NOSUCHCHANNEL, args[1] + " :No such channel");
	// if channel doesn't exist, create it
	if (s.findChannel(args[1]) == false) {
		Channel::newChannel(args[1], c, s);
	}
	else
	{
		std::vector<Channel>::iterator channel = s.getChannelIterator(args[1]);
		if (channel->isInviteOnlyActive() && !channel->isInvitedClient(c))
			CommandInfo(c, args, ERR_INVITEONLYCHAN, INVITE_ONLY_CHAN);
		else if (channel->isUserLimitActive() && (int)channel->getClients().size() >= channel->getUserLimit() && !channel->isInvitedClient(c))
			CommandInfo(c, args, ERR_CHANNELISFULL, CHANNEL_IS_FULL);
		else if (channel->isKeyActive() && (args.size() == 2 || args[2] != channel->getKey()))
			CommandInfo(c, args, ERR_BADCHANNELKEY, BAD_CHANNEL_KEY);
		else
		{
			if (channel->isClient(c) == false)
			{
				Channel::joinChannel(channel->getName(), c, s, false);
				IRCsend(c.getFd(), PRIV_MSG(c.getNickname(), channel->getName(), "Now talking on " + channel->getName()))
				channel->broadcast(c, c.getNickname() + " has joined " + channel->getName());
			}
			if (channel->isInvitedClient(c))
				channel->removeInvitedClient(c);
		}
	}
}

void PART(Client &c, std::vector<std::string> args, Server &s)
{
	print_cmd(args[0], args);
	if (args.size() == 1)
		CommandInfo(c, args, ERR_NEEDMOREPARAMS, NEED_MORE_PARAMS);
	else if (args[1][0] != '#')
		IRCsend(c.getFd(), GEN_MSG("NOTICE", "Usage: PART [<channel>]", to_string(c.getFd())))
	else
	{
		std::vector<Channel>::iterator channel = s.getChannelIterator(args[1]);
		if (channel->isClient(c) == false)
			CommandInfo(c, args, ERR_NOSUCHCHANNEL, args[1] + " :No such channel");
		else
		{
			// remove the client from the channel
			channel->removeClient(c);
			IRCsend(c.getFd(), PRIV_MSG(c.getNickname(), channel->getName(), "Leaving"))
			channel->broadcast(c, c.getNickname() + " has left " + channel->getName());
		}
	}
}

// sending a msg in a channel --> PRIVMSG #bitcoin :hey
// @time=2024-04-12T13:55:42.526Z :loris!~lorislori@2001:8a0:7ac8:2300:20bc:455c:deab:d375 PRIVMSG #okokok :ola
// sending a msg to a user --> PRIVMSG user :hey
// @time=2024-04-12T14:07:23.350Z :loris!~lorislori@2001:8a0:7ac8:2300:20bc:455c:deab:d375 PRIVMSG lolo :hey dude
void PRIVMSG(Client &client, std::vector<std::string> args, Server &serv)
{
	print_cmd(args[0], args);

	if (args.size() < 3) {
		return;
	}
	// if server or client doesn't exist
	if (serv.findChannel(args[1]) == false && serv.getClientIterator(args[1]) == serv.getClients().end()) {
		return;
	}

	std::string newMsg = "";
	for (size_t i = 2; i < args.size(); i++) {
		newMsg += args[i] + " ";
	}
	// remove the first character ':'
	newMsg = newMsg.substr(1, newMsg.size() - 1);
	// remove the last space
	newMsg = newMsg.substr(0, newMsg.size() - 1);

	if (serv.findChannel(args[1])) {
		std::vector<Channel>::iterator channel = serv.getChannelIterator(args[1]);
		if (channel->isClient(client)) {
			std::vector<Client> clients = channel->getClients();
			for (size_t i = 0; i < clients.size(); i++) {
				if (clients[i].getNickname() != client.getNickname()) {
					IRCsend(clients[i].getFd(), PRIV_MSG(client.getNickname(), channel->getName(), newMsg))
				}
			}
		}
	} else if (serv.getClientIterator(args[1]) != serv.getClients().end()) {
		std::vector<Client>::iterator sentClient = serv.getClientIterator(args[1]);
		IRCsend(sentClient->getFd(), PRIV_MSG(client.getNickname(), sentClient->getNickname(), newMsg))
	}
}

// KICK #channel nickname
void KICK(Client &client, std::vector<std::string> args, Server &serv)
{
	print_cmd(args[0], args);
	std::string channelName = args[1];
	std::vector<Channel>::iterator channel = serv.getChannelIterator(channelName);

	// get the nickname of the client to kick
	std::string nickname = args[2];
	std::vector<Client>::iterator clientToKick = serv.getClientIterator(nickname);

	if (!channel->isOperator(client)) {
		// @time=2024-04-12T13:31:42.528Z :calcium.libera.chat 482 loris #ubuntu :You're not a channel operator
		// print: #ubuntu :You're not a channel operator
		IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channel->getName(), channelName + " :You're not a channel operator"));
		return;
	}
	if (!channel->isClient(*clientToKick)) {
		// @time=2024-04-12T13:42:22.833Z :calcium.libera.chat 401 loris nickname :No such nick/channel
		// print nothing
		return;
	}
	// @time=2024-04-12T13:43:19.153Z :loris!~lorislori@2001:8a0:7ac8:2300:20bc:455c:deab:d375 KICK #jiofdsnog loris :loris
	channel->removeClient(*clientToKick);
	// print : You have been kicked from #jiofdsnog by loris (loris)
	IRCsend(clientToKick->getFd(), PRIV_MSG(client.getNickname(), channel->getName(), channelName + " :" + "You have been kicked from " + channelName + " by " + client.getNickname()));
	// print to the channel: client has kicked clientToKick from #okokok (clientToKick)
	channel->broadcast(client, client.getNickname() + " has kicked " + clientToKick->getNickname() + " from " + channelName);
}

void INVITE(Client &client, std::vector<std::string> args, Server &serv)
{
	print_cmd(args[0], args);
	if (args.size() < 3) {
		return;
	}
	std::string channelName = args[2];
	// check if channel exist
	if (serv.findChannel(channelName) == false) {
		// IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channelName, channelName + " :No such channel"));
		return;
	}
	std::vector<Channel>::iterator channel = serv.getChannelIterator(channelName);
	
	if (!channel->isOperator(client)) {
		IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channel->getName(), channelName + " :You're not a channel operator"));
		return;
	}

	std::string nickname = args[1];
	// check if client exist on the server
	if (serv.getClientIterator(nickname) == serv.getClients().end()) {
		// IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channel->getName(), nickname + " :No such nick/channel"));
		return;
	}
	Client clientToInvite = serv.getClient(nickname);
	// check if client is already in the channel or is already invited to the channel
	if (channel->isClient(clientToInvite) || channel->isInvitedClient(clientToInvite)){
		return;
	}
	channel->addInvitedClient(clientToInvite);
	// print: You've invited user1__ to #okkkkk (iridium.libera.chat)
	IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channel->getName(), "You've invited " + clientToInvite.getNickname() + " to " + channelName));
	// print: You have been invited to #okkkkk by user2_ (iridium.libera.chat)
	IRCsend(clientToInvite.getFd(), GEN_MSG("NOTICE", "You have been invited to " + channelName + " by " + client.getNickname(), to_string(clientToInvite.getFd())))
}

void TOPIC(Client &client, std::vector<std::string> args, Server &serv)
{
	print_cmd(args[0], args);
	std::string channelName = args[1];
	std::vector<Channel>::iterator channel = serv.getChannelIterator(channelName);

	// print topic
	if (args.size() == 2) {
		IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channel->getName(), channel->getTopic()));
		return;
	}

	// set topic
	if (channel->isRestrictedTopicActive() && !channel->isOperator(client)) {
		IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channel->getName(), channelName + " :You're not a channel operator"));
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

	channel->setTopic(newTopic);
	// print: loris has changed the topic to: new topic is this..
	channel->broadcast(client, client.getNickname() + " has changed the topic to: " + newTopic);
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
	std::vector<Channel>::iterator channel = serv.getChannelIterator(channelName);

	if (args.size() < 3) {
		return;
	} else if (!channel->isOperator(client)) {
		IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channel->getName(), channel->getName() + " :You're not a channel operator"));
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
		channel->broadcast(client, client.getNickname() + " sets mode +i on " + channel->getName());
		return;
	}
}
void modeMI(Client &client, std::vector<Channel>::iterator &channel) {
	if (channel->isInviteOnlyActive()) {
		channel->deactivateInviteOnly();
		channel->broadcast(client, client.getNickname() + " sets mode -i on " + channel->getName());
		return;
	}
}

void modePT(Client &client, std::vector<Channel>::iterator &channel) {
	if (!channel->isRestrictedTopicActive()) {
		channel->activateRestrictedTopic();
		channel->broadcast(client, client.getNickname() + " sets mode +t on " + channel->getName());
		return;
	}
}
void modeMT(Client &client, std::vector<Channel>::iterator &channel) {
	if (channel->isRestrictedTopicActive()) {
		channel->deactivateRestrictedTopic();
		channel->broadcast(client, client.getNickname() + " sets mode -t on " + channel->getName());
		return;
	}
}

void modePK(Client &client, std::vector<Channel>::iterator &channel, std::vector<std::string> args) {
	if (args.size() < 4) {
		IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channel->getName(), channel->getName() + ": Not enough parameters to run mode +k"));
		return;
	}
	std::string key = args[3];
	channel->setKey(key);
	channel->activateKey();
	channel->broadcast(client, client.getNickname() + " sets channel keyword to " + key);
}
void modeMK(Client &client, std::vector<Channel>::iterator &channel) {
	if (channel->isKeyActive()) {
		channel->deactivateKey();
		channel->broadcast(client, client.getNickname() + " removes channel keyword");
		return;
	}
}

void modePO(Client &client, Server &serv, std::vector<Channel>::iterator &channel, std::vector<std::string> args) {
	if (args.size() < 4) {
		IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channel->getName(), channel->getName() + ": Not enough parameters to run mode +o"));
		return;
	}
	std::string nickname = args[3];
	Client clientToOp = serv.getClient(nickname);
	// if the client does not exist
	if (channel->isClient(clientToOp) == false){
		IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channel->getName(), nickname + " :No such nick/channel"));
		return;
	}
	// if the client is not an operator, give operator status
	if (!channel->isOperator(clientToOp)) {
		channel->addOperator(clientToOp);
		channel->broadcast(client, client.getNickname() + " gives channel operator status to " + nickname);
	}
}
void modeMO(Client &client, Server &serv, std::vector<Channel>::iterator &channel, std::vector<std::string> args) {
	if (args.size() < 4) {
		IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channel->getName(), channel->getName() + ": Not enough parameters to run mode -o"));
		return;
	}
	std::string nickname = args[3];
	Client clientToOp = serv.getClient(nickname);
	// if the client does not exist
	if (channel->isClient(clientToOp) == false){
		IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channel->getName(), nickname + " :No such nick/channel"));
		return;
	}
	// if the client is an operator, remove operator status
	if (channel->isOperator(clientToOp)) {
		channel->removeOperator(clientToOp);
		channel->broadcast(client, client.getNickname() + " removes channel operator status from " + nickname);
	}
}

void modePL(Client &client, std::vector<Channel>::iterator &channel, std::vector<std::string> args) {
	if (args.size() < 4) {
		IRCsend(client.getFd(), PRIV_MSG(client.getNickname(), channel->getName(), channel->getName() + ": Not enough parameters to run mode +l"));
		return;
	}
	// if (!channel->isUserLimitActive()) {
		std::string limit = args[3];
		// convert limit to double then int
		char* endPtr;
		double userLimit = strtod(limit.c_str(), &endPtr);
		// compare original string to endPtr to see if something was parsed
		if (endPtr == limit.c_str() || userLimit < 1 || userLimit > INT_MAX) {
			return;
		}
		channel->setUserLimit((int)userLimit);
		channel->activateUserLimit();
		channel->broadcast(client, client.getNickname() + " sets channel limit to " + to_string(channel->getUserLimit()));
	// }
}
void modeML(Client &client, std::vector<Channel>::iterator &channel) {
	if (channel->isUserLimitActive()) {
		channel->deactivateUserLimit();
		channel->broadcast(client, client.getNickname() + " removes user limit");
	}
}
