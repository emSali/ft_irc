#include "../lib/ircserver.hpp"
#include "../lib/Client.hpp"
#include "../lib/Server.hpp"

int main(int ac, char *av[]) {
    if (ac != 3) {
        std::cout << "The program takes 2 arguments. Call the program like so: ./ircserv <port> <password>" << std::endl;
        return 0;
    }

	char *s;
	long int port_int = std::strtol(av[1], &s, 10);
	if (port_int < 1024 || port_int > 49151 || s[0] != '\0')
	{
		std::cout << "Invalid port number" << std::endl;
		return 1;
	}

	Server serv(av[1], av[2]);

	try {
		// parse and check prompt
		signal(SIGINT, Server::signal_to_close);
		signal(SIGQUIT, Server::signal_to_close);
		serv.serverInit();
		serv.serverStart();
	} catch (std::string e) {
		std::cerr << e << std::endl;
		serv.closePollFds();
	} catch (...){
		std::cout << "throw detected!" << std::endl;
	}
    return 0;
}	