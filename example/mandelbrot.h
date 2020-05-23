/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/31 13:27:36 by snicolet          #+#    #+#             */
/*   Updated: 2020/05/21 02:16:25 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANDELBROT_H
# define MANDELBROT_H
# include "test_mlx.h"

typedef float		t_fract;

struct				s_mandel {
	struct s_image	*img;
	unsigned int	*color_map;
	unsigned int	max_iterations;
	t_fract			x1;
	t_fract			x2;
	t_fract			y1;
	t_fract			y2;
	t_fract			coef_x;
	t_fract			coef_y;
	t_fract			zoom;
	t_fract			offset_x;
	t_fract			offset_y;
};

struct s_mandel		mandelbrot_init(struct s_image *img,
	unsigned int iterations);
void				mandelbrot(struct s_image *img, unsigned int iterations);
void				zoom(struct s_mandel *mandel,
	const unsigned int x, const unsigned int y, const t_fract zoom);

#endif
