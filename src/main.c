/**
 * Connor Campbell (2016)
 * ccampb21@mail.uoguelph.ca
 */

#include "preGame.h"
#include "drawGame.h"
#include "playGame.h"

int main(int argc, char* argv[])
{
    checkArg(argc, argv);
    
    Room* roomInfo;
    int heroY;
    int heroX;
    int heroRoom;
    int statusY;
    int yAdd;
    yAdd = 0;

    //mallocs the room pointer and parses the level file
    roomInfo = scanLevel(argv[1], &yAdd);
    
    // starts ncurses, draws the room and fills it with elements
    drawRooms(roomInfo, &heroY, &heroX, &heroRoom, &statusY, yAdd);

    //accepts user input to play the game, ends ncurses
    gameLoop(roomInfo, heroY, heroX, heroRoom, statusY);
    
    free(roomInfo);

    return 0;
}
