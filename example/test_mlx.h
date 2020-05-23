/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mlx.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/31 13:37:33 by snicolet          #+#    #+#             */
/*   Updated: 2020/05/21 03:34:05 by snicolet         ###   ########.fr       */
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

# define KEY_ARROW_RIGHT	65363
# define KEY_ARROW_LEFT		65361
# define KEY_ARROW_UP		65362
# define KEY_ARROW_DOWN		65364
# define KEY_PAGEUP			65365
# define KEY_PAGEDOWN		65366
# define KEY_ESCAPE	    	65307
# define KEY_P		    	112
# define KEY_Q			    113

# define MOUSE_CLICK_LEFT	1
# define MOUSE_CLICK_MIDDLE	2
# define MOUSE_CLICK_RIGHT	3
# define MOUSE_WHEEL_UP		5
# define MOUSE_WHEEL_DOWN	6

# define COMPUTE			1u
# define FLUSH	            2u
# define MOVE_UP			4u
# define MOVE_DOWN			8u
# define MOVE_LEFT			16u
# define MOVE_RIGHT			32u


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

int					image_fill_threaded(
	struct s_image *img,
	void (*func)(unsigned int, unsigned int, void *),
	void *userdata,
	const unsigned int threads_count);

void				image_fill(struct s_image *img,
	void (*func)(unsigned int, unsigned int, void *),
	void *userdata);

#endif
