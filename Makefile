CC=gcc
CFLAGS=-c -Wall -std=c99 -Iinclude
EXE=rogue
LLIB=-lncurses

all:
	gcc -Wall -std=c99 -lncurses src/main.c src/preGame.c src/drawGame.c src/playGame.c -Iinclude -o rogue
