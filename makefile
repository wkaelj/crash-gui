UNAME := $(shell uname)

BIN := build

LIBCRASHGUI_STATIC := $(BIN)/libcrashgui-static.a
LIBCRASHGUI_SHARED := $(BIN)/libcrashgui-shared.so
SRC = src/crashgui.c

ifeq ($(UNAME), Linux)
	SRC += $(wildcard src/window_gtk/*.c)
	WINDOW_DIR += src/window_gtk
endif
# osx should use universal window
ifeq ($(UNAME), Darwin)
	SRC += $(wildcard src/window_osx/*.c)
	WINDOW_DIR += src/window_osx
endif

CFLAGS = -std=gnu2x -O3
CFLAGS += -Wall -Wextra -fPIC
CFLAGS += `pkg-config gtk4 --cflags`

LDFLAGS := `pkg-config gtk4 --libs`

CC = cc

OBJ := $(SRC:%.c=$(BIN)/%.o)

.PHONY: all

all: static

dirs: 
	mkdir -p $(BIN) $(BIN)/$(WINDOW_DIR)

test: test.c dirs static
	cc $< -o $@ -std=gnu2x -Wall -Wextra -L./$(BIN) -lcrashgui-static $(LDFLAGS)
	./test

clean:
	rm -f $(OBJ) $(LIBCRASHGUI_STATIC) $(LIBCRASHGUI_SHARED)

shared: $(OBJ)
	$(CC) -shared $^ $(LDFLAGS) $(CFLAGS) -o $(LIBCRASHGUI_SHARED)

static: $(OBJ)
	ar rcs $(LIBCRASHGUI_STATIC) $^
	
$(BIN)/%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)
	
