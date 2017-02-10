/**
 * Connor Campbell (2016)
 * ccampb21@mail.uoguelph.ca
 */
 
#ifndef __CCAMPB21_PLAYGAME__
#define __CCAMPB21_PLAYGAME__
 
#include <ncurses.h>

typedef struct Hero_
{
    int health;
    int attack;
    int defense;
    int pCount;
    int iCount;
    char items[5];
    int val[5];

}Hero;

/** 
 * gameLoop
 * A while loop that gathers user input to move the hero character
 * around the screen until the users hits q or reahces a staircase. 
 * When they do exit, their gold total is printed out.
 * 
 * IN: Room* for room elements, 3 ints for the hero's starting position,
 *     and one int for the y coordinate of the status bar
 * OUT: none
 * 
 * POST: the user will have played the game, ncurses will be 
 * de-initialized and the gold total will be printed out. 
 * ERROR: The player will not be able to move the hero character if the
 * coordinates are wrong.
 */
void gameLoop(Room* rPtr, int heroY, int heroX, int heroRoom, int statY);

/** 
 * singleMove
 * Jacob's function from lab5. Modified for different kinds of collison.
 * May move the character one space depending on the character the hero
 * steps on.
 * 
 * IN: Room* for room elements, char for user input, 2 ints for the
 *     current cursor position, char for the last character stepped on, and
 *     Hero* for the hero struct
 * OUT: char that returns different values to gameLoop depending on what
 *      canMove returns.
 * 
 * POST: The hero will move one space, or no spaces at all
 * ERROR: no errors should occur because nothing will happen if one of
 * the four input characters is not pressed.
 */
char singleMove(Room* rPtr, char input, int currY, int currX, char lastCh, Hero* hero);

/** 
 * encounter
 * Does a number of things depending on the character returned: Either
 * moves the hero or keeps it in the same spot, prints out a message if
 * a special event occurs, enters a combat phase, and even kills the hero.
 * Most of this is done by making use of other functions.
 * 
 * IN: Two ints for where you want the hero to move, a char for the
 *     character that you stepped one last, Room* pointer for info about
 *     the rooms and Hero* for the hero stats.
 * OUT: char for the character that the hero has currently overwpritten
 * 
 * POST: some sort of action will be taken based on the character that
 *       the hero tried to move on
 * ERROR: none
 */
char encounter(int y, int x, char lastCh, Room* rPtr, Hero* hero);

/** 
 * moveCharacter
 * Jacob's funtion from lab 5. Will overwrite the current cursors
 * location and then move and print out the hero character in the
 * direction indicated by the parameters.
 * 
 * IN: 2 ints for the coordinates that the hero character is to be
 *     printed in, char for the character that you want to be printed in the
 *     location that the hero was previously in.
 * OUT: none
 * 
 * POST: The hero character will move one space.
 * ERROR: There should be no errors if the parameters correspond
 *        properly with the input character.
 */
void moveCharacter(int updatedY, int updatedX, char spot);

/** 
 * canMove
 * Jacob's canMove function from lab 5. Modified to detect collisions
 * with monsters, gold, doors, and stairs. 
 * 
 * IN: two ints for the coordinates that the player is trying to move to
 * OUT: returns a chart to indicate what character is in the position
 *      that the player is trying to move to.
 * 
 * POST: The function will either tell send a character to the encounter
 *       function, which will then decide which course of action should
 *       be taken.
 * ERROR: the function should always successfully return a value unless
 *        parameters are different from the direction that the hero is
 *        attempting to move
 */
char canMove(int toGoY, int toGoX);

/** 
 * printMsg
 * Justin's print message function. Prints out a message at the top of 
 * the screen in ncurses
 * 
 * IN: const char* which is the message 
 * OUT: none
 * 
 * POST: Prints out the input message 
 * ERROR: Should not be any if ncurses is active and the input is 
 *        the correct type 
 */
void printMsg(const char* msg);

