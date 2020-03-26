NAME=a.out
LINKER=-lXext -lX11 -lmlx
CC=clang
CFLAGS=-Wall -Werror -Wextra

all: $(NAME)

$(NAME):
	$(CC) test_mlx.c -o $(NAME) $(CFLAGS) $(LINKER)

fclean:
	$(RM) $(NAME)

re: fclean all
