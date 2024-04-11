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
		IRCsend(c.getFd(), std::string(":OurIRCServer 332 nick #test :helloooo\r\n"))

		std::cout << "sent to: " << c.getFd() << std::string(" :OurIRCServer 332 nick #test :helloooo \r\n") << std::endl;
			i->addOperator(c); return ;
	}
	
		IRCsend(c.getFd(), std::string(":nick!~n@localhost 332 nick #test :helloooo\r\n"))
		IRCsend(c.getFd(), std::string(":OurIRCServer MODE #test +nt\r\n"))
		IRCsend(c.getFd(), std::string(":OurIRCServer 353 nick = #test :@nick\r\n"))
		IRCsend(c.getFd(), std::string(":OurIRCServer 366 nick #test :End of /NAMES list.\r\n"))

}