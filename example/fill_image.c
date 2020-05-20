/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/20 18:52:40 by snicolet          #+#    #+#             */
/*   Updated: 2020/05/20 18:58:20 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test_mlx.h"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/*
** At this point, i absolutly don't care about the norm, KISS
*/

struct				s_thread {
	unsigned int	id;
	unsigned int	max_id;
	pthread_t		thread;
	struct s_image	*img;
	void			(*func)(unsigned int, unsigned int, void *);
	void			*userdata;
};

static void			*image_fill_routine(void *context)
{
	struct s_thread		*thread;
	unsigned int		slice_size;
	unsigned int		min_x;
	unsigned int		x;
	unsigned int		y;

	thread = context;
	slice_size = thread->img->width / thread->max_id;
	min_x = slice_size * thread->id;
	x = slice_size * (thread->id + 1);
	while (x-- > min_x)
	{
		y = thread->img->height;
		while (y--)
			thread->func(x, y, thread->userdata);
	}
	return (NULL);
}

/*
** multi threaded implementation to fill an image using pointer on function.
*/

int				image_fill_threaded(
	struct s_image *img,
	void (*func)(unsigned int, unsigned int, void *),
	void *userdata,
	const unsigned int threads_count)
{
	struct s_thread	*threads;
	unsigned int	i;

	threads = malloc(sizeof(struct s_thread) * threads_count);
	if (!threads)
		return (EXIT_FAILURE);
	i = 0;
	while (i < threads_count)
	{
		threads[i].id = i;
		threads[i].max_id = threads_count;
		threads[i].func = func;
		threads[i].userdata = userdata;
		threads[i].img = img;
		pthread_create(&threads[i].thread, NULL, image_fill_routine,
			&threads[i]);
		i++;
	}
	i = 0;
	while (i < threads_count)
		pthread_join(threads[i++].thread, NULL);
	free(threads);
	return (EXIT_SUCCESS);
}

/*
** Non multi threaded implementation to fill an image using pointer on function.
*/

void			image_fill(struct s_image *img,
	void (*func)(unsigned int, unsigned int, void *),
	void *userdata)
{
	unsigned int		x;
	unsigned int		y;

	x = img->width;
	while (x--)
	{
		y = img->height;
		while (y--)
			func(x, y, userdata);
	}
}
