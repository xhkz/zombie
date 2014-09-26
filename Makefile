TARGET = zombie

SRC = $(wildcard *.c)
OBJS = $(SRC:%.c=%.o)

ifeq ($(HOSTNAME), avoca)
CC = mpicc
else
CC = gcc
endif

CFLAGS = --std=gnu99 -g -Wall -fopenmp
LIBS = -lgomp

all: clean zombie

zombie: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJS)


