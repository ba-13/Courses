# Makefile for package with tests, src, and include directories

CC := gcc
CFLAGS := -g -Wall
LDLIBS := -lm

# Directories
TEST_DIR := test
SRC_DIR := src
INCLUDE_DIR := include
BIN_DIR := bin
OBJ_DIR := obj

# Header files
INCLUDE_FILES := $(wildcard $(INCLUDE_DIR)/*.h)

# Source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Test files
TEST_FILES := $(wildcard $(TEST_DIR)/*.c)
TEST_BINARIES := $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%, $(TEST_FILES))

# Compiler flags
CFLAGS += -I$(INCLUDE_DIR)

# Targets
.PHONY: all clean

all: $(TEST_BINARIES)

# Compile object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_FILES)
	@mkdir -p $(OBJ_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

# Compile test binaries
$(BIN_DIR)/%: $(TEST_DIR)/%.c $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $< $(OBJ_FILES) $(CFLAGS) $(LDLIBS)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
