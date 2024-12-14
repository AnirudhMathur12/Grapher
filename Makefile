SRC := $(wildcard src/*.c)
INC := include


make:
	gcc `pkg-config --cflags --libs sdl2 sdl2_image` src/main.c -o grapher -g -lm
