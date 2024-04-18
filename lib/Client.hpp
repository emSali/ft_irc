#ifndef CLIENT_HPP
#define CLIENT_HPP

# include "ircserver.hpp"
// Contains the information about a specific client inside the server.

class Server;

class Client {
    public:
        Client(sockaddr_in addr, int fd, char *hostname) : _fd(fd),  client_addr(addr), _hostname(hostname)
		{
			_nickname = "*";
			_username = "*";
			_has_nick = false;
			_has_user = false;
			_has_pass = false;
			_has_registred = false;
			_password = "*";
			_mode = "*";
			_realname = "*";
		};

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
		void RegisterClient(int fd) {
			_has_registred = true;
			
			IRCsend(fd, GEN_MSG(RPL_WELCOME, WELCOME, _nickname))
			std::cout << "Client " << fd << " Authenticated :D" << std::endl;
			
		};

		std::string getHostname() {return _hostname;};
		void setHostname(std::string hostname) {_hostname = hostname;};
		std::string getMode() {return _mode;};
		void setMode(std::string mode) {_mode = mode;};
		std::string getRealname() {return _realname;};
		void setRealname(std::string realname) {_realname = realname;};
		
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