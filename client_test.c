
#include <string.h>
# include <unistd.h>

# include <poll.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <netdb.h>

int main() {

char *msg = "Beej was here!";
int len, bytes_sent;



len = strlen(msg);
bytes_sent = send(3, msg, len, 0);

return 0;
}