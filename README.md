# NamuOS

## Building & Running
### Requirements
We need some basics for building with GCC and an assembler, as well as something to create images to run.
```sh
sudo apt install build-essential
sudo apt install git bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo libisl-dev
sudo apt install grub-common xorriso grub-pc-bin # ISOs and GRUB
```

We also want something that is able to run our OS.
```sh
sudo apt install qemu-system qemu-utils
```

Some info on these packages:
- [bison](https://www.gnu.org/software/bison/)
- [flex](https://www.gnu.org/software/flex/)
- [texinfo](https://www.gnu.org/software/texinfo/)
- [qemu](https://www.qemu.org/)

If documentation is to be generated,
```sh
sudo apt install doxygen graphviz
```


### C Cross Compiler
We need to be able to cross compile C. For this we build GCC for i686-elf.

First set up some environment variables, and a place to build binutils and the cross compiler
```sh
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
mkdir -p $HOME/opt/cross/bin
mkdir -p $HOME/src
```

Next download and install binutils
```sh
cd $HOME/src
git clone --depth 1 https://github.com/bminor/binutils-gdb.git ~/src/binutils-gdb
mkdir ~/src/binutils-gdb/build-binutils
cd ~/src/binutils-gdb/build-binutils
../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
```

And now gcc
```sh
git clone --depth 1 https://github.com/gcc-mirror/gcc.git ~/src/gcc
mkdir ~/src/gcc/build-gcc
cd ~/src/gcc/build-gcc
../configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
```

### Building with Make
Just run `make` to build the project. This will create an .iso `namuos.iso`.

### Running
You can run the OS with qemu using `make qemu`


## Resources
- [Medium - Writing a basic kernel](https://computers-art.medium.com/writing-a-basic-kernel-6479a495b713)
- [OSDev - Bare Bones with NASM](https://wiki.osdev.org/Bare_Bones_with_NASM)
- [OSDev - Bare Bones: Booting the Operating System](https://wiki.osdev.org/Bare_Bones#Booting_the_Operating_System)

## TODO
- Finish documenting libc
- Finish implementing libc
- Implement a way to launch OS in debug mode within VS Code
- Documentation for kernel side
- Handle other C locales other than default?
