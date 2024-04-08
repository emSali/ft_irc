#ifndef MSG_HPP
# define MSG_HPP


// Password
# define ERR_NEEDMOREPARAMS "461"
# define NEED_MORE_PARAMS "Need more parameters"
# define ERR_ALREADYREGISTRED "462"
# define ALREADY_REGISTRED "Already registred"

// Nickname
# define ERR_NONICKNAMEGIVEN "431"
# define NO_NICKNAME_GIVEN "No nickname given"
# define ERR_NICKNAMEINUSE "433"
# define NICKNAME_IN_USE "Nickname is already in use"
# define ERR_ERRONEUSNICKNAME "432"
# define ERRONEUS_NICKNAME "Nickname is erroneus"

// Msg to server
# define CMD_ERR(c_fd, cmd, err) "[SERVER] <" + to_string(c_fd) + "> "+ cmd +" ERROR: " + err + "\n"
# define CMD_SET(c_fd, cmd, arg) "[SERVER] <" + to_string(c_fd) + "> Has set " + cmd + " to " + arg

// ERR_MSG to client
# define GEN_ERR(err, msg, nick) std::string(":" + std::string(HOSTNAME) + " " + err + " " + nick + " :" + msg + "\r\n")

#endif