#include "../lib/Channel.hpp"
#include "../lib/Server.hpp"


void Channel::newChannel(std::string name, Client &c, Server &s)
{
	Channel newChannel(name);
	

	s.addChannel(newChannel);
	Channel::joinChannel(name, c, s, true);
	std::cout << "Channel " << name << " created by " << c.getNickname() << std::endl;
	(void)s;
	(void)c;
	(void)name;
}

void Channel::joinChannel(std::string name, Client &c, Server &s, bool op)
{
	std::vector<Channel>::iterator i = s.getChannel(name);
	std::vector<Channel> Channels = s.getChannels();
	if (i == Channels.end())
	{
		std::cout << "Channel " << name << " does not exist" << std::endl;
		return;
	}
	if (op == true) {

		std::string msg;
		msg.append(":");
		msg.append(HOSTNAME);
		msg.append(" JOIN :");
		msg.append(name);
		msg.append(MSG_END);
		
		IRCsend(c.getFd(), msg)
		std::cout << "sent to: <" << c.getFd() << "> " << msg << std::endl;
			i->addOperator(c); return ;
	}
	
	

}