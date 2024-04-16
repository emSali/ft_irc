#include "../lib/Channel.hpp"
#include "../lib/Server.hpp"


void Channel::newChannel(std::string name, Client &c, Server &s)
{
	Channel newChannel(name);
	
	s.addChannel(newChannel);
	Channel::joinChannel(name, c, s, true);
	std::cout << "Channel " << name << " created by " << c.getNickname() << std::endl;
}

void Channel::joinChannel(std::string name, Client &c, Server &s, bool op)
{
	std::vector<Channel>::iterator i = s.getChannelIterator(name);
	std::vector<Channel> Channels = s.getChannels();

	std::string msg = ":" + c.getNickname() + " JOIN " + name + MSG_END;
	if (i == Channels.end())
		std::cout << "Channel " << name << " does not exist" << std::endl;
	else if (op == true) {
		IRCsend(c.getFd(), msg)
		std::cout << "[SERVER]<" << c.getFd() << "> " << msg << std::endl;
		i->addOperator(c);
		i->addClient(c);
	}
	else 
	{
		IRCsend(c.getFd(), msg)
		i->addClient(c);
		std::cout << "[SERVER]<" << c.getFd() << "> Added to channel " << name << std::endl;
	}
}

std::string Channel::createMsg(const char* code)
{
	std::string cha_msg = ":" + std::string(HOSTNAME) + " ";

	cha_msg.append(code);
	cha_msg.append(" * "); cha_msg.append(this->_name);cha_msg.append(" ");
	cha_msg.append(to_string(this->_clients.size())); cha_msg.append(" ");
	cha_msg.append(":"); cha_msg.append(this->_topic); cha_msg.append(MSG_END);

	return cha_msg;
};

void Channel::broadcast(Client &client, std::string msg, bool isNotice)
{
	std::vector<Client> clients = this->_clients;
	for (std::vector<Client>::iterator i = clients.begin(); i != clients.end(); i++) {
		if (isNotice) {
			// to send a msg without it coming from a specific client, maybe use GEN_MSG
			// IRCsend(client.getFd(), ":" + this->getName() + " NOTICE " + this->_name + " :" + msg);
			// IRCsend(i->getFd(), GEN_MSG("NOTICE", msg, client.getNickname()));
			IRCsend(i->getFd(), msg);
		} else {
			IRCsend(i->getFd(), PRIV_MSG(client.getNickname(), this->_name, msg));
		}
	}
}

void Channel::broadcast(Client &client, std::string msg)
{
	std::vector<Client> clients = this->_clients;
	for (std::vector<Client>::iterator i = clients.begin(); i != clients.end(); i++)
		IRCsend(i->getFd(), PRIV_MSG(client.getNickname(), this->_name, msg));
}