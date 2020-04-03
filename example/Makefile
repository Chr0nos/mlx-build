NAME=a.out
LINKER=-lXext -lX11 -lmlx -lm
CC=clang
CFLAGS=-Wall -Werror -Wextra -O2 -Weverything -Wno-padded

all: $(NAME)

$(NAME):
	$(CC) test_mlx.c mandelbrot.c mlx_terminate.c -o $(NAME) $(CFLAGS) $(LINKER)

fclean:
	$(RM) $(NAME)

re: fclean all
