OBJS=$(wildcard src/*.c src/**/*.c)

CC=gcc

CFLAGS= -Wall -g

LDFLAGS= -lm

TARGET=task2

all : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)