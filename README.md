# ft_irc

Internet Relay Chat or IRC is a text-based communication protocol on the Internet.
It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.
IRC clients connect to IRC servers in order to join channels. IRC servers are connected
together to form a network.

The server forms the backbone of IRC as it is the only component of the protocol which is able to link all the other components together.
It provides a point to which clients may connect to talk to each other, and a point for other servers to connect to each other. The server is also responsible for providing the basic services defined by the IRC protocol. Servers provide connections between client to client, client to server, and server to server.

1. setting up a communication endpoint and signal handling mechanisms
we need two classes, a server and a client:
- server: contain all the information about the server. It will also have a vector of clients to keep track of all the clients and manage their requests.
- client: contain all the information about a specific client inside the server.
We then need to use sockets. Essentially, a socket is a file descriptor. A socket is an endpoint that enables two processes to communicate with each other, either on the same machine or across a network.

Articles:
(long) Internet Relay Chat Protocol - May 1993: https://datatracker.ietf.org/doc/html/rfc1459
(long) Modern IRC Client Protocol: https://modern.ircdocs.horse/
(short) IRC Introduction and its main ingredients: https://medium.com/the-complete-guide-for-irc-network-i-e-freenode/irc-introduction-and-its-main-ingredients-982cd3639a67

Tutorials:
Small IRC Server [ft_irc 42 network]: https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9

IRC client:
IRSSI: https://irssi.org/
HexChat, mIRC, WeeChat, and also LimeChat

How to connect and chat in IRC
https://stackoverflow.com/questions/12655951/establishing-a-connection-to-irc

In-deepth explanation how it works
https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf


Launching:
from this repo:
https://github.com/twagger/ft_irc

start server:
./ircserv 6667 my-password

Connect with NetCat
nc -C 127.0.0.1 6667
pass my-password
user tom 0 * :tom
nick tom

Connect with another user
nc -C 127.0.0.1 6667
pass my-password
user rick 0 * :rick
nick rick

Join a channel
JOIN #main

Send private msg
PRIVMSG tom messagetext