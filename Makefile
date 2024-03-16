NAME 			= ircserv
SRCS 			= ./main.cpp ./client.cpp ./server.cpp
OBJS 			= $(SRCS:.cpp=.o)
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror -std=c++98
CXX				= c++

.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o ${<:.cpp=.o}

$(NAME): $(OBJS)
	$(CXX) -fsanitize=address $(CFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

fclean: clean
	$(RM) $(NAME)

clean:
	$(RM) -f $(OBJS) $(OBJS_B)

re: fclean all

.PHONY: all clean fclean re .cpp.o