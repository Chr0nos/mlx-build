/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/31 13:27:36 by snicolet          #+#    #+#             */
/*   Updated: 2020/03/31 13:40:48 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANDELBROT_H
# define MANDELBROT_H

struct				s_mandel {
	struct s_image	*img;
	unsigned int	max_iterations;
	float			zoom_x;
	float			zoom_y;
	unsigned int	*color_map;
};

void				mandelbrot(struct s_image *img, unsigned int iterations);

#endif
