UNAME := $(shell uname)

CFLAGS = -std=gnu2x -O3
CFLAGS += -Wall -Wextra
CFLAGS += `pkg-config gtk4 --cflags`
LDFLAGS = `pkg-config gtk4 --libs` -g
SRC = src/crashgui.c
OBJ = $(SRC:.c=.o)

ifeq ($(UNAME), Linux)
	SRC += $(wildcard src/window_gtk/*.c)
endif
ifeq ($(UNAME), Darwin)
	SRC += $(wildcard src/window_osx/*.c)
endif

CC = cc

.PHONY: all

all: static

clean:
	rm -f $(OBJ) libcrash-gui.a libcrash-gui.so

shared: $(OBJ)
	$(CC) -shared $^ $(LDFLAGS) $(CFLAGS) -o libcrash-gui.so

static: $(OBJ)
	ar rcs libcrash-gui.a $^
	
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)
	
