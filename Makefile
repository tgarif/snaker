# Compiler and tools
CC_LINUX = gcc
CC_WINDOWS = x86_64-w64-mingw32-gcc
WINDRES = x86_64-w64-mingw32-windres

# Flags
CFLAGS_LINUX = -Wall -std=c99 -Iinclude -Iraylib/include
CFLAGS_WINDOWS = -Wall -std=c99 -Iinclude -Iraylib/include

# Linker flags (for raylib and its dependencies)
LDFLAGS_LINUX = -L./raylib/lib_linux -Wl,-rpath,./raylib/lib_linux -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
LDFLAGS_WINDOWS_STATIC = -L./raylib/lib_windows -lraylib -lopengl32 -lgdi32 -lwinmm
LDFLAGS_WINDOWS_DYNAMIC = -L./raylib/lib_windows -lraylibdll -lopengl32 -lgdi32 -lwinmm

# Source files and output
SRC = src/snaker.c src/game.c src/input.c src/renderer.c src/utils.c
OUT_LINUX = build_linux/snaker
OUT_WINDOWS_STATIC = build_windows/snaker_static.exe
OUT_WINDOWS_DYNAMIC = build_windows/snaker_dynamic.exe

# Ico resource
ICON_RC = resources/icons/snake.rc
ICON_RES = build_windows/snake.res

# Build target
all: linux windows_static windows_dynamic

# Linux build
linux: $(OUT_LINUX)

$(OUT_LINUX): $(SRC)
	$(CC_LINUX) $(CFLAGS_LINUX) $(SRC) -o $(OUT_LINUX) $(LDFLAGS_LINUX)

# Windows static build
windows_static: $(OUT_WINDOWS_STATIC)

$(OUT_WINDOWS_STATIC): $(SRC) $(ICON_RES)
	$(CC_WINDOWS) $(CFLAGS_WINDOWS) $(SRC) $(ICON_RES) -o $(OUT_WINDOWS_STATIC) $(LDFLAGS_WINDOWS_STATIC) -mwindows

# Windows dynamic build
windows_dynamic: $(OUT_WINDOWS_DYNAMIC)

$(OUT_WINDOWS_DYNAMIC): $(SRC) $(ICON_RES)
	$(CC_WINDOWS) $(CFLAGS_WINDOWS) $(SRC) $(ICON_RES) -o $(OUT_WINDOWS_DYNAMIC) $(LDFLAGS_WINDOWS_DYNAMIC) -mwindows

# Compile resource file for Windows
$(ICON_RES): $(ICON_RC)
	$(WINDRES) $(ICON_RC) -O coff -o $(ICON_RES)

# Run targets
run_linux:
	./$(OUT_LINUX)

run_windows_static:
	wine ./$(OUT_WINDOWS_STATIC)

run_windows_dynamic:
	wine ./$(OUT_WINDOWS_DYNAMIC)

# Clean targets
clean:
	rm -f $(OUT_LINUX) $(OUT_WINDOWS_STATIC) $(OUT_WINDOWS_DYNAMIC) $(ICON_RES)

rebuild: clean all

.PHONY: all linux windows_static windows_dynamic clean rebuild run_linux run_windows_static run_windows_dynamic
