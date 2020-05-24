/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/31 13:36:43 by snicolet          #+#    #+#             */
/*   Updated: 2020/05/21 11:49:58 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mlx.h"
#include "test_mlx.h"
#include "color_map.h"
#include "mandelbrot.h"
#include "mlx_terminate.h"

__attribute_pure__
static int		create_image(struct s_mlx *mlx,
	const unsigned int w, const unsigned int h, struct s_image *img)
{
	int		line_size;

	if ((!w) || (!h))
		return (EXIT_FAILURE);
	*img = (struct s_image) {
		.ptr = mlx_new_image(mlx->ptr, (int)w, (int)h),
		.width = w,
		.height = h,
		.bpp = 0,
		.endian = 0,
		.pixels = NULL
	};
	if (!img->ptr)
		return (EXIT_FAILURE);
	img->pixels = (void*)mlx_get_data_addr(
		img->ptr, &img->bpp, &line_size, &img->endian);
	if (!img->pixels) {
		mlx_destroy_image(mlx->ptr, img->ptr);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static size_t	get_key_related_flag(const int scancode)
{
	size_t		n;

	n = sizeof(g_key_binds) / sizeof(struct s_binding);
	while (n--)
	{
		if (g_key_binds[n].scancode == scancode)
			return g_key_binds[n].flag;
	}
	printf("unknow key code %d\n", scancode);
	return (0);
}

static int		key_press_hook(int keycode, void *userdata)
{
	struct s_mlx	*mlx;

	mlx = userdata;
	if ((keycode == KEY_ESCAPE) || (keycode == KEY_Q)) {
		free(((struct s_mandel*)mlx->userdata)->color_map);
		mlx_destroy_image(mlx->ptr, mlx->window.image.ptr);
		mlx_destroy_window(mlx->ptr, mlx->window.ptr);
		mlx_terminate(mlx->ptr);
		exit(EXIT_SUCCESS);
	}
	mlx->flags |= get_key_related_flag(keycode);
	return (EXIT_SUCCESS);
}

static int		key_rlz_hook(int keycode, void *userdata)
{
	struct s_mlx	*mlx;

	mlx = userdata;
	mlx->flags &= ~get_key_related_flag(keycode);
	return (EXIT_SUCCESS);
}

static int		mouse_click(int button, int x, int y, void *userdata)
{
	struct s_mlx	*mlx;

	mlx = userdata;
	printf("button: %d at %dx%d userptr: %p\n", button, x, y, userdata);
	if ((button == MOUSE_CLICK_LEFT) && (x >= 0) && (y >= 0))
	{
		zoom(mlx->userdata, (unsigned int)x, (unsigned int)y, (t_fract)0.95);
		mlx->flags |= COMPUTE;
	}
	return (EXIT_SUCCESS);
}

__attribute_pure__
static int		create_window(struct s_mlx *mlx, struct s_window *win)
{
	win->ptr = mlx_new_window(mlx->ptr, (int)win->width, (int)win->height,
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

static void	apply_move(size_t flags, struct s_mandel *mandel, t_fract speed)
{
	unsigned int	iter_step;

	if (flags & RESET)
	{
		free(mandel->color_map);
		*mandel = mandelbrot_init(mandel->img, 90);
	}
	if (flags & MOVE_RIGHT)
		mandel->offset_x += speed * mandel->zoom;
	if (flags & MOVE_LEFT)
		mandel->offset_x -= speed * mandel->zoom;
	if (flags & MOVE_UP)
		mandel->offset_y -= speed * mandel->zoom;
	if (flags & MOVE_DOWN)
		mandel->offset_y += speed * mandel->zoom;
	if (flags & ZOOM_IN)
		mandel->zoom /= (t_fract)1.02;
	if (flags & ZOOM_OUT)
		mandel->zoom += (t_fract)1.02;

	if (flags & (ITER_LESS | ITER_MORE))
	{
		iter_step = ITER_STEP;
		free(mandel->color_map);
		if (flags & ITER_MORE)
			mandel->max_iterations += iter_step;
		if ((flags & ITER_LESS) && (mandel->max_iterations > iter_step))
			mandel->max_iterations -= iter_step;
		mandel->color_map = create_color_map(0x00bfff, 0x001015, COLOR_BLACK,
			mandel->max_iterations);
	}
}

static int	display(struct s_mlx *mlx)
{
	if (mlx->flags & (MOVE_LEFT | MOVE_DOWN | MOVE_RIGHT | MOVE_UP | \
		ZOOM_IN | ZOOM_OUT | RESET | ITER_LESS | ITER_MORE))
	{
		apply_move(mlx->flags, mlx->userdata, (t_fract)0.04);
		mlx->flags |= COMPUTE | FLUSH;
	}
	if (mlx->flags & COMPUTE)
	{
		mandelbrot(mlx->userdata);
		mlx->flags &= ~COMPUTE;
		mlx->flags |= FLUSH;
	}
	if (mlx->flags & FLUSH)
	{
		mlx_put_image_to_window(mlx->ptr, mlx->window.ptr,
			mlx->window.image.ptr, 0, 0);
		mlx->flags &= ~FLUSH;
	}
	else
		usleep(60);
	return (EXIT_SUCCESS);
}

int			main(int ac, char **av)
{
	struct s_mlx	mlx;
	struct s_mandel	mandel;

	bzero(&mlx, sizeof(mlx));
	if (!(mlx.ptr = mlx_init())) {
		puts("failed to init mlx");
		return (EXIT_FAILURE);
	}
	mlx.window.title = "Testing mlx window";
	mlx.window.width = 1910;
	mlx.window.height = 1000;
	mlx.flags = COMPUTE;
	if (ac > 1)
		sscanf(av[1], "%5ux%5u", &mlx.window.width, &mlx.window.height);
	if ((!mlx.window.height) || (!mlx.window.width))
		return (EXIT_FAILURE);
	if (create_window(&mlx, &mlx.window) != EXIT_SUCCESS) {
		puts("failed to create window");
		return (EXIT_FAILURE);
	}
	mandel = mandelbrot_init(&mlx.window.image, 90);
	mlx.userdata = &mandel;
	mlx_string_put(mlx.ptr, mlx.window.ptr, 10, 10, COLOR_WHITE, "Please wait");
	mlx_hook(mlx.window.ptr, HOOK_KEY_DOWN, 1, &key_press_hook, &mlx);
	mlx_hook(mlx.window.ptr, HOOK_KEY_UP, 2, &key_rlz_hook, &mlx);
	mlx_hook(mlx.window.ptr, HOOK_MOUSE_DOWN, 4, &mouse_click, &mlx);
	mlx_loop_hook(mlx.ptr, &display, &mlx);
	mlx_loop(mlx.ptr);
	return (EXIT_SUCCESS);
}
