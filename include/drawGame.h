/**
 * Connor Campbell (2016)
 * ccampb21@mail.uoguelph.ca
 */

#ifndef __CCAMPB21_DRAWGAME__
#define __CCAMPB21_DRAWGAME__

#include <ncurses.h>


/** 
 * drawRooms
 * Draws out all of the rooms in the level, including the doors and the 
 * contents. Also provides info to main about hero position. 
 * 
 * IN: Room* for all of the level info, 3 int* for the hero's starting
 *     cordinates, as well as the room he starts in, an int* for the y
 *     coordinate of the status bar, and an int for the y coordinate of
 *     the middle of the screen.
 * OUT: none
 * 
 * POST: all the rooms and contents and hallways will be printed to the
 *       screen, the hero's starting coordinates and inital room will be
 *       returned to main.
 * ERROR: none if the file was parsed correctly.
 */
void drawRooms(Room* rPtr, int* hYPos, int* hXPos, int* hRNum, int* statY, int yAdd);

/** 
 * checkWindow
 * checks the size of the window and compares it to the size needed for
 * printing out all the rooms. Exits the program if the window is not
 * large enough.
 * 
 * IN: Room* for each room's height, length, x and y coordinates.
 * OUT: int for the y coordinate of the middle of the screen.
 * 
 * POST: the game will continue to run or it will exit.
 * ERROR: if the window is not large enough to fit all the rooms the
 *        game cannot be played properly and the program will exit.
 */
int checkWindow(Room* rPtr);

/** 
 * printRec
 * Justin's print rectangle function, modified to add floors. Prints 
 * out the walls and floor for a single room in ncurses.
 * 
 * IN: 4 ints for the room's length and width, as well as it's starting
 *     coordinates
 * OUT: int for location to print out status bar
 * 
 * POST: The room's walls and floors will be printed to the screen.
 * ERROR: There should be no errors if the checkWindow function was 
 *        successful
 */
void printRec(int y, int x, int h, int w);

/** 
 * printDoors
 * Prints out a single door in a room.
 * 
 * IN: Room* to store door coordinates for teleportation, char for the 
 * door's direction 5 ints for the amount of door spaces, the room's x 
 * and y coordinates and the room's length and height. 2 more ints to be
 * counters for storing the door's coordinates.
 * OUT: none
 * 
 * POST: A door will be printed and its coordinates will be stored in
 * the room pointer. 
 * ERROR: The function can only print a number of spaces that are within
 * the size of its length and width, otherwise the doors will be
 * inaccessible.
 */
void printDoors(Room* rPtr, char d, int s, int y, int x, int h, int l, int i, int j);

/** 
 * printItems
 * Prints out a single item in a room, and assigns values to a monster
 * struct if a monster is printed.
 * 
 * IN: char for the item type, 4 ints for the room's x any y positions
 *     and the x and y posisitons of the item within the room, a Room*
 *     and two ints for accessing and storing info in the monster struct 
 * OUT: int to increase the monster counter for the current room
 * 
 * POST: The item will be printed in the room.
 * ERROR: the function cannot print a type of element that is not
 *        described in its switch statement.
 */
int printItems(char t, int iy, int ix, int ry, int rx, int mCount, int i, Room* rPtr);

/** 
 * printHalls
 * Prints out the main framework for the hallways between all of the rooms
 * 
 * IN: a Room* for the room pointer, and an int and int* for specific y 
 *     coordinates
 * OUT: none
 * 
 * POST: Main hallways will be printed
 * ERROR: none
 */
void printHalls(Room* rPtr, int yAdd, int* statY);

/** 
 * printRoomHalls
 * Prints the hallways that connects the doors in each room to the main hallway
 * 
 * IN: Room* for the room struct, 2 ints for accssiing the room structs
 *     a char for the wall the door is on, and an int for the y coordiante
 *     where the hallways should start/stop
 * OUT: none
 * 
 * POST: the hallways for each door in each room will be connected to
 *       the main hallway
 * ERROR: none
 */
void printRoomHalls(int rNum, int dNum, char dir, Room* rPtr, int yAdd);

#endif
