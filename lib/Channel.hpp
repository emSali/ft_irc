#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ircserver.hpp"

class Server;

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
        std::vector<Client> _clients;
        std::vector<Client> _operators;

    public:
        Channel() : _keyActive(false), _userLimitActive(false), _inviteOnly(false), _restrictedTopic(true) {
            _userLimit = 100;
            _topic = "No topic is set.";
        };

		Channel(std::string name) : _name(name), _keyActive(false), _userLimitActive(false), _inviteOnly(false), _restrictedTopic(true) {
			_userLimit = 100;
            _topic = "No topic is set.";
		};

		static void newChannel(std::string name, Client &c, Server &s);
		static void joinChannel(std::string name, Client &c, Server &s, bool op);

		std::string createMsg(const char* code);

	 	uint getNumberClient() { return (_clients.size() + _operators.size());}

        void setName(std::string name) {_name = name;};
        std::string getName() {return _name;};


        // KICK - Eject a client from the channel
		void removeClient(Client &client) {
            for (size_t i = 0; i < _clients.size(); i++) {
                if (_clients[i].getNickname() == client.getNickname()) {
                    _clients.erase(_clients.begin() + i);
                    break;
                }
            }
        }

        // INVITE - Invite a client to a channel
        void addClient(Client &client) {
			_clients.push_back(client);
		}
        bool isClient(Client &client){
            for (size_t i = 0; i < _clients.size(); i++) {
                if (_clients[i].getNickname() == client.getNickname()) {
                    return true;
                }
            }
            return false;
        }
        std::vector<Client> getClients() {return _clients;};

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
        void addOperator(Client &client) {_operators.push_back(client);};
		void removeOperator(Client &client) {
			for (size_t i = 0; i < _operators.size(); i++) {
				if (_operators[i].getNickname() == client.getNickname()) {
					_operators.erase(_operators.begin() + i);
					break;
				}
			}
		};
        bool isOperator(Client &client) {
            for (size_t i = 0; i < _operators.size(); i++) {
                if (_operators[i].getNickname() == client.getNickname()) {
                    return true;
                }
            }
            return false;
        };
		std::vector<Client> getOperators() {return _operators;};

        // MODE l: Set/remove the user limit to channel --> activateUserLimit(), deactivateUserLimit()
        void setUserLimit(int userLimit) {_userLimit = userLimit;};
        int getUserLimit() {return _userLimit;};
        void activateUserLimit() {_userLimitActive = true;}
        void deactivateUserLimit() {_userLimitActive = false;}
        bool isUserLimitActive() {return _userLimitActive;};

};

#endif