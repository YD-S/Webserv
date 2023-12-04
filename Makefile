
CC=c++
# Debug levels
# 0 - DEBUG
# 1 - INFO
# 2 - WARNING
# 3 - ERROR

CFLAGS= -Wall -Wextra -Werror -DDEBUG=0 -MD -g

NAME= webserv

SRC_DIR= src
OBJ_DIR= obj
INC_DIR= includes

SRC_FILES = main.cpp config/ServerConfig.cpp config/LocationConfig.cpp \
 			HttpRequest.cpp HttpResponse.cpp HttpStatus.cpp MimeTypes.cpp \
 			fileValidate/fileValidate.cpp utils.cpp ICgiExecutor.cpp \
 			cgi/PhpCgiExecutor.cpp

SRC= $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ= $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.cpp=.o))

all: $(NAME)

-include $(OBJ:.o=.d)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "\033[32m[✔] $(NAME) created successfully!\033[0m"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CFLAGS) -I $(INC_DIR) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "\033[31m[✘] $(NAME) objects removed!\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[31m[✘] $(NAME) removed!\033[0m"

re: fclean all