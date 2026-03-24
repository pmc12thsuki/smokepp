NAME = smoke++

CC = gcc
CFLAGS = -O2 -Wall -Wextra
LDFLAGS = -lncurses

SRC = smokepp.c
HDR = smokepp.h

all: $(NAME)

$(NAME): $(SRC) $(HDR)
	$(CC) $(CFLAGS) -o '$(NAME)' $(SRC) $(LDFLAGS)

clean:
	rm -f '$(NAME)'

re: clean all

install: $(NAME)
	install -d /usr/local/bin
	install -m 755 '$(NAME)' /usr/local/bin/'$(NAME)'

uninstall:
	rm -f /usr/local/bin/'$(NAME)'

.PHONY: all clean re install uninstall
