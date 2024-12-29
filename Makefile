# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c99 -Iraylib/include

# Linker flags (for raylib and its dependencies)
LDFLAGS = -L./raylib/lib_linux -Wl,-rpath,./raylib/lib_linux -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Source and output
SRC = src/main.c
OUT = build_linux/meow_editor

# Build target
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

run:
	./$(OUT)

# Clean target
clean:
	rm -f $(OUT)

rebuild: clean $(OUT)

.PHONY: all clean
