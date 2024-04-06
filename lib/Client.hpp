#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "ircserver.hpp"

// Contains the information about a specific client inside the server.

class Client {
    public:
        Client(sockaddr_in addr, int fd) : _fd(fd),  client_addr(addr) {};

        int getFd() {return _fd;};
		void setFd(int fd) {_fd = fd;};

		std::string getNickname() {return _nickname;};
		void setNickname(std::string nickname) {_nickname = nickname;};

		std::string getUsername() {return _username;};
		void setUsername(std::string username) {_username = username;};

		std::string getRealname() {return _realname;};
		void setRealname(std::string realname) {_realname = realname;};

		std::string getCurrentChannel() {return _current_channel;};
		void setCurrentChannel(std::string current_channel) {_current_channel = current_channel;};

		std::string getActiveChannels() {return _active_channels;};
		void setActiveChannels(std::string active_channels) {_active_channels = active_channels;};


        sockaddr_in getClient_addr() {return client_addr;};

    private:
        int _fd;
		std::string _current_channel;
		std::string _nickname;
		std::string _username;
		std::string _realname;
		std::string _active_channels; 
		sockaddr_in client_addr;
};

#endif
