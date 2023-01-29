NAME := os.img

OBJ_DIR := obj
KERNEL_DIR := kernel
KERNEL := $(KERNEL_DIR)/kernel
IMPORTANT_ASM_SOURCES := kernel/boot/boot.asm kernel/boot/secondstage.asm
ASM_SOURCES := kernel/font.asm 
BINARIES := $(IMPORTANT_ASM_SOURCES:%.asm=%.o) $(KERNEL) $(ASM_SOURCES:%.asm=%.o)

KERNEL_MAKE := $(MAKE) -C $(KERNEL_DIR)

DISKIMG_DIR := tools/create-disk-image
DISKIMG_MAKE := $(MAKE) -C $(DISKIMG_DIR)
DISKIMG := $(DISKIMG_DIR)/diskimg

all: $(NAME)

$(NAME): $(BINARIES)
	./programs/build-all.sh all
	make -C $(DISKIMG_DIR) all
	$(DISKIMG) $(NAME) $(BINARIES) $(shell cat programs/programs)

%.o: %.asm
	mkdir -p $(@D)
	nasm -o $@ $< -Ikernel/boot

$(KERNEL):
	$(KERNEL_MAKE) all

clean:
	rm -f $(NAME) $(BINARIES)
	$(KERNEL_MAKE) clean
	$(DISKIMG_MAKE) clean
	./programs/build-all.sh clean
	rm -f programs/programs

re:
	./programs/build-all.sh clean
	$(KERNEL_MAKE) clean
	$(DISKIMG_MAKE) clean
	$(MAKE) clean all

run:
	qemu-system-i386 -drive file=$(NAME),format=raw,index=0 -serial stdio