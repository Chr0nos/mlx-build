/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_terminate.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/31 13:33:37 by snicolet          #+#    #+#             */
/*   Updated: 2020/03/31 13:34:44 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <X11/Xlib.h>
#include <unistd.h>
#include "mlx_int.h"

void	mlx_terminate(void *mlxptr)
{
	struct s_xvar	*xvar;

	xvar = mlxptr;
	if (xvar->private_cmap)
		XFreeColormap(xvar->display, xvar->private_cmap);
	XCloseDisplay(xvar->display);
	free(xvar);
}
