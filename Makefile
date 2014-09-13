SRC = $(wildcard *.c)
OBJS = $(SRC:%.c=%.o)
CC = gcc
CFLAGS = --std=gnu99 -g -Wall
LIBS = -lgomp

all: zombie

zombie: $(OBJS)
	$(CC) $(CFLAGS) -o zombie $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS)

.PHONY:clean
