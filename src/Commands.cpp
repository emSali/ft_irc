#include "../lib/Commands.hpp"

bool Commands::isCommand(std::string &msg, Client &c)
{
	std::string command = msg.substr(0, msg.find(" "));
	if (command == "NICK")
	{
		NICK(c, split_string(msg, ' '));
		return true;
	}
	else if (command == "USER")
	{
		std::cout << "[" << c.getFd() << "]" << "USER command: " << msg.substr(msg.find(" ") + 1) << std::endl;
		c.setUsername(msg.substr(msg.find(" ") + 1));
		return true;

	}
	else if (command == "PASS")
	{
		c.setPassword(msg.substr(msg.find(" ") + 1));
		std::cout << "PASS command" << std::endl;
		return true;
	}
	else if (command == "JOIN")
	{
		std::cout << "JOIN command" << std::endl;
		std::string response = ":IRC JOIN #linux\r\n";
		send(c.getFd(), response.c_str(), response.size(), 0);

		return true;
	}
	return false;
}

void Commands::NICK(Client &c, std::vector<std::string> args)
{
	if (args.size() < 2)
	{
		std::cout << "Not enough arguments for NICK command" << std::endl;
		if (send(c.getFd(), "Not enough arguments for NICK command\n", 34, 0) == -1)
			std::cerr << "Error sending message to client: " << c.getFd() << std::endl;
		return;
	}
	c.setNickname(args[1]);
	std::cout << "[" << c.getFd() << "]" << " changed nickname to: " << c.getNickname() << std::endl;	
}

void Commands::USER(Client &c, std::vector<std::string> args)
{
	if (args.size() < 5)
	{
		std::cout << "Not enough arguments for USER command" << std::endl;
		if (send(c.getFd(), "Not enough arguments for USER command\n", 34, 0) == -1)
			std::cerr << "Error sending message to client: " << c.getFd() << std::endl;
		return;
	}
	c.setUsername(args[1]);
	c.setRealname(args[4].erase(args[4].size() - 1));
	std::cout << "[" << c.getFd() << "]" << " changed username to: " << c.getUsername() << std::endl;
	std::cout << "[" << c.getFd() << "]" << " changed realname to: " << c.getRealname() << std::endl;
}

// void Commands::kick(Client client, Channel channel) {
//     if (channel.isOperator(client) && channel.isClient(client)) {
//         channel.removeClient(client);
//         std::cout << client.getNickname() << " has left " << channel.getName() << std::endl;
//     }
// }

// void Commands::invite(Client client, Channel channel) {
//     if (channel.isClient(client)) {
//         // Already in the channel
//         return;
//     } else if (channel.isUserLimitActive() && channel.getClients().size() >= channel.getUserLimit()) {
//         // Channel is full
//         return;
//     } else if (channel.isOperator(client)) {
//         channel.addClient(client);
//         std::cout << client.getNickname() << " has joined " << channel.getName() << std::endl;
//     }
// }

// // only operators can change the topic when it is not restricted. Otherwise, nobody can change the topic. Everyone can see the topic.
// std::string topic(Channel channel) {
//     return channel.getTopic();
// }
// void Commands::topic(Client client, Channel channel, std::string topic) {
//     if (channel.isRestrictedTopicActive()) {
//         return;
//     } else if (channel.isOperator(client)) {
//         channel.setTopic(topic);
//         std::cout << client.getNickname() << " has changed the topic to: " << topic << std::endl;
//     }
// }

// void Commands::modeI(Client client, Channel channel) {
//     if (channel.isOperator(client) && !channel.isInviteOnlyActive()) {
//         channel.activateInviteOnly();
//         std::cout << channel.getName() << "Invite-only mode activated" << std::endl;
//     } else if (channel.isOperator(client) && channel.isInviteOnlyActive()) {
//         channel.deactivateInviteOnly();
//         std::cout << channel.getName() << "Invite-only mode deactivated" << std::endl;
//     }
// }

// void Commands::modeT(Client client, Channel channel) {
//     if (channel.isOperator(client) && !channel.isRestrictedTopicActive()) {
//         channel.activateRestrictedTopic();
//         std::cout << channel.getName() << "Topic restricted mode activated" << std::endl;
//     } else if (channel.isOperator(client) && channel.isRestrictedTopicActive()) {
//         channel.deactivateRestrictedTopic();
//         std::cout << channel.getName() << "Topic restricted mode deactivated" << std::endl;
//     }
// }

// void Commands::modeK(Client client, Channel channel) {
//     if (channel.isOperator(client) && channel.isKeyActive()) {
//         channel.deactivateKey();
//         std::cout << channel.getName() << "Key deactivated" << std::endl;
//     } else if (channel.isOperator(client) && !channel.isKeyActive()) {
//         channel.activateKey();
//         std::cout << channel.getName() << "Key activated" << std::endl;
//     }
// }

// void Commands::modeO(Client client, Channel channel, std::string nickname) {
//     if (channel.isOperator(client)) {
//         // if nickname is already an operator, remove operator status
//         for (std::map<int, Client *> it = channel.getClients().begin(); it != channel.getClients().end(); it++) {
//             if (it->second->getNickname() == nickname) {
//                 channel.removeOperator(*it->second);
//                 std::cout << nickname << " is no longer an operator in " << channel.getName() << std::endl;
//                 return;
//             }
//         }

//         // if nickname is not an operator, add operator status
//         for (std::map<int, Client *> it = channel.getClients().begin(); it != channel.getClients().end(); it++) {
//             if (it->second->getNickname() == nickname) {
//                 channel.addOperator(*it->second);
//                 std::cout << nickname << " is now an operator in " << channel.getName() << std::endl;
//                 return;
//             }
//         }
//     }
// }

// void Commands::modeL(Client client, Channel channel) {
//     if (channel.isOperator(client) && !channel.isUserLimitActive()) {
//         channel.activateUserLimit();
//         std::cout << channel.getName() << "User limit activated" << std::endl;
//     } else if (channel.isOperator(client) && channel.isUserLimitActive()) {
//         channel.deactivateUserLimit();
//         std::cout << channel.getName() << "User limit deactivated" << std::endl;
//     } 
// }
