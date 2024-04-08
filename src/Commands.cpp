#include "../lib/Commands.hpp"

bool Commands::isCommand(std::string &msg, Client &c)
{
	std::string command = msg.substr(0, msg.find(" "));

	if (command == "PASS") {
		PASS(c, split_string(msg, ' '));
	} else if (command == "NICK") {
		std::cout << "NICK command" << std::endl;
	} else if (command == "USER") {
		std::cout << "USER command" << std::endl;
	} else if (command == "JOIN") {
		std::cout << "JOIN command" << std::endl;
	} else if (command == "PRIVMSG") {
		std::cout << "PRIVMSG command" << std::endl;
	} else if (command == "KICK") {
		std::cout << "KICK command" << std::endl;
	} else if (command == "INVITE") {
		std::cout << "INVITE command" << std::endl;
	} else if (command == "TOPIC") {
		std::cout << "TOPIC command" << std::endl;
	} else if (command == "MODE") {
		std::cout << "MODE command" << std::endl;
	} else {
		return false;
	}

	return true;
}


void Commands::PASS(Client &c, std::vector<std::string> args)
{
	if (c.getRegistred())
	{
		std::cout << ALREADY_REGISTRED(c.getFd());

		std::string msg = GEN_ERR(ERR_ALREADYREGISTRED, ERR_ALREADYREGISTRED_, c.getNickname());
		if (send(c.getFd(), msg.c_str(), msg.size(), 0) == -1)
			std::cerr << "Error: send" << std::endl;
	}
	else if (args.size() == 1)
	{
		std::cout << NEED_MORE_PARAMS(c.getFd()) << std::endl;
		std::string msg = GEN_ERR(ERR_NEEDMOREPARAMS, ERR_NEEDMOREPARAMS_, c.getNickname());
		if (send(c.getFd(), msg.c_str(), msg.size(), 0) == -1)
			std::cerr << "Error: send" << std::endl;
	}
	else
	{
		c.setPassword(args[1]);
		std::cout << SET_PASS(c.getFd(), args[1]) << std::endl;
	}

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
