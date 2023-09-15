SRC := Testcases

SOURCES := $(wildcard $(SRC)/*.c)
OBJ = $(patsubst %.c,%.o,$(SOURCES))
EXEC = $(patsubst %.c,%,$(SOURCES))
FLAGS = -g -Wall

all: $(OBJ) mylib.o $(EXEC)

%: %.o mylib.o
	$(CC) $< mylib.o -o $@ $(FLAGS)

%.o: %.c
	$(CC) -c $< -o $@

mylib.o: mylib.c
	$(CC) -c mylib.c

clean:
	rm mylib.o
	rm $(OBJ)
	rm $(EXEC)
