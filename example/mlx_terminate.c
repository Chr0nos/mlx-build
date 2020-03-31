#include <X11/Xlib.h>
#include <unistd.h>
#include "mlx_int.h"

void    mlx_terminate(void *mlxptr)
{
    struct s_xvar   *xvar;

    xvar = mlxptr;
    if (xvar->private_cmap)
        XFreeColormap(xvar->display, xvar->private_cmap);
    XCloseDisplay(xvar->display);
    free(xvar);
}
