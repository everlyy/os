NAME := diskimg

CC := gcc
CFLAGS := -Wall -Wextra -Werror

SOURCES := main.c
OBJECTS := $(SOURCES:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) -o $(NAME) $(OBJECTS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(NAME) $(OBJECTS)

re: clean all