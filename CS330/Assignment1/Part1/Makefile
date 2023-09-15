FLAGS=-Wall -std=c99
BINDIR=bin
CC=gcc

all: sqroot square double

sqroot: sqroot.c
	$(CC) -o $(BINDIR)/$@ $^ -lm $(FLAGS)

square: square.c
	$(CC) -o $(BINDIR)/$@ $^ $(FLAGS)

double: double.c
	$(CC) -o $(BINDIR)/$@ $^ $(FLAGS)
