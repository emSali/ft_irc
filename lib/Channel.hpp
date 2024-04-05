#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
#include <vector>

#include "Client.hpp"


class Client;

class Channel {
    private:
        std::string _name;
        std::string _topic;
        std::map<int, Client *> _clients;
        std::map<int, Client *> _operators;

    public:
        Channel();

        void setName(std::string name) {_name = name;};
        std::string getName() {return _name;};

        void setTopic(std::string topic) {_topic = topic;};
        std::string getTopic() {return _topic;};

        void addClient(Client &client);
		void removeClient(Client &client);
        bool isClient(Client &client);

        void addOperator(Client &client);
		void removeOperator(Client &client);
        bool isOperator(Client &client);

};

#endif