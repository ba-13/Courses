CC		:= gcc
BIN		:= ./bin
OBJ		:= ./obj
INCLUDE	:= ./include
SRC		:= ./src
SRCS	:= $(wildcard $(SRC)/*.c)
INCLUDES:= $(wildcard $(INCLUDE)/*.h)
OBJS	:= $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
CFLAGS	:= -g -Wall -I$(INCLUDE)
LDLIBS 	:= -lm

$(OBJ)/%.o: $(SRC)/%.c $(INCLUDES)
	@mkdir -p $(BIN)
	@mkdir -p $(OBJ)
	$(CC) -c -o $@ $< $(CFLAGS)

kruskal: $(OBJ)/kruskal.o
	$(CC) -o $(BIN)/$@ $^ $(CFLAGS)

prim: $(OBJ)/prim.o $(OBJ)/rway_heaps.o $(OBJ)/graph.o
	$(CC) -o $(BIN)/$@ $^ $(CFLAGS)

clean:
	rm -rf $(OBJ) $(BIN)
