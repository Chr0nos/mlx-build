NAME=a.out
LINKER=-lXext -lX11 -lmlx -lm -lpthread
CC=clang
CFLAGS=-Wall -Werror -Wextra -Weverything -Wno-padded -O2

all: $(NAME)

$(NAME):
	$(CC) test_mlx.c mandelbrot.c mlx_terminate.c fill_image.c -o $(NAME) $(CFLAGS) $(LINKER)

fclean:
	$(RM) $(NAME)

re: fclean all
