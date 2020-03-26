#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mlx.h"

#define HOOK_KEY_DOWN	2
#define HOOk_KEY_UP		3
#define HOOK_MOUSE_DOWN	4
#define HOOK_MOUSE_UP	5
#define HOOK_MOUSE_MOVE	6
#define HOOK_EXPOSE		12
#define HOOK_CLOSE		17

#define KEY_ESCAPE		65307
#define KEY_P			112
#define KEY_Q			113

#define DISPLAY_DOT		(1u << 0)

#define COLOR_BLACK	0x00000000
#define COLOR_RED	0x00ff0000

struct				s_image {
	void			*ptr;
	unsigned int	width;
	unsigned int	height;
	int				bpp;
	int				endian;
	unsigned int	*pixels;
};

struct				s_window {
	const char		*title;
	void			*ptr;
	unsigned int	width;
	unsigned int	height;
	struct s_image	image;
};

struct 				s_mlx {
	void			*ptr;
	struct s_window	window;
	unsigned int	keyboard;
	unsigned int	flags;
};

static void		putpx(struct s_image *img,
	const unsigned int x, const unsigned y, const unsigned int color)
{
	img->pixels[x + y * img->width] = color;
}

__attribute_pure__
static int		create_image(struct s_mlx *mlx,
	int w, int h, struct s_image *img)
{
	int		line_size;

	if ((w < 0) || (h < 0))
		return (EXIT_FAILURE);
	*img = (struct s_image) {
		.ptr = mlx_new_image(mlx->ptr, w, h),
		.width = w,
		.height = h,
		.bpp = 0,
		.endian = 0,
		.pixels = NULL
	};
	if (!img->ptr)
		return (EXIT_FAILURE);
	img->pixels = (void*)(size_t)mlx_get_data_addr(
		img->ptr, &img->bpp, &line_size, &img->endian);
	if (!img->pixels) {
		mlx_destroy_image(mlx->ptr, img->ptr);
		return (EXIT_FAILURE);
	}
	bzero(img->pixels, (img->bpp >> 3) * (img->width * img->height));
	return (EXIT_SUCCESS);
}

static int		key_press_hook(int keycode, void *userdata)
{
	struct s_mlx	*mlx;

	mlx = userdata;
	if ((keycode == KEY_ESCAPE) || (keycode == KEY_Q)) {
		mlx_destroy_image(mlx->ptr, mlx->window.image.ptr);
		mlx_destroy_window(mlx->ptr, mlx->window.ptr);
		free(mlx->ptr);
		exit(EXIT_SUCCESS);
	}
		mlx->flags |= DISPLAY_DOT;
	return (EXIT_SUCCESS);
}

static int		key_rlz_hook(int keycode, void *userdata) {
	struct s_mlx	*mlx;

	mlx = userdata;
	if (keycode == KEY_P)
		mlx->flags ^= DISPLAY_DOT;
	return (EXIT_SUCCESS);
}

__attribute_pure__
static int		create_window(struct s_mlx *mlx, struct s_window *win) {
	win->ptr = mlx_new_window(mlx->ptr, win->width, win->height,
		(void*)(size_t)win->title);
	if (!win->ptr)
		return (EXIT_FAILURE);
	if (create_image(mlx, win->width, win->height, &win->image)) {
		mlx_destroy_window(mlx->ptr, win->ptr);
		return (EXIT_FAILURE);
	}
	mlx_clear_window(mlx->ptr, win->ptr);
	return (EXIT_SUCCESS);
}

static int	display(struct s_mlx *mlx)
{
	putpx(&mlx->window.image,
		mlx->window.width >> 1, mlx->window.height >> 1,
		(mlx->flags & DISPLAY_DOT) ? COLOR_RED : COLOR_BLACK);
	mlx_put_image_to_window(mlx->ptr, mlx->window.ptr,
		mlx->window.image.ptr, 0, 0);
	return (EXIT_SUCCESS);
}

int			main(void)
{
	struct s_mlx	mlx;

	bzero(&mlx, sizeof(mlx));
	if (!(mlx.ptr = mlx_init())) {
		puts("failed to init mlx");
		return (1);
	}
	mlx.window.title = "Testing mlx window";
	mlx.window.width = 640;
	mlx.window.height = 480;
	if (create_window(&mlx, &mlx.window) != EXIT_SUCCESS) {
		puts("failed to create window");
		return (2);
	}
	mlx_hook(mlx.window.ptr, HOOK_KEY_DOWN, 1, key_press_hook, &mlx);
	mlx_hook(mlx.window.ptr, HOOk_KEY_UP, 2, key_rlz_hook, &mlx);
	mlx_loop_hook(mlx.ptr, &display, &mlx);
	mlx_loop(mlx.ptr);
	return (EXIT_SUCCESS);
}
