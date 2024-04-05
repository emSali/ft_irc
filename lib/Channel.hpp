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
        std::string _key;
        bool _keyActive;    // default value false.
        int _userLimit;
        bool _userLimitActive;
        bool _inviteOnly;
        bool _restrictedTopic;
        std::map<int, Client *> _clients;
        std::map<int, Client *> _operators;

    public:
        Channel() : _keyActive(false), _userLimitActive(false), _inviteOnly(false), _restrictedTopic(false) {
            _userLimit = 100;
        };

        void setName(std::string name) {_name = name;};
        std::string getName() {return _name;};


        // KICK - Eject a client from the channel
		void removeClient(Client &client);

        // INVITE - Invite a client to a channel
        void addClient(Client &client);
        bool isClient(Client &client);
        std::map<int, Client *> getClients() {return _clients;};

        // TOPIC - Change or view the channel topic
        // TOPIC <text> --> setTopic()
        void setTopic(std::string topic) {_topic = topic;};
        // TOPIC --> getTopic()
        std::string getTopic() {return _topic;};

        // MODE i: Set/remove Invite-only channel --> activateInviteOnly(), deactivateInviteOnly()
        void activateInviteOnly() {_inviteOnly = true;}
        void deactivateInviteOnly() {_inviteOnly = false;}
        bool isInviteOnlyActive() {return _inviteOnly;};

        // MODE t: Set/remove the restrictions of the TOPIC command to channel operators --> activateRestrictedTopic(), deactivateRestrictedTopic()
        void activateRestrictedTopic() {_restrictedTopic = true;}
        void deactivateRestrictedTopic() {_restrictedTopic = false;}
        bool isRestrictedTopicActive() {return _restrictedTopic;};

        // MODE k: Set/remove the channel key (password) --> activateKey(), deactivateKey()
        void setKey(std::string key) {_key = key;};
        std::string getKey() {return _key;};
        void activateKey() {_keyActive = true;}
        void deactivateKey() {_keyActive = false;}
        bool isKeyActive() {return _keyActive;};

        // MODE o: Give/take channel operator privilege --> addOperator(), removeOperator()
        void addOperator(Client &client);
		void removeOperator(Client &client);
        bool isOperator(Client &client);

        // MODE l: Set/remove the user limit to channel --> activateUserLimit(), deactivateUserLimit()
        void setUserLimit(int userLimit) {_userLimit = userLimit;};
        int getUserLimit() {return _userLimit;};
        void activateUserLimit() {_userLimitActive = true;}
        void deactivateUserLimit() {_userLimitActive = false;}
        bool isUserLimitActive() {return _userLimitActive;};

};

#endif