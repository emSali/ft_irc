#include "../lib/ircserver.hpp"
#include "../lib/Client.hpp"
#include "../lib/Server.hpp"

int main(int ac, char *av[]) {
    if (ac != 3) {
        std::cout << "The program takes 2 arguments. Call the program like so: ./ircserv <port> <password>" << std::endl;
        return 0;
    }

	// parse and check prompt
    Server serv(av[1], av[2]);

	try {
		signal(SIGINT, Server::signal_to_close);
		signal(SIGQUIT, Server::signal_to_close);
		serv.serverInit();
		serv.serverStart();
	} catch (const std::exception &e) {
		std::cerr << "ERROR\n";
		std::cerr << e.what() << std::endl;
		serv.closePollFds();
	}
	
    (void)av;
    return 0;
}	