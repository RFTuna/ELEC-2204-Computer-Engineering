CC = g++
CFLAGS = -std=c++17 -g -Wall -Wextra -Wfloat-equal -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS =  $(wildcard $(SRC_DIR)/*.cpp)

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

EXE = $(BIN_DIR)/main

.PHONY: all clean run debug

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	bin\main

debug: all
	bin\main debug -f test/testcode.mips

clean:
	@-del /Q $(BIN_DIR) $(OBJ_DIR)