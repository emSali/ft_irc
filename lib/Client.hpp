#ifndef CLIENT_HPP
#define CLIENT_HPP

# include "ircserver.hpp"

// Contains the information about a specific client inside the server.

class Client {
    public:
		Client() : _fd(-1) {};
	
        Client(sockaddr_in addr, int fd) : _fd(fd),  client_addr(addr)
		{
			_nickname = "*";
			_username = "*";
			_has_nick = false;
			_has_user = false;
			_has_pass = false;
			_has_registred = false;
			_password = "*";
			_hostname = "*";
			_mode = "*";
			_realname = "*";
		};

		Client(const Client &client) : _fd(client._fd), client_addr(client.client_addr)
		{
			
			_has_nick = client._has_nick;
			_has_user = client._has_user;
			_has_pass = client._has_pass;
			_has_registred = client._has_registred;

			_password = client._password;
			_nickname = client._nickname;
			_username = client._username;
			
			_hostname = client._hostname;
			_mode = client._mode;
			_realname = client._realname;
			// _active_channels = client._active_channels;
		};

		Client & operator=(const Client &client)
		{
			_fd = client._fd;
			client_addr = client.client_addr;

			_has_nick = client._has_nick;
			_has_user = client._has_user;
			_has_pass = client._has_pass;
			_has_registred = client._has_registred;

			_password = client._password;
			_nickname = client._nickname;
			_username = client._username;
			
			_hostname = client._hostname;
			_mode = client._mode;
			_realname = client._realname;

			// _active_channels = client._active_channels;
			return *this;
		};

		~Client() {};

        int getFd() {return _fd;};
		void setFd(int fd) {_fd = fd;};

		std::string getPassword() {return _password;};
		void setPassword(std::string password) {_password = password; _has_pass = true;};
		std::string getNickname() {return _nickname;};
		void setNickname(std::string nickname) {_nickname = nickname; _has_nick = true;};
		std::string getUsername() {return _username;};
		void setUsername(std::string username) {_username = username; _has_user = true;};

		bool HasNick() {return _has_nick;};
		bool HasUser() {return _has_user;};
		bool HasPass() {return _has_pass;};

		bool HasRegistred() {return _has_registred;};
		void setRegistred(bool registred) {
			_has_registred = registred;
			if (registred)
				Authenticate_user();
			};
		void Authenticate_user() {
			std::cout << GEN_MSG(RPL_WELCOME, WELCOME, _nickname);
		}

		std::string getHostname() {return _hostname;};
		void setHostname(std::string hostname) {_hostname = hostname;};
		std::string getMode() {return _mode;};
		void setMode(std::string mode) {_mode = mode;};
		std::string getRealname() {return _realname;};
		void setRealname(std::string realname) {_realname = realname;};

		// std::vector<Channel> getChannels() {return _active_channels;};
		// void addChannel(Channel channel) {_active_channels.push_back(channel);};
		// void removeChannel(Channel channel) {
		// 	for (size_t i = 0; i < _active_channels.size(); i++) {
		// 		if (_active_channels[i].getName() == channel.getName()) {
		// 			_active_channels.erase(_active_channels.begin() + i);
		// 			break;
		// 		}
		// 	}
		// };
		
        sockaddr_in getClient_addr() {return client_addr;};

		std::string getBuffer() {return buffer;};
		void clearBuffer() {buffer.clear();};
		void appendBuffer(std::string msg) {buffer.append(msg);};


    private:
        int _fd;
		sockaddr_in client_addr;
		
		bool _has_nick;
		bool _has_user;
		bool _has_pass;
		bool _has_registred;
		
		std::string _password;
		std::string _nickname;
		std::string _username;
		std::string _hostname;
		std::string _mode;
		std::string _realname;

		std::string buffer;
		
};

#endif