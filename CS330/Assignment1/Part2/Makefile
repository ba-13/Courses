CC = gcc
FLAGS = -Wall -std=gnu99

myDU: myDU.o
	$(CC) -o $@ $^ $(FLAGS)

myDU.o: myDU.c
	$(CC) -o $@ -c $^ $(FLAGS)

clean:
	rm myDU.o myDU