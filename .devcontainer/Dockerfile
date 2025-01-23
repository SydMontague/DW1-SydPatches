FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

WORKDIR /tmp/build-mips-tools

RUN echo "WAS"

RUN apt-get update \
 && apt-get upgrade -y \
 && apt-get install libmpc-dev flex texinfo -y

ADD https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.xz gcc-14.2.0.tar.xz
ADD https://ftp.gnu.org/gnu/binutils/binutils-2.43.tar.xz binutils-2.43.tar.xz

RUN tar -xf gcc-14.2.0.tar.xz \
 && tar -xf binutils-2.43.tar.xz

WORKDIR /tmp/build-mips-tools/build-gcc

RUN ../gcc-14.2.0/configure --target=mips --prefix=/usr --program-prefix=mips- --disable-nls --disable-shared --disable-multilib --disable-threads --without-headers --enable-languages=c,c++ --with-gnu-as --with-gnu-ld \
 && make all-gcc -j8 \
 && make install-gcc

WORKDIR /tmp/build-mips-tools/build-binutils

RUN ../binutils-2.43/configure --prefix=/usr --target=mips --program-prefix=mips- --disable-nls --with-gnu-as --with-gnu-ld \
 && make -j8 \
 && make install

