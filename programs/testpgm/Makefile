NAME := testpgm

CC := gcc
CFLAGS := -m32 -Wall -Wextra -Werror -ffreestanding -fno-builtin -nostdinc -fno-pic -I../../include
LDFLAGS := --oformat binary -melf_i386 -z muldefs

SOURCES := main.c
OBJECTS := $(SOURCES:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	ld -T../program.ld $(OBJECTS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(NAME) $(OBJECTS)

re: clean all