/** 
 * printSats
 * Prints a message to the bottom of the screen that displays all of the
 * hero's stats
 * 
 * IN: 5 ints for the various hero stats, and 1 int for the position on
 *     the y axis where the status bar is printed.
 * OUT: none
 * 
 * POST: The hero's current stats are printed to the bottom of the screen.
 * ERROR: none; if there is not enough room the program will not run.
 */
void printStats(int h, int p, int a, int i, int y);

/** 
 * initHero
 * Mallocs for a struct of type Hero and uses the parameters to define
 * the members of the struct
 * 
 * IN: 5 ints for assigning to the hero, and a Room* for freeing the memory
 *     in case the malloc fails and the program exits
 * OUT: Hero* with the hero stats
 * 
 * POST: The hero struct will have memory allocated and its parameters
 *       will be defined
 * ERROR: Program will exit if malloc is unsuccessful
 */
Hero* initHero(int h, int a, int d, int p, int i, Room* rPtr);

/** 
 * checkRoom
 * Takes the current cursor coordinates and uses the room pointer to
 * determine which room the cursor is in.
 * 
 * IN: 2 ints for the current x and y coordinates and Room* for the room
 *     locations
 * OUT: int for room number
 * 
 * POST: returns the room number for the current coordinate.
 * ERROR: If the function is used with coordinates outside of a room,
 *        it will return -1, and the program will segfault if it attempts
 *        to access a room of that number 
 */
int checkRoom(int x, int y, Room* rPtr);

/** 
 * addVal
 * Increases a hero's attack or defense depending on the input parameter
 * 
 * IN: Room* for the information about the item, Hero* for information
 *     about the hero stats, and an int* so the function can be used to
 *     add to either attack or defense
 * OUT: none
 * 
 * POST: Either the hero's attack or the hero's defense will be increased
 * ERROR: none
 */
void addVal(Room* rPtr, Hero* hero, int* val);

/** 
 * exchange
 * Executes a battle phase between the player and the monster. One of
 * them will lose health, and one of them could die. The Aquator will
 * take damage from the hero, rather than health.
 * 
 * IN: Room* for info about the monster, Hero* for info about the hero,
 *     and an int to indicate wheteher or not the enemy is an Aquator
 * OUT: returns an int to indicate whether or not there was a death
 * 
 * POST: Either the enemy or hero will lose health (or attack) and one
 *       of them may die.
 * ERROR: none, there are always checks to make sure the values never
 *        drop below 0.
 */
int exchange(Room* rPtr, Hero* hero, int type);

/** 
 * printDam
 * Prints various messages depending on who takes damage and also
 * depending on whether or not the monster or the hero dies.
 * 
 * IN: an int for the amount of damage caused, and another int to
 *     specify which type of maessage should be printed.
 * OUT: none
 * 
 * POST: a message about the damage outcome will be printed.
 * ERROR: none, since the function is essentially only a print statement
 *        with one variable.
 */
void printDam(int dmg, int type);

/** 
 * moveMon
 * Moves the cursor around the sreen to each of the monsters that are
 * currently alive and moves them one space in their movement pattern.
 * If they try to move in a direction that has anything other than a hero
 * or a dot character, they will automatically attempt the next type of
 * movement in their pattern. If they try to move on a hero, they simply
 * will not move.
 * 
 * IN: Room* for updating the monster's location, char for the type of
 *     monster, two ints for the monster's current coordinates, an int
 *     for the monsters current movement pattern, and two more ints to
 *     access the correct information in the room struct.  
 * OUT: none
 * 
 * POST: the monster will move a space
 * ERROR: none
 */
void moveMon(Room* rPtr, char type, int monY, int monX, int move, int i, int j);

/** 
 * randDir
 * Calculates a random interger, and based on that returns a character
 * which indicates a direction
 * 
 * IN: none
 * OUT: char for a direction
 * 
 * POST: A random direction is calculated
 * ERROR: none, if something goes wrong when calculating the random number,
 *        the function will simply return 'n'
 */
char randDir();

#endif
