/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/31 13:36:43 by snicolet          #+#    #+#             */
/*   Updated: 2020/05/20 22:10:45 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mlx.h"
#include "test_mlx.h"
#include "mlx_terminate.h"
#include "mandelbrot.h"

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

static int		key_press_hook(int keycode, void *userdata)
{
	struct s_mlx	*mlx;
	unsigned int	old_kbd;

	mlx = userdata;
	old_kbd = mlx->keyboard;
	if ((keycode == KEY_ESCAPE) || (keycode == KEY_Q)) {
		mlx_destroy_image(mlx->ptr, mlx->window.image.ptr);
		mlx_destroy_window(mlx->ptr, mlx->window.ptr);
		mlx_terminate(mlx->ptr);
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

static int	display(struct s_mlx *mlx)
{
	if (mlx->flags & RENDER) {
		mlx_put_image_to_window(mlx->ptr, mlx->window.ptr,
			mlx->window.image.ptr, 0, 0);
		mlx->flags &= ~RENDER;
	}
	return (EXIT_SUCCESS);
}

int			main(int ac, char **av)
{
	struct s_mlx	mlx;

	bzero(&mlx, sizeof(mlx));
	if (!(mlx.ptr = mlx_init())) {
		puts("failed to init mlx");
		return (EXIT_FAILURE);
	}
	mlx.window.title = "Testing mlx window";
	mlx.window.width = 1910;
	mlx.window.height = 1000;
	mlx.flags = RENDER;
	if (ac > 1)
		sscanf(av[1], "%5ux%5u", &mlx.window.width, &mlx.window.height);
	if ((!mlx.window.height) || (!mlx.window.width))
		return (EXIT_FAILURE);
	if (create_window(&mlx, &mlx.window) != EXIT_SUCCESS) {
		puts("failed to create window");
		return (EXIT_FAILURE);
	}
	mlx_string_put(mlx.ptr, mlx.window.ptr, 10, 10, COLOR_WHITE, "Please wait");
	mandelbrot(&mlx.window.image, 90);
	mlx_hook(mlx.window.ptr, HOOK_KEY_DOWN, 1, key_press_hook, &mlx);
	mlx_hook(mlx.window.ptr, HOOK_KEY_UP, 2, key_rlz_hook, &mlx);
	mlx_loop_hook(mlx.ptr, &display, &mlx);
	mlx_loop(mlx.ptr);
	return (EXIT_SUCCESS);
}
