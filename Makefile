NAME = philo


SRC = main.c \
	  routine.c \
	  utils.c


all: $(NAME)

$(NAME):
	gcc $(SRC) -pthread -fsanitize=address -o $(NAME)


clean:
	rm -rf $(NAME)

fclean: clean

re: fclean all

.PHONY: fclean all re clean
