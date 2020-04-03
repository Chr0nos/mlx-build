/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_terminate.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/31 13:33:37 by snicolet          #+#    #+#             */
/*   Updated: 2020/04/04 01:18:20 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <X11/Xlib.h>
#include <unistd.h>
#include "mlx_int.h"
#include "mlx_terminate.h"

void	mlx_terminate(void *mlxptr)
{
	struct s_xvar	*xvar;

	xvar = mlxptr;
	if (xvar->private_cmap)
		XFreeColormap(xvar->display, (Colormap)xvar->private_cmap);
	XCloseDisplay(xvar->display);
	free(xvar);
}
