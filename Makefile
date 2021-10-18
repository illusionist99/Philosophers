NAME = philo

SRC = main.c \
	  routine.c \
	  utils.c \
	  more_utils.c


g_all: $(NAME)

$(NAME):
	gcc $(SRC) -pthread -Wall -Werror -Wextra  -o $(NAME)

clean:
	rm -rf $(NAME)

fclean: clean

re: fclean g_all

.PHONY: fclean g_all re clean
