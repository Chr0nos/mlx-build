/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/31 13:23:35 by snicolet          #+#    #+#             */
/*   Updated: 2020/05/21 03:26:53 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "color_map.h"
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

void					zoom(struct s_mandel *mandel,
	const unsigned int x, const unsigned int y, const t_fract zoom)
{
	const t_fract	w = (mandel->x2 - mandel->x1) * mandel->zoom;
	const t_fract	h = (mandel->y2 - mandel->y2) * mandel->zoom;
	const t_fract	nw = (mandel->x2 - mandel->x1) * (mandel->zoom * zoom);
	const t_fract	nh = (mandel->y2 - mandel->y1) * (mandel->zoom * zoom);

	mandel->zoom *= zoom;
	mandel->offset_x -= ((t_fract)x / (t_fract)mandel->img->width) * (nw - w);
	mandel->offset_y -= ((t_fract)y / (t_fract)mandel->img->height) * (nh - h);
}

struct s_mandel			mandelbrot_init(struct s_image *img,
	unsigned int iterations)
{
	return (struct s_mandel) {
		.img = img,
		.max_iterations = iterations,
		.coef_x = (t_fract)img->width / (t_fract)(X2 - X1),
		.coef_y = (t_fract)img->height / (t_fract)(Y2 - Y1),
		.color_map = create_color_map(0x00bfff, 0x001015, COLOR_BLACK, iterations),
		.zoom = (t_fract)1.0f,
		.offset_x = (t_fract)0.0f,
		.offset_y = (t_fract)0.0f,
		.x1 = (t_fract)X1,
		.x2 = (t_fract)X2,
		.y1 = (t_fract)Y1,
		.y2 = (t_fract)Y2
	};
}

void					mandelbrot(struct s_mandel *mandel)
{
	if (!mandel->color_map)
		return ;
	image_fill_threaded(mandel->img, draw_mandelpix, mandel, 12);
}
