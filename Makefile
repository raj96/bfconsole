PROJECT=bfconsole

CC=clang
CFLAGS=-O3 -Wall -Wextra -Werror -pedantic
IFLAGS=-Iinclude
LFLAGS=

OBJS=main.o
OBJS+=lib/vm/token.o
OBJS+=lib/vm/executor.o

.PHONY:
help:
	@echo "\nThe following commands are supported:"
	@echo "make build\t- Build binary"
	@echo "make debug\t- Build binary in debug mode"
	@echo "make clean\t- Clean build artifacts"
	@echo

.PHONY:
debug: CFLAGS+=-DDEBUG
debug: $(PROJECT)

.PHONY:
clean:
	rm -rf $(OBJS) $(PROJECT)

.PHONY:
build: $(PROJECT)

$(PROJECT): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(IFLAGS) $< -o $@

lib/%.c: include/%.h

