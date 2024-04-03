#include "../lib/ircserver.hpp"
#include "../lib/Client.hpp"
#include "../lib/Server.hpp"



int main(int ac, char *av[]) {
    Server serv;
	
    if (ac != 3) {
        std::cout << "The program takes 2 arguments. Call the program like so: ./ircserv <port> <password>" << std::endl;
        return 0;
    }
    // Server::_signal = false;

	
	
		// We use AF_INET because it's the only compatible with IPv4
		// We use stream socket because it's the only compatible with TCP
		// We use IPPROTO_TCP because it's the only compatible with TCP (same as getprotobyname("tcp"))
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(std::atoi(av[1])); // short, network byte order
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	std::memset(serv_addr.sin_zero, '\0', sizeof (serv_addr.sin_zero));

	fcntl(sockfd, F_SETFL, O_NONBLOCK); // Makes the socket non-blocking, as the subject requires
	
	std::cout << "trying to bind to port: " << std::atoi(av[1]) << std::endl;
	int b = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (b != 0)
		std::cerr << "Error binding socket" << std::endl;

	std::cout << "socket fd created: " << sockfd << std::endl;
	
    (void)serv;
    (void)av;
    return 0;
}	