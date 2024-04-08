#ifndef MSG_HPP
# define MSG_HPP

// Password
# define ERR_NEEDMOREPARAMS "461"
# define ERR_NEEDMOREPARAMS_ "Need more parameters"
# define ERR_ALREADYREGISTRED "462"
# define ERR_ALREADYREGISTRED_ "already registred"

// Nickname
# define ERR_NONICKNAMEGIVEN "431"
# define ERR_NICKNAMEINUSE "433"
# define ERR_ERRONEUSNICKNAME "432"
# define ERR_NICKCOLLISION "436"

// Msg to server
# define GEN_ERR(err, msg, nick) std::string(":" + std::string(HOSTNAME) + " " + err + " " + nick + " :" + msg + "\r\n")
# define ALREADY_REGISTRED(c_fd) "[SERVER] <" + to_string(c_fd) + "> PASS ERROR: Already registred\n"
# define NEED_MORE_PARAMS(c_fd) "[SERVER] <" + to_string(c_fd) + "> PASS ERROR: Need more parameters\n"
# define SET_PASS(c_fd, pass) "[SERVER] <" + to_string(c_fd) + "> Has set password to " + pass

// Err to client
#endif