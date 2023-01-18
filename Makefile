NAME := os.img

OBJ_DIR := obj
KERNEL_DIR := kernel
KERNEL := $(KERNEL_DIR)/kernel
IMPORTANT_ASM_SOURCES := kernel/boot/boot.asm kernel/filetable.asm kernel/boot/secondstage.asm
BINARIES := $(IMPORTANT_ASM_SOURCES:%.asm=%.o) $(KERNEL)

KERNEL_MAKE := $(MAKE) -C $(KERNEL_DIR)

all: $(NAME)

$(NAME): $(BINARIES)
	cat $(BINARIES) > $(NAME)

%.o: %.asm
	mkdir -p $(@D)
	nasm -o $@ $< -Ikernel/boot

$(KERNEL):
	$(KERNEL_MAKE) all

clean:
	rm -f $(NAME) $(BINARIES)
	$(KERNEL_MAKE) clean

re: 
	$(KERNEL_MAKE) re
	$(MAKE) clean all

run:
	qemu-system-i386 -drive file=$(NAME),format=raw,index=0 -serial stdio