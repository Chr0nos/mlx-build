/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/31 13:23:35 by snicolet          #+#    #+#             */
/*   Updated: 2020/05/21 02:31:19 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_mlx.h"
#include <stdlib.h>
#include <unistd.h>
#include "mandelbrot.h"

#define RE            0
#define IM            1
#define RE2           2
#define IM2           3
#define CR            4
#define CI            5

#define X1            -2.1f
#define X2            0.6f
#define Y1            -1.2f
#define Y2            1.2f

static unsigned int		color_lerp(unsigned int start, unsigned int end,
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

static void				draw_mandelpix(const unsigned int x,
	const unsigned int y, void *userdata)
{
	struct s_mandel		*mandel;
	unsigned int		i;
	register t_fract	z[6];

	mandel = userdata;
	z[RE] = 0.0;
	z[IM] = 0.0;
	z[CR] = ((t_fract)x / mandel->coef_x + mandel->x1) * \
		mandel->zoom + mandel->offset_x;
	z[CI] = ((t_fract)y / mandel->coef_y + mandel->y1) * \
		mandel->zoom + mandel->offset_y;
	i = 0;
	while (i < mandel->max_iterations)
	{
		z[RE2] = z[RE] * z[RE];
		z[IM2] = z[IM] * z[IM];
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
		map[i] = color_lerp(0x00bfff, 0x101015, (1.0f - (float)i / size_f));
		i++;
	}
	map[size] = COLOR_BLACK;
	return (map);
}


void					zoom(struct s_mandel *mandel,
	const unsigned int x, const unsigned int y, const t_fract zoom)
{
	const t_fract	w = (mandel->x2 - mandel->x1) * mandel->zoom;
	const t_fract	h = (mandel->y2 - mandel->y2) * mandel->zoom;
	const t_fract	nw = (mandel->x2 - mandel->x1) * (mandel->zoom * zoom);
	const t_fract	nh = (mandel->y2 - mandel->y1) * (mandel->zoom * zoom);

	mandel->zoom *= zoom;
	mandel->offset_x -= ((t_fract)x / mandel->img->width) * (nw - w);
	mandel->offset_y -= ((t_fract)y / mandel->img->height) * (nh - h);
}

void					mandelbrot(struct s_image *img, unsigned int iterations)
{
	struct s_mandel		mandel;

	mandel = (struct s_mandel) {
		.img = img,
		.max_iterations = iterations,
		.coef_x = (t_fract)img->width / (t_fract)(X2 - X1),
		.coef_y = (t_fract)img->height / (t_fract)(Y2 - Y1),
		.color_map = create_color_map(iterations),
		.zoom = (t_fract)1.0f,
		.offset_x = (t_fract)0.0f,
		.offset_y = (t_fract)0.0f,
		.x1 = (t_fract)X1,
		.x2 = (t_fract)X2,
		.y1 = (t_fract)Y1,
		.y2 = (t_fract)Y2
	};
	if (!mandel.color_map)
		return ;
	image_fill_threaded(img, draw_mandelpix, &mandel, 8);
	free(mandel.color_map);
}
