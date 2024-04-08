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
		void PASS(Client &c, std::string &cmd, std::vector<std::string> args);
		void NICK(Client &c, std::string &cmd, std::vector<std::string> args);
		void USER(Client &c, std::string &cmd, std::vector<std::string> args);
		void JOIN(Client &c, std::string &cmd, std::vector<std::string> args);
		void PRIVMSG(Client &c, std::string &cmd, std::vector<std::string> args);
		void KICK(Client &c, std::string &cmd, std::vector<std::string> args);
		void INVITE(Client &c, std::string &cmd, std::vector<std::string> args);
		void TOPIC(Client &c, std::string &cmd, std::vector<std::string> args);
		void MODE(Client &c, std::string &cmd, std::vector<std::string> args);

};

#endif