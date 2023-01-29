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

PROGRAMS_DIR := programs
PROGRAMS_SCRIPT := $(PROGRAMS_DIR)/build.sh
PROGRAMS_LIST_FILE := $(PROGRAMS_DIR)/programs

all: $(NAME)

$(NAME): $(BINARIES)
	$(PROGRAMS_SCRIPT) all
	$(DISKIMG_MAKE) all
	$(DISKIMG) $(NAME) $(BINARIES) $$(cat ${PROGRAMS_LIST_FILE})

%.o: %.asm
	mkdir -p $(@D)
	nasm -o $@ $< -Ikernel/boot

$(KERNEL):
	$(KERNEL_MAKE) all

clean:
	rm -f $(NAME) $(BINARIES)
	$(KERNEL_MAKE) clean
	$(DISKIMG_MAKE) clean
	$(PROGRAMS_SCRIPT) clean
	rm -f $(PROGRAMS_LIST_FILE)

re:
	$(PROGRAMS_SCRIPT) re
	$(KERNEL_MAKE) clean
	$(DISKIMG_MAKE) clean
	$(MAKE) clean all

run:
	qemu-system-i386 -drive file=$(NAME),format=raw,index=0 -serial stdio