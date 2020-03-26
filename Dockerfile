FROM archlinux
RUN pacman -Sy
RUN pacman -S --noconfirm make clang zsh vim gdb xorg-server extra/libbsd git wget patch

# install the MLX
WORKDIR /root
COPY mlx_fix_linux.patch ./
RUN wget https://projects.intra.42.fr/uploads/document/document/7/sources.tgz
RUN tar -xvf sources.tgz
RUN cd minilibx && patch -p1 < ../mlx_fix_linux.patch && make
RUN cp -v minilibx/libmlx.a /usr/lib && cp -v minilibx/mlx.h /usr/include && mkdir project

WORKDIR /root/project
CMD ["/usr/bin/zsh"]
