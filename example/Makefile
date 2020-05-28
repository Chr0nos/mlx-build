NAME=a.out
MLX_PATH=../minilibx
LINKER=-L$(MLX_PATH) -lXext -lX11 -lmlx -lpthread
CC=clang
CFLAGS=-Wall -Werror -Wextra -Weverything -Wno-padded -Ofast -Wno-strict-prototypes
OBJS=test_mlx.o mandelbrot.o mlx_terminate.o fill_image.o color_map.o

ifeq ($(CC), gcc)
	CFLAGS=-Wall -Werror -Wextra -O3
endif

all: $(NAME)

%.o: %.c
	$(CC) -c $< -I $(MLX_PATH) $(CFLAGS) -o $@

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(CFLAGS) $(LINKER)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
