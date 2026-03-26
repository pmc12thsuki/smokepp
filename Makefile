NAME = smoke++

CC = gcc
CFLAGS = -O2 -Wall -Wextra
SRC = smokepp.c
HDR = smokepp.h

all: $(NAME)

$(NAME): $(SRC) $(HDR)
	$(CC) $(CFLAGS) -o '$(NAME)' $(SRC)

clean:
	rm -f '$(NAME)'

re: clean all

setup: $(NAME)
	@if grep -q 'smokepp' $$HOME/.zshrc 2>/dev/null; then \
		echo "PATH already configured in ~/.zshrc"; \
	else \
		echo 'export PATH="$$PATH:$(CURDIR)"' >> $$HOME/.zshrc; \
		echo "Added $(CURDIR) to PATH in ~/.zshrc"; \
		echo "Run: source ~/.zshrc"; \
	fi

.PHONY: all clean re setup
