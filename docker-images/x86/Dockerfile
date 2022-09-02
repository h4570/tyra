# syntax=docker/dockerfile:1
FROM alpine

# Install necessary packages
RUN apk add --no-cache wget make rsync \
    git g++ texinfo bison flex gettext \
    gmp-dev gcc binutils cmake patch \
    zlib-dev gsl-dev unzip libpng-dev

CMD ["/bin/bash"]

# TODO: Remove
# docker run -it --rm -v ${PWD}:/host h4570/tyra-x86 sh -c 'cd /host/engine && make EE_PREFIX='    