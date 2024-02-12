CC := clang
#
# [XOPEN:600 POSIX:200112L] SUSv3 / UNIX 03 / POSIX.1-2001 / XPG6
# [XOPEN:700 POSIX:200809L] SUSv4 / UNIX 08 / POSIX.1-2008 / XPG7
#
PORTABILITY := -D_XOPEN_SOURCE=700 -D_POSIX_C_SOURCE=200809L
#
# CONSISTENCY
#
WARNINGS := #-Wno-long-long -Wall -pedantic -Werror
#
# INSPECTION
#
DEBUG := -g -ggdb
#
# STRUCTURE
#
CCFLAGS := $(PORTABILITY) -Iinc $(WARNINGS) $(DEBUG)

LDFLAGS := -lgit2

HEADERS := $(shell ls inc/*.h)

SOURCES := $(shell ls src/*.c)

gd: main.c $(HEADERS) $(SOURCES)
	$(CC) $(CCFLAGS) -o $@ $< $(SOURCES) $(LDFLAGS)
clean:
	$(RM) gd

install: gd
	sudo cp -p gd /usr/local/bin

uninstall: clean
	sudo rm /usr/local/bin/gd

