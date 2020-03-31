#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_mlx.h"
#include "mlx.h"


static void		putpx(struct s_image *img,
	const unsigned int x, const unsigned y, const unsigned int color)
{
	img->pixels[x + y * img->width] = color;
}

/*
** Setting the same color horizontaly can be significanlty optimized with a
** PROPER memset (don't write char by char if you use a ft_memset)
*/

static void		draw_box(struct s_image *img, const struct s_box *box,
	const unsigned int color)
{
	unsigned int	*pixels;
	int				n;

	n = box->h;
	while (n--)
	{
		pixels = &img->pixels[img->width * (box->y + n) + box->x];
		memset(pixels, color, box->w * sizeof(unsigned int));
	}
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
	unsigned int	old_kbd;

	mlx = userdata;
	old_kbd = mlx->keyboard;
	if ((keycode == KEY_ESCAPE) || (keycode == KEY_Q)) {
		mlx_destroy_image(mlx->ptr, mlx->window.image.ptr);
		mlx_destroy_window(mlx->ptr, mlx->window.ptr);
		free(mlx->ptr);
		exit(EXIT_SUCCESS);
	}
	else if (keycode == KEY_P)
		mlx->flags |= (DISPLAY_DOT | RENDER);
	if ((keycode >= 'a') && (keycode <= 'z'))
		mlx->keyboard |= (1u << (keycode - 'a'));\
	if (mlx->keyboard != old_kbd)
		mlx->flags |= RENDER;
	return (EXIT_SUCCESS);
}

static int		key_rlz_hook(int keycode, void *userdata)
{
	struct s_mlx	*mlx;
	unsigned int	old_kbd;

	mlx = userdata;
	old_kbd = mlx->keyboard;
	if (keycode == KEY_P) {
		mlx->flags ^= DISPLAY_DOT;
		mlx->flags |= RENDER;
	}
	if ((keycode >= 'a') && (keycode <= 'z'))
		mlx->keyboard &= ~(1u << (keycode - 'a'));
	if (mlx->keyboard != old_kbd)
		mlx->flags |= RENDER;
	return (EXIT_SUCCESS);
}

__attribute_pure__
static int		create_window(struct s_mlx *mlx, struct s_window *win)
{
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
	const struct s_box	box = (struct s_box) {42, 42, 42, 42};

	if (mlx->flags & RENDER) {
		draw_box(&mlx->window.image, &box,
			(mlx->keyboard) ? COLOR_WHITE : COLOR_BLACK);
		putpx(&mlx->window.image,
			mlx->window.width >> 1, mlx->window.height >> 1,
			(mlx->flags & DISPLAY_DOT) ? COLOR_RED : COLOR_BLACK);
		mlx_put_image_to_window(mlx->ptr, mlx->window.ptr,
			mlx->window.image.ptr, 0, 0);
		mlx->flags &= ~RENDER;
	}
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
	mlx.window.width = 1280;
	mlx.window.height = 720;
	mlx.flags = RENDER;
	if (create_window(&mlx, &mlx.window) != EXIT_SUCCESS) {
		puts("failed to create window");
		return (2);
	}
	mlx_string_put(mlx.ptr, mlx.window.ptr, 10, 10, COLOR_WHITE, "Please wait");
	mandelbrot(&mlx.window.image, 90);
	mlx_hook(mlx.window.ptr, HOOK_KEY_DOWN, 1, key_press_hook, &mlx);
	mlx_hook(mlx.window.ptr, HOOk_KEY_UP, 2, key_rlz_hook, &mlx);
	mlx_loop_hook(mlx.ptr, &display, &mlx);
	mlx_loop(mlx.ptr);
	return (EXIT_SUCCESS);
}
