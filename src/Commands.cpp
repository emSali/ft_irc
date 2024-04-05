#include "../lib/Commands.hpp"

void kick(Client client, Channel channel) {
    if (channel.isOperator(client) && channel.isClient(client)) {
        channel.removeClient(client);
        std::cout << client.getNickname() << " has left " << channel.getName() << std::endl;
    }
}

void invite(Client client, Channel channel) {
    if (channel.isClient(client)) {
        // Already in the channel
        return;
    } else if (channel.isUserLimitActive() && channel.getClients().size() >= channel.getUserLimit()) {
        // Channel is full
        return;
    } else if (channel.isOperator(client)) {
        channel.addClient(client);
        std::cout << client.getNickname() << " has joined " << channel.getName() << std::endl;
    }
}

// only operators can change the topic when it is not restricted. Otherwise, nobody can change the topic. Everyone can see the topic.
std::string topic(Channel channel) {
    return channel.getTopic();
}
void topic(Client client, Channel channel, std::string topic) {
    if (channel.isRestrictedTopicActive()) {
        return;
    } else if (channel.isOperator(client)) {
        channel.setTopic(topic);
        std::cout << client.getNickname() << " has changed the topic to: " << topic << std::endl;
    }
}

void modeI(Client client, Channel channel) {
    if (channel.isOperator(client) && !channel.isInviteOnlyActive()) {
        channel.activateInviteOnly();
        std::cout << channel.getName() << "Invite-only mode activated" << std::endl;
    } else if (channel.isOperator(client) && channel.isInviteOnlyActive()) {
        channel.deactivateInviteOnly();
        std::cout << channel.getName() << "Invite-only mode deactivated" << std::endl;
    }
}

void modeT(Client client, Channel channel) {
    if (channel.isOperator(client) && !channel.isRestrictedTopicActive()) {
        channel.activateRestrictedTopic();
        std::cout << channel.getName() << "Topic restricted mode activated" << std::endl;
    } else if (channel.isOperator(client) && channel.isRestrictedTopicActive()) {
        channel.deactivateRestrictedTopic();
        std::cout << channel.getName() << "Topic restricted mode deactivated" << std::endl;
    }
}

void modeK(Client client, Channel channel) {
    if (channel.isOperator(client) && channel.isKeyActive()) {
        channel.deactivateKey();
        std::cout << channel.getName() << "Key deactivated" << std::endl;
    } else if (channel.isOperator(client) && !channel.isKeyActive()) {
        channel.activateKey();
        std::cout << channel.getName() << "Key activated" << std::endl;
    }
}

void modeO(Client client, Channel channel, std::string nickname) {
    if (channel.isOperator(client)) {
        // if nickname is already an operator, remove operator status
        for (std::map<int, Client *> it = channel.getClients().begin(); it != channel.getClients().end(); it++) {
            if (it->second->getNickname() == nickname) {
                channel.removeOperator(*it->second);
                std::cout << nickname << " is no longer an operator in " << channel.getName() << std::endl;
                return;
            }
        }

        // if nickname is not an operator, add operator status
        for (std::map<int, Client *> it = channel.getClients().begin(); it != channel.getClients().end(); it++) {
            if (it->second->getNickname() == nickname) {
                channel.addOperator(*it->second);
                std::cout << nickname << " is now an operator in " << channel.getName() << std::endl;
                return;
            }
        }
    }
}

void modeL(Client client, Channel channel) {
    if (channel.isOperator(client) && !channel.isUserLimitActive()) {
        channel.activateUserLimit();
        std::cout << channel.getName() << "User limit activated" << std::endl;
    } else if (channel.isOperator(client) && channel.isUserLimitActive()) {
        channel.deactivateUserLimit();
        std::cout << channel.getName() << "User limit deactivated" << std::endl;
    } 
}
