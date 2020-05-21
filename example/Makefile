NAME=a.out
LINKER=-lXext -lX11 -lmlx -lpthread
CC=clang
CFLAGS=-Wall -Werror -Wextra -Weverything -Wno-padded -O2
OBJS=test_mlx.o mandelbrot.o mlx_terminate.o fill_image.o


all: $(NAME)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(CFLAGS) $(LINKER)

clean:
	$(RM) $(OBJS)

fclean:
	$(RM) $(NAME)

re: fclean all
