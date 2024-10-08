# TODO: Pull arch from parent makefile
ARCH=arch_i386


ASM=i686-elf-gcc
ASM_FLAGS=-O2 -g -ffreestanding -Wall -Wextra

CC=i686-elf-gcc
CC_FLAGS=-std=gnu11 -O2 -g -ffreestanding -Wall -Wextra

AR=i686-elf-ar
AR_FLAGS=


BUILD_ROOT=../../build
BUILD_DIR=$(BUILD_ROOT)/kernel
SYSROOT=$(BUILD_ROOT)/sysroot
KERNEL=$(BUILD_ROOT)/namuos.kernel


# Boot related stuff
KERNEL_BOOT_START_OBJ=\
	$(BUILD_DIR)/$(ARCH)/crti.o \
	$(BUILD_DIR)/$(ARCH)/crtbegin.o \
	$(BUILD_DIR)/$(ARCH)/boot.o
KERNEL_BOOT_END_OBJ=\
	$(BUILD_DIR)/$(ARCH)/crtend.o \
	$(BUILD_DIR)/$(ARCH)/crtn.o

# Kernel source
KERNEL_C_SRC=$(wildcard *.c) $(wildcard **/*.c)
KERNEL_C_OBJ=$(patsubst %.c, $(BUILD_DIR)/%.o, $(KERNEL_C_SRC))
KERNEL_SRC_DIRS=$(wildcard **/)
KERNEL_OBJ_DIRS=$(patsubst %, $(BUILD_DIR)/%, $(KERNEL_SRC_DIRS))
LINKER=$(ARCH)/linker.ld


.phony: build clean

build: $(BUILD_DIR) $(KERNEL_OBJ_DIRS) $(KERNEL)

$(KERNEL): $(KERNEL_BOOT_START_OBJ) $(KERNEL_BOOT_END_OBJ) $(KERNEL_C_OBJ)
	$(ASM) $(ASM_FLAGS) --sysroot=$(SYSROOT) -isystem=/usr/include -T $(LINKER) -o $(KERNEL) $(KERNEL_BOOT_START_OBJ) $(KERNEL_C_OBJ) -nostdlib -lk -lgcc $(KERNEL_BOOT_END_OBJ)
	grub-file --is-x86-multiboot $(KERNEL)

	mkdir -p $(SYSROOT)/boot
	cp $(KERNEL) $(SYSROOT)/boot

$(BUILD_DIR)/$(ARCH)/crtbegin.o $(BUILD_DIR)/$(ARCH)/crtend.o: # Special
	OBJ=`$(ASM) $(ASM_FLAGS) --sysroot=$(SYSROOT) -isystem=/usr/include -print-file-name=$(@F)` && cp "$$OBJ" $@


$(BUILD_DIR)/%.o: %.c
	$(CC) $(CC_FLAGS) --sysroot=$(SYSROOT) -isystem=/usr/include -c $< -o $@ -D__is_libc -Iinclude -D__is_libk
	$(CC) $(CC_FLAGS) --sysroot=$(SYSROOT) -isystem=/usr/include -M -E -c $< -o $(basename $@).d -D__is_libc -Iinclude -D__is_libk

$(BUILD_DIR)/%.o: %.S
	$(ASM) $(ASM_FLAGS) --sysroot=$(SYSROOT) -isystem=/usr/include -c $< -o $@ -D__is_kernel -Iinclude
	$(ASM) $(ASM_FLAGS) --sysroot=$(SYSROOT) -isystem=/usr/include -M -E -c $< -o $(basename $@).d -D__is_kernel -Iinclude


# $(LIBK_OBJ): $(LIBC_OBJ)
# 	$(AR) $(AR_FLAGS) rcs $@ $(LIBC_OBJ)
# 	mkdir -p $(SYSROOT)/usr/lib
# 	cp $(LIBK_OBJ) $(SYSROOT)/usr/lib/.

# $(BUILD_DIR)/%.libk.o: %.c
# 	$(CC) $(CC_FLAGS) --sysroot=$(SYSROOT) -isystem=/usr/include -M -c $< -o $@ -D__is_libc -Iinclude -D__is_libk

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(KERNEL_OBJ_DIRS):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
