# syntax=docker/dockerfile:1
FROM alpine:3.16.2

# Install necessary packages
RUN apk add --no-cache wget make rsync \
    git g++ texinfo bison flex gettext \
    gmp-dev gcc binutils cmake patch \
    zlib-dev coreutils gsl-dev unzip libpng-dev

CMD ["/bin/bash"]
