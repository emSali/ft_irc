NAME 			= ircserv
SRCS 			= ./src/main.cpp ./src/Server.cpp ./src/Channel.cpp
OBJS 			= $(SRCS:.cpp=.o)
RM				= rm -f
CXXFLAGS		= -Wall -Wextra -Werror -std=c++98
CXX				= c++

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o ${<:.cpp=.o}

$(NAME): $(OBJS)
	$(CXX) -fsanitize=address $(CXXFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

fclean: clean
	$(RM) $(NAME)

clean:
	$(RM) -f $(OBJS) $(OBJS_B)

re: fclean all

.PHONY: all clean fclean re .cpp.o