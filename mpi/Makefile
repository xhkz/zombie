TARGET = zombie

SRC = $(wildcard *.c)
OBJS = $(SRC:%.c=%.o)

CC = mpicc
CFLAGS = --std=gnu99 -g -Wall -fopenmp
LIBS = -lgomp

all: clean zombie

zombie: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJS) $(TARGET)


