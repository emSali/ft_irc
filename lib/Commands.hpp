#ifndef COMMANDS_HPP
#define COMMANDS_HPP

# include "ircserver.hpp"
# include "Channel.hpp"

class Client;
class Channel;

class Commands {

	public:
		Commands() {};
		~Commands() {};
		bool isCommand(std::string &msg, Client &c, std::vector<Client> &clients);

	private:
		void PASS(Client &c, std::vector<std::string> args);
		void NICK(Client &c, std::vector<std::string> args, std::vector<Client> &clients);
		void USER(Client &c, std::vector<std::string> args);
		void JOIN(Client &c, std::vector<std::string> args);
		void PRIVMSG(Client &c, std::vector<std::string> args);
		void KICK(Client &c, std::vector<std::string> args);
		void INVITE(Client &c, std::vector<std::string> args);
		void TOPIC(Client &c, std::vector<std::string> args);
		void MODE(Client &c, std::vector<std::string> args);

};

#endif