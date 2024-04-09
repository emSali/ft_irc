#include "../lib/Channel.hpp"
#include "../lib/Server.hpp"
void Channel::newChannel(std::string name, Client &c, Server &s)
{
	Channel newChannel(name);
	
	s.addChannel(newChannel);

	std::cout << "Channel " << name << " created by " << c.getNickname() << std::endl;
	(void)s;
	(void)c;
	(void)name;
}

void Channel::joinChannel(std::string name, Client &c, Server &s)
{
	(void)s;
	(void)c;
	(void)name;
}