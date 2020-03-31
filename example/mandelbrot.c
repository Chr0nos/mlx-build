/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/31 13:23:35 by snicolet          #+#    #+#             */
/*   Updated: 2020/03/31 13:31:17 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_mlx.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include "mandelbrot.h"

#define RE            0
#define IM            1
#define RE2           2
#define IM2           3
#define CR            4
#define CI            5

#define X1            -2.1
#define X2            0.6
#define Y1            -1.2
#define Y2            1.2

unsigned int			color_lerp(unsigned int start, unsigned int end,
	float pc)
{
	float				r;
	float				g;
	float				b;

	if (pc <= 0.0f)
		return (start);
	else if (pc >= 1.0f)
		return (end);
	r = (float)(start & 0xff0000) * (1.0f - pc) + (float)(end & 0xff0000) * pc;
	g = (float)(start & 0x00ff00) * (1.0f - pc) + (float)(end & 0x00ff00) * pc;
	b = (float)(start & 0x0000ff) * (1.0f - pc) + (float)(end & 0x0000ff) * pc;
	return ((((unsigned int)r) & 0xff0000) |
		(((unsigned int)g) & 0x00ff00) |
		(((unsigned int)b) & 0x0000ff));
}

static void				image_fill(struct s_image *img,
	void (*func)(int, int, void *),
	void *userdata)
{
	int					x;
	int					y;

	x = img->width;
	while (x--)
	{
		y = img->height;
		while (y--)
			func(x, y, userdata);
	}
}

static void				draw_mandelpix(const int x, const int y, void *userdata)
{
	struct s_mandel		*mandel;
	unsigned int		i;
	register float		z[6];

	mandel = userdata;
	z[RE] = 0.0;
	z[IM] = 0.0;
	z[CR] = (float)x / mandel->zoom_x + X1;
	z[CI] = (float)y / mandel->zoom_y + Y1;
	i = 0;
	while (i < mandel->max_iterations)
	{
		z[RE2] = pow(z[RE], 2);
		z[IM2] = pow(z[IM], 2);
		if (z[RE2] + z[IM2] > 4)
			break ;
		z[IM] = 2 * z[IM] * z[RE] + z[CI];
		z[RE] = z[RE2] - z[IM2] + z[CR];
		i++;
	}
	mandel->img->pixels[x + y * mandel->img->width] = mandel->color_map[i];
}

static unsigned int		*create_color_map(unsigned int size)
{
	unsigned int		*map;
	unsigned int		i;
	const float			size_f = (float)size;

	map = malloc(sizeof(unsigned int) * (size + 1));
	if (!map)
		return (NULL);
	i = 0;
	while (i < size)
	{
		map[i] = color_lerp(0x2070ff, COLOR_BLACK, (1.0 - (float)i / size_f));
		i++;
	}
	map[size] = COLOR_BLACK;
	return (map);
}

void					mandelbrot(struct s_image *img, unsigned int iterations)
{
	struct s_mandel		mandel;

	mandel = (struct s_mandel) {
		.img = img,
		.max_iterations = iterations,
		.zoom_x = img->width / (X2 - X1),
		.zoom_y = img->height / (Y2 - Y1),
		.color_map = create_color_map(iterations)
	};
	if (!mandel.color_map)
		return ;
	image_fill(img, draw_mandelpix, &mandel);
	free(mandel.color_map);
}
