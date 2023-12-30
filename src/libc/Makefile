ASM=i686-elf-gcc
ASM_FLAGS=-O2 -g -ffreestanding -Wall -Wextra

CC=i686-elf-gcc
CC_FLAGS=-std=gnu11 -O2 -g -ffreestanding -Wall -Wextra

AR=i686-elf-ar
AR_FLAGS=


BUILD_ROOT=../../build
BUILD_DIR=$(BUILD_ROOT)/libc
SYSROOT=$(BUILD_ROOT)/sysroot
LIBK_OBJ=$(BUILD_ROOT)/libk.a


LIBC_SRC=$(wildcard **/*.c)
LIBC_OBJ=$(patsubst %.c, $(BUILD_DIR)/%.libk.o, $(LIBC_SRC))
LIBC_SRC_DIRS=$(wildcard **/)
LIBC_OBJ_DIRS=$(patsubst %, $(BUILD_DIR)/%, $(LIBC_SRC_DIRS))


.phony: build clean

build: $(BUILD_DIR) $(LIBC_OBJ_DIRS) $(LIBK_OBJ)

$(LIBK_OBJ): $(LIBC_OBJ)
	$(AR) $(AR_FLAGS) rcs $@ $(LIBC_OBJ)
	mkdir -p $(SYSROOT)/usr/lib
	cp $(LIBK_OBJ) $(SYSROOT)/usr/lib/.

$(BUILD_DIR)/%.libk.o: %.c
	$(CC) $(CC_FLAGS) --sysroot=$(SYSROOT) -isystem=/usr/include -c $< -o $@ -D__is_libc -Iinclude -D__is_libk
	$(CC) $(CC_FLAGS) --sysroot=$(SYSROOT) -isystem=/usr/include -M -E -c $< -o $(basename $@).d -D__is_libc -Iinclude -D__is_libk

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIBC_OBJ_DIRS):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
