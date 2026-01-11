PROJECT=bfconsole

CC=clang
CFLAGS+=-Wall -Wextra -Werror
IFLAGS=-Iinclude
LFLAGS=

OBJS=main.o
OBJS+=lib/vm/token.o

$(PROJECT): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(IFLAGS) $< -o $@

lib/%.c: include/%.h

.PHONY:
clean:
	rm -rf $(OBJS) $(PROJECT)
