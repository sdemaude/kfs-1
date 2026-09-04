NAME := isodir/boot/myos

CROSS ?= x86_64-elf-
CC    := $(CROSS)gcc
LD    := $(CROSS)ld
AS    := nasm

OBJ_DIR := build
SRC_DIR := src
LINKER  := $(SRC_DIR)/linker.ld

C_SRCS   := $(shell find $(SRC_DIR) -type f -name '*.c')
ASM_SRCS := $(shell find $(SRC_DIR) -type f \( -name '*.s' -o -name '*.asm' \))
C_OBJS   := $(C_SRCS:%.c=$(OBJ_DIR)/%.o)
ASM_OBJS := $(patsubst %.s,$(OBJ_DIR)/%.o,$(filter %.s,$(ASM_SRCS))) \
            $(patsubst %.asm,$(OBJ_DIR)/%.o,$(filter %.asm,$(ASM_SRCS)))
OBJS     := $(ASM_OBJS) $(C_OBJS)

CPPFLAGS :=
CFLAGS   := -m32 -std=gnu11 -ffreestanding -fno-builtin \
            -fno-stack-protector -fno-pie -nostdlib -nodefaultlibs \
            -Wall -Wextra -Werror
ASFLAGS  := -f elf32
LDFLAGS  := -m elf_i386 -T $(LINKER) -nostdlib

.DEFAULT_GOAL := all

.PHONY: all clean fclean re check-tools

all: check-tools $(NAME)

check-tools:
	@command -v $(CC) >/dev/null 2>&1 || { \
		echo "error: $(CC) is required (install an i686-elf cross-compiler)" >&2; \
		exit 1; \
	}
	@command -v $(LD) >/dev/null 2>&1 || { \
		echo "error: $(LD) is required (install i686-elf binutils)" >&2; \
		exit 1; \
	}
	@command -v $(AS) >/dev/null 2>&1 || { \
		echo "error: $(AS) is required" >&2; \
		exit 1; \
	}

$(NAME): $(OBJS) $(LINKER)
	@mkdir -p $(@D)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.s
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@

$(OBJ_DIR)/%.o: %.asm
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
