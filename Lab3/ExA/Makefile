# 1. Compiler and Flags
CC       = gcc
CFLAGS   = -Wall -Wextra -g

# 2. Files and Paths
# Target executable name in the top-level directory
TARGET   = shell.out

# Name of the temporary build directory
BUILD_DIR = build

# Find all C files in the current folder
SRCS     = $(wildcard *.c)

# Generate object file paths inside the build directory
# Example: main.c becomes build/main.o
OBJS     = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))

# 3. Targets and Rules
.PHONY: all clean

# Default target
all: $(TARGET)

# Link object files into the final executable at the top level
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile C files into object files inside the build directory
# The | rule ensures the build directory is created before compiling
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up build artifacts safely
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
