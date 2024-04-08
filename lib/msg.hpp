#ifndef MSG_HPP
# define MSG_HPP


// General
# define NEED_MORE_PARAMS "Need more parameters"
# define ERR_NEEDMOREPARAMS "461"
# define ERR_ALREADYREGISTRED "462"
# define ALREADY_REGISTRED "Already registred"

// Nickname
# define ERR_NONICKNAMEGIVEN "431"
# define ERR_NICKNAMEINUSE "433"
# define ERR_ERRONEUSNICKNAME "432"
# define ERR_NICKCOLLISION "436"

// Msg to server
# define CMD_ERR(c_fd, cmd, err) "[SERVER] <" + to_string(c_fd) + "> "+ cmd +" ERROR: " + err + "\n"
# define CMD_SET(c_fd, cmd, arg) "[SERVER] <" + to_string(c_fd) + "> Has set " + cmd + " to " + arg

// ERR_MSG to client
# define GEN_ERR(err, msg, nick) std::string(":" + std::string(HOSTNAME) + " " + err + " " + nick + " :" + msg + "\r\n")

#endif