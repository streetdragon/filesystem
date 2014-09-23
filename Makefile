# Name: Kurt McAlpine 	\
  UPI: 2004750 		\
  ID: 2004750

CC=gcc
CFLAGS=-I. -lm
DEPS = main.h fileutils.h commands.h
OBJ = main.o fileutils.o commands.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

A2com: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
