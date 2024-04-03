#include "../lib/ircserver.hpp"
#include "../lib/Client.hpp"
#include "../lib/Server.hpp"

int main(int ac, char *av[]) {
    Client clien;
    Server serv;
	
    if (ac != 3) {
        std::cout << "The program takes 2 arguments. Call the program like so: ./ircserv <port> <password>" << std::endl;
        return 0;
    }

		// We use AF_INET because it's the only compatible with IPv4
		// We use stream socket because it's the only compatible with TCP
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(sockfd, F_SETFL, O_NONBLOCK); // Makes the socket non-blocking, as the subject requires
	
	std::cout << "socket fd created: " << sockfd << std::endl;
	
    (void)clien;
    (void)serv;
    (void)av;
    return 0;
}