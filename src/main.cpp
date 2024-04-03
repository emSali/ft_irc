#include "../lib/ircserver.hpp"
#include "../lib/Client.hpp"
#include "../lib/Server.hpp"



int main(int ac, char *av[]) {
    Server serv;
	
    if (ac != 3) {
        std::cout << "The program takes 2 arguments. Call the program like so: ./ircserv <port> <password>" << std::endl;
        return 0;
    }
	try {
		serv.serverInit(av[1], av[2]);
		serv.serverAccept();
	} catch(std::string &e) {
		std::cout << e << std::endl;
	}
	
    (void)av;
    return 0;
}	