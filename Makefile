CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRCS = bitmap.c memory_manager.c
OBJS = $(SRCS:.c=.o)

LIBRARY = memory_manager.a

all: $(LIBRARY)

$(LIBRARY): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(LIBRARY)
