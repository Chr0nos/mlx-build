/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_mlx.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/31 13:37:33 by snicolet          #+#    #+#             */
/*   Updated: 2020/05/26 20:17:34 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_MLX_H
# define TEST_MLX_H
# include <stdlib.h>

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
# define KEY_HOME			65360
# define KEY_END			65367
# define KEY_ESCAPE	    	65307
# define KEY_P		    	112
# define KEY_Q			    113
# define KEY_R				114

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
# define ZOOM_IN			64u
# define ZOOM_OUT			128u
# define ITER_MORE			256u
# define ITER_LESS			512u
# define RESET				1024u

# define ITER_STEP			5

# define COLOR_BLACK	    0x00000000
# define COLOR_RED	        0x00ff0000
# define COLOR_WHITE        0x00ffffff

struct				s_binding {
	int				scancode;
	size_t			flag;
};

static const struct s_binding	g_key_binds[] = {
	(struct s_binding){KEY_ARROW_LEFT, MOVE_LEFT},
	(struct s_binding){KEY_ARROW_RIGHT, MOVE_RIGHT},
	(struct s_binding){KEY_ARROW_UP, MOVE_UP},
	(struct s_binding){KEY_ARROW_DOWN, MOVE_DOWN},
	(struct s_binding){KEY_P, FLUSH},
	(struct s_binding){KEY_PAGEDOWN, ZOOM_OUT},
	(struct s_binding){KEY_PAGEUP, ZOOM_IN},
	(struct s_binding){KEY_HOME, ITER_MORE},
	(struct s_binding){KEY_END, ITER_LESS},
	(struct s_binding){KEY_R, RESET}
};

struct				s_image {
	void			*ptr;
	unsigned int	width;
	unsigned int	height;
	int				bpp;
	int				endian;
	unsigned int	*pixels;
};

/*
** Used for dual buffer rendering
*/

typedef struct		s_dual_buffers {
	struct s_image	buffers[2];
	unsigned int	current_index;
}					t_buffpair;

struct				s_window {
	const char		*title;
	void			*ptr;
	unsigned int	width;
	unsigned int	height;
	t_buffpair		images;
};

struct				s_mlx {
	void			*ptr;
	struct s_window	window;
	unsigned int	keyboard;
	unsigned int	flags;
	void			*userdata;
};

int					image_fill_threaded(
	struct s_image *img,
	void (*func)(unsigned int, unsigned int, void *),
	void *userdata,
	const unsigned int threads_count);

void				image_fill(struct s_image *img,
	void (*func)(unsigned int, unsigned int, void *),
	void *userdata);
void				*swap_buffer(struct s_dual_buffers *buffers);

#endif
