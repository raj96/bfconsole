PROJECT=bfconsole

CC=clang
CFLAGS=-Wall -Wextra -Werror -pedantic
IFLAGS=-Iinclude
LFLAGS=

OBJS=main.o
OBJS+=lib/vm/token.o
OBJS+=lib/vm/executor.o

$(PROJECT): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(IFLAGS) $< -o $@

lib/%.c: include/%.h

.PHONY:
debug: CFLAGS+=-DDEBUG
debug: $(PROJECT)

.PHONY:
clean:
	rm -rf $(OBJS) $(PROJECT)

