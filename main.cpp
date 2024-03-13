#include <iostream>

int main(int ac, char *av[]) {
    if (ac != 3) {
        std::cout << "The program takes 2 arguments. Call the program like so: ./ircserv <port> <password>" << std::endl;
        return 0;
    }
    (void)av;
    return 0;
}