#include "../lib/ircserver.hpp"
#include "../lib/Client.hpp"
#include "../lib/Server.hpp"

int main(int ac, char *av[]) {
    if (ac != 3) {
        std::cout << "The program takes 2 arguments. Call the program like so: ./ircserv <port> <password>" << std::endl;
        return 0;
    }

	// parse and check prompt
    Server serv(444); // pass the port from av here

	try {
		signal(SIGINT, Server::signalHandler); // (ctrl + c)
  		signal(SIGQUIT, Server::signalHandler); // (ctrl + \)
		serv.serverInit();
	} catch (const std::exception &e) {
		serv.closePollFds();
		std::cerr << e.what() << std::endl;
	}
	
    (void)av;
    return 0;
}