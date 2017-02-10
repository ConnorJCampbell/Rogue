/**
 * Connor Campbell (2016)
 * ccampb21@mail.uoguelph.ca
 */

#ifndef __CCAMPB21_PREGAME__
#define __CCAMPB21_PREGAME__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* struct for each elemenet in each room. Contains variables for the 
 * type of element as well as the cordinates within the room.
 */
typedef struct item_
{
    char type;
    int y;
    int x;
    int val;
    int X;
    int Y;
    
}item;

/* struct for each door in each room, contains variables for parsing the
 * info from the level file, as well as the coordinates with regards to
 * the whole screen.
 */
typedef struct door_
{
    char direction;
    int spaces;
    int x;
    int y;
    
}door;

/* struct for each of the monsters, an array of 10 is placed in each room struct.
 * Holds all a monsters stats, icluding type, coordinates, movement case, and an alive value
 */
typedef struct Monster_
{
    int health;
    int attack;
    int speed;
    int xpos;
    int ypos;
    int isAlive;
    int move;
    char type;

}Monster;

/* struct for each of the 6 rooms, contains variables for all elements, 
 * doors and dimensions.
 */
typedef struct Room_ 
{
    int x;
    int y;
    int l;
    int h;
    door doors[4];
    item contents[10];
    Monster mon[10];

}Room;


/** 
 * checkArg
 * prints out the command line arguements to the user and checks if it is 
 * the correct amount for the program to run. Taken from my A1.
 * 
 * IN: int for the number of arguments and char* for the file names.
 * OUT: none
 * 
 * POST: verifies whether or not the number of files that the program was
 * executed with is correct
 * ERROR: Will exit the program if the number of arguments is not correct.
 */
void checkArg(int numArg, char* argNames[]);

/** 
 * nullFile
 * Checks if a file that was opened actually exists. Taken from my A1.
 * 
 * IN: FILE* for the file pointer, char* for the actual name of the file. 
 * OUT: none
 * 
 * POST: verifies that the file exists. If it does not, the program will exit.
 * ERROR: The program will end if the file does not exist.
 */
void nullFile(FILE* fptr, char* fName);

/** 
 * scanLevel
 * Opens the the level file, parses the data and sorts the information
 * into an array of room structs, the closes the file. Also assigns
 * values to all the collectable items. Uses an algorithm similar to my
 * A1.
 * 
 * IN: char* for the name of the level file
 * OUT: Room* for a pointer that contains all the level information.
 * 
 * POST: All the information for the level and it's rooms will be stored,
 *       the level file will be closed.
 * ERROR: The program will exit if the file does not exist or if the 
 *        malloc fails. An error or segmentation fault will occur if the
 *        format of the level file is not correct.
 */
Room* scanLevel(char* file, int* yAdd);

#endif
