#ifndef COMMANDS_HPP
#define COMMANDS_HPP

# include "ircserver.hpp"
# include "Client.hpp"
# include "Channel.hpp"

class Client;
class Channel;

class Commands {

	public:
		Commands() {};
		~Commands() {};
		bool isCommand(std::string &msg, Client &c);

	private:
		void NICK(Client &c, std::vector<std::string> args);

		void kick(Client client, Channel channel);
		void invite(Client client, Channel channel);
		std::string topic(Channel channel);
		void topic(Client client, Channel channel, std::string topic);
		void modeI(Client client, Channel channel);
		void modeT(Client client, Channel channel);
		void modeK(Client client, Channel channel);
		void modeO(Client client, Channel channel, std::string nickname);
		void modeL(Client client, Channel channel);
};

#endif