/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mlx.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/31 13:37:33 by snicolet          #+#    #+#             */
/*   Updated: 2020/03/31 13:39:04 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_MLX_H
# define TEST_MLX_H

# define HOOK_KEY_DOWN  	2
# define HOOK_KEY_UP	    3
# define HOOK_MOUSE_DOWN	4
# define HOOK_MOUSE_UP	    5
# define HOOK_MOUSE_MOVE	6
# define HOOK_EXPOSE		12
# define HOOK_CLOSE	    	17

# define KEY_ESCAPE	    	65307
# define KEY_P		    	112
# define KEY_Q			    113

# define DISPLAY_DOT		1u
# define RENDER             2u

# define COLOR_BLACK	    0x00000000
# define COLOR_RED	        0x00ff0000
# define COLOR_WHITE        0x00ffffff

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

struct				s_mlx {
	void			*ptr;
	struct s_window	window;
	unsigned int	keyboard;
	unsigned int	flags;
};

struct				s_box {
	int				x;
	int				y;
	int				w;
	int				h;
};

#endif
