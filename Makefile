NAME := ircserv

SOURCE :=	main.cpp	\
			Server.cpp	\
			User.cpp	\
			Message.cpp	\
			utils.cpp	\
			sendError.cpp	\
			sendReply.cpp	\
			Channel.cpp	\
			History.cpp	\
			Hash.cpp	\
			commands/channelCommands.cpp	\
			commands/otherCommands.cpp	\
			commands/registrationCommands.cpp	\
			commands/userCommands.cpp	\
			commands/ircOperatorCommands.cpp

SOURCE_DIR := src/

OBJ_DIR := objects/

INCLUDE_DIR := Include/

SRC= $(addprefix $(SOURCE_DIR), $(SOURCE))

OBJ= $(addprefix $(OBJ_DIR), $(SOURCE:.cpp=.o))

all : $(NAME)

$(OBJ_DIR):
	mkdir objects
	mkdir objects/commands

$(OBJ_DIR)%.o: $(SOURCE_DIR)%.cpp
	g++ -Wall -Werror -Wextra -c $< -o $@ -std=c++98 -I $(INCLUDE_DIR)

$(NAME): $(OBJ_DIR) $(OBJ)
	g++ $(OBJ) -o $(NAME) -std=c++98

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all