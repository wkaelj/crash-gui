UNAME := $(shell uname)

BIN = build

CFLAGS = -std=gnu2x -O3
CFLAGS += -Wall -Wextra -fPIC
CFLAGS += `pkg-config gtk4 --cflags`
LDFLAGS = `pkg-config gtk4 --libs` -g
SRC = src/crashgui.c
OBJ = $(SRC:%.c=$(BIN)/%.o)

ifeq ($(UNAME), Linux)
	SRC += $(wildcard src/window_gtk/*.c)
	WINDOW_DIR += src/window_gtk
endif
ifeq ($(UNAME), Darwin)
	SRC += $(wildcard src/window_osx/*.c)
	WINDOW_DIR += src/window_osx
endif

CC = cc

.PHONY: all

all: dirs static  test

dirs: 
	mkdir -p $(BIN) $(BIN)/$(WINDOW_DIR)

test: test.c dirs static
	cc $< -o $@ -std=gnu2x -Wall -Wextra -L./ -lcrash-gui-static $(LDFLAGS)

clean:
	rm -f $(OBJ) libcrash-gui-static.a libcrash-gui.so

shared: $(OBJ)
	$(CC) -shared $^ $(LDFLAGS) $(CFLAGS) -o libcrash-gui.so

static: $(OBJ)
	ar rcs libcrash-gui-static.a $^
	
$(BIN)/%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)
	
