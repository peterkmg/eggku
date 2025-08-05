# compiler
CC = gcc

# flags
CFLAGS = -Wall -Wextra

SRC_DIR = src
BLD_DIR = build

# executable
TARGET = $(BLD_DIR)/eggku

# sources
SRC = $(wildcard $(SRC_DIR)/*.c)
HDR = $(wildcard $(SRC_DIR)/*.h)

# object files
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BLD_DIR)/%.o,$(SRC))

# rules
build: $(TARGET)
	@echo "Done."

compiling:
	@echo "Compiling source files..."

$(BLD_DIR)/%.o: $(SRC_DIR)/%.c $(HDR) compiling
	@mkdir -p $(BLD_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "    $< -> $@"

$(TARGET): $(OBJ)
	@mkdir -p $(BLD_DIR)
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "Linking object files..."
	@echo "    -> $@"

run: $(TARGET)
	@./$(TARGET)

clean:
	@echo "Cleaning up..."
	@rm -rf $(BLD_DIR)
	@echo "Cleaned up build directory."

.PHONY: build compiling run clean
