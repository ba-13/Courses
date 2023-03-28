CC = gcc
CFLAGS = -O4 -Wall
DEPS = avl_tree.h
OBJ = avl_tree.o
BINDIR = bin
ODIR = obj

# should change if .h changes
$(ODIR)/%.o: %.c $(DEPS)
	@mkdir -p $(ODIR)
	@mkdir -p $(BINDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

avl_tree: $(ODIR)/avl_tree_test.o $(ODIR)/$(OBJ)
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS)

rotation: $(ODIR)/rotation_test.o $(ODIR)/$(OBJ)
	$(CC)  -o $(BINDIR)/$@ $^ $(CFLAGS)

minimum: $(ODIR)/minimum_find.o $(ODIR)/$(OBJ)
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS)

random_numbers: $(ODIR)/generate_random_numbers.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS)

clean:
	rm -rf ./obj/*.o ./bin/* ./*.out ./*.o
