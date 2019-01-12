OBJS = game.o
CFLAGS = -Wall -pedantic -std=gnu99 -g

all: game

fitz: $(OBJS)
	gcc -lm -o fitz fitz.o $(CFLAGS)

fitz.o: game.c
	gcc -c game.c -o game.o $(CFLAGS)

clean: rm -f game

