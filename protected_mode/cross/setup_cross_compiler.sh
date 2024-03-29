#!/bin/bash
export PREFIX="/home/ajabbar/code/A-DOS/protected_mode/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"


mkdir build-binutils
cd build-binutils
../binutils-2.40/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

which --$TARGET-as || echo $TARGET-as is not in the PATH
cd ..
mkdir build-gcc
cd build-gcc
../gcc-12.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ -without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc


