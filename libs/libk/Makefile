CC = gcc
CFLAGS = -c -m32 -march=i686 -std=gnu99 -Wall -Wextra -fno-stack-protector -Iinclude

override CFILES := $(shell find -L * -type f -name '*.c')
override OBJ := $(addprefix obj/,$(CFILES:.c=.c.o))
override HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d))

.PHONY: all
all: libk.a

libk.a: Makefile $(OBJ)
	mkdir -p "$$(dirname $@)"
	ar rcs $@ $(OBJ)

-include $(HEADER_DEPS)

obj/%.c.o: %.c Makefile
	mkdir -p "$$(dirname $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf obj libk.a