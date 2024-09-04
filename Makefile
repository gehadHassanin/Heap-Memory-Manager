# Makefile for Custom Memory Management

# Compiler and flags
CC = gcc
CFLAGS = -g -Wall 

# Source files, Objs files and target names
SRC := $(wildcard src/*.c)
TEST_SRC ?= main.c
TEST_OBJ := $(SRC:.c=.o)
TEST_OBJ += $(TEST_SRC:.c=.o)
SHARED_OBJ := $(SRC:.c=.dynamic.o)
SHARED_TARGET := libhmm.so
TEST_TARGET := hmm

.PHONY: all clean run run-with-lib

test-build: $(TEST_TARGET)

# Compile the test program
$(TEST_TARGET): $(TEST_OBJ)
	@$(CC) $(CFLAGS) $(TEST_OBJ) -o $@

# Create the shared library
$(SHARED_TARGET): $(SHARED_OBJ)
	@$(CC) --shared $(SHARED_OBJ) -o $@

# Run the test program
run: $(TEST_TARGET)
	@./$(TEST_TARGET)

# Run any command using shared library
run-shared-lib: $(SHARED_TARGET)
	@LD_PRELOAD=$(realpath $(SHARED_TARGET)) $(COMMAND)
 

# RULE TO COMPILE .c FILES TO .o FILES
$(TEST_OBJ): %.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

# RULE TO COMPILE .c FILES TO .dynamic.o FILES
$(SHARED_OBJ): %.dynamic.o: %.c
	@$(CC) $(CFLAGS) -fPIC -c $< -o $@

# Clean the build files
clean:
	@rm -rf *.o
	@rm -rf *.dynamic.o
	@rm -rf src/*.o
	@rm -rf $(TEST_TARGET) 
	@rm -rf $(SHARED_TARGET)


