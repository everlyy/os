NAME := kernel

SOURCES := kernel.c $(shell find include/ -type f -name "*.c")
OBJECTS := $(SOURCES:%.c=%.o)

CC := gcc
CFLAGS := -m32 -Wall -Wextra -Werror -ffreestanding -fno-builtin -nostdinc -fno-pic -I../include -Iinclude -O3
LDFLAGS := --oformat binary -melf_i386 -z muldefs

all: $(NAME)

$(NAME): $(OBJECTS)
	ld -T$(NAME).ld $(OBJECTS) $(LDFLAGS) -o $(NAME)
	dd if=/dev/zero of=$(NAME) bs=1 count=1 seek=8191 status=none

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(NAME) $(OBJECTS)

re: clean all