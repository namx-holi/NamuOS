ASM=i686-elf-gcc
ASM_FLAGS=-O0 -g -ffreestanding -Wall -Wextra

CC=i686-elf-gcc
CC_FLAGS=-std=gnu11 -O0 -g -ffreestanding -Wall -Wextra

AR=i686-elf-ar
AR_FLAGS=


BUILD_DIR=build
ISO_DIR=$(BUILD_DIR)/isodir
SYSROOT=$(BUILD_DIR)/sysroot
ISO_OBJ=namuos.iso


.phony: all build_iso install_headers build_project qemu clean docs


all: build_iso


build_iso: $(ISO_OBJ)


$(ISO_OBJ): install_headers build_project
	mkdir -p $(ISO_DIR)
	mkdir -p $(ISO_DIR)/boot
	mkdir -p $(ISO_DIR)/boot/grub

	cp $(SYSROOT)/boot/*.kernel $(ISO_DIR)/boot/.
	cp grub.cfg $(ISO_DIR)/boot/grub/.
	grub-mkrescue -o $(ISO_OBJ) $(ISO_DIR)


install_headers:
	mkdir -p $(SYSROOT)/usr/include
	cp -R --preserve=timestamps include/libc/. $(SYSROOT)/usr/include/.
	cp -R --preserve=timestamps include/kernel/. $(SYSROOT)/usr/include/.
# TODO: Include ~/opt/cross/lib/gcc/i686-elf/15.0.0/include/.


build_project:
	$(MAKE) -j4 -C src/libc build
	$(MAKE) -j4 -C src/kernel build

qemu:
	qemu-system-i386 -cdrom $(ISO_OBJ)
# NOTE: Can add -d int for debugging

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(ISO_OBJ)

docs:
	doxygen Doxyfile
