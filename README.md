# What is it ?
This ia a Docker image with the mlx pre-installed, so you can easly
compile and run your graphical project based on the minilibx from 42

It also have clang installed

# Build the image
Just run
```shell
docker build -t mlx .
```

# Run the image
```shell
# lazy alias
drg='docker run -it --rm --env="DISPLAY" --volume="$HOME/.Xauthority:/root/.Xauthority:rw" --net=host'

# run the image with the example code
drg -v `pwd`/example:/root/project mlx
make
```

then you can run the a.out file either from the container or the host machine directly
that's all it takes, that image require a linux with a xorg server running.
