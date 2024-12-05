# Compiler and flags
CC := gcc
CFLAGS := -Wall -Werror -std=c11
LDFLAGS := 
BUILD_DIR := build
SRC_DIR := src

# Source and object files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Output binary name
TARGET := $(BUILD_DIR)/asciivideo

# Default rule: build the program
all: $(BUILD_DIR) $(TARGET)

# Link all object files into the final binary
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)


run:
	./build/asciivideo

# Add a "phony" rule to avoid conflicts with files named "all" or "clean"
.PHONY: all clean
