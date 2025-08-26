# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I source -I tests
LDFLAGS = 

# Directories
SRC_DIR = source
BIN_DIR = bin
TESTS_DIR = tests

# Application targets
APP_NAME = library_app
TEST_NAME = library_test

# Source files
SRCS = $(SRC_DIR)/Book.cpp $(SRC_DIR)/User.cpp $(SRC_DIR)/Library.cpp $(SRC_DIR)/LibraryUI.cpp $(SRC_DIR)/main.cpp
TEST_SRCS = $(TESTS_DIR)/tests.cpp

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRCS))
TEST_OBJS = $(patsubst $(TESTS_DIR)/%.cpp,$(BIN_DIR)/%.o,$(TEST_SRCS)) \
            $(BIN_DIR)/Book.o $(BIN_DIR)/User.o $(BIN_DIR)/Library.o

# Default target
all: $(APP_NAME) $(TEST_NAME)

# Main application
$(APP_NAME): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Test application
$(TEST_NAME): $(TEST_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compile source files to object files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile test files to object files
$(BIN_DIR)/%.o: $(TESTS_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Dependencies
$(BIN_DIR)/Book.o: $(SRC_DIR)/Book.h
$(BIN_DIR)/User.o: $(SRC_DIR)/User.h
$(BIN_DIR)/Library.o: $(SRC_DIR)/Library.h $(SRC_DIR)/Book.h $(SRC_DIR)/User.h
$(BIN_DIR)/LibraryUI.o: $(SRC_DIR)/LibraryUI.h $(SRC_DIR)/Library.h
$(BIN_DIR)/main.o: $(SRC_DIR)/LibraryUI.h
$(BIN_DIR)/tests.o: $(TESTS_DIR)/catch.hpp $(SRC_DIR)/Book.h $(SRC_DIR)/User.h $(SRC_DIR)/Library.h

# Clean up
clean:
	rm -rf $(BIN_DIR) $(APP_NAME) $(TEST_NAME)

# Phony targets
.PHONY: all clean