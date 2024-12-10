# Compiler and flags
CC = gcc
CXX = g++
CXXFLAGS = -Wall -Wextra -lgmp -lgmpxx

# Directories
SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build

# Target executable
TARGET = main

# Source files and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Add library directory
CXXFLAGS += -I$(INC_DIR)

# Default target
all: $(TARGET)

# Build target executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(TARGET).cpp -o $(BUILD_DIR)/$@ $(CXXFLAGS)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run:
	./build/$(TARGET)

.PHONY: all clean run
