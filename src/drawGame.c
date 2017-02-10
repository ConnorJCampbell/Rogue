/**
 * Connor Campbell (2016)
 * ccampb21@mail.uoguelph.ca
 */

#include "preGame.h"
#include "drawGame.h"
#include "playGame.h"

void drawRooms(Room* rPtr, int* hYPos, int* hXPos, int* hRNum, int* statY, int yAdd)
{
    
    int i;
    int j;
    int k;
    int mCount;
    mCount = 0;
    
    
    //checks that the window is big enough
    *statY = checkWindow(rPtr);
        
    //foor loop for each room to print doors and items
    for (i=0; i<6; i++)
    {
        printRec(rPtr[i].y, rPtr[i].x, rPtr[i].h, rPtr[i].l);
        //for rooms with multiple doors and for printing hallways for each door
        for (j=0; j<4; j++)
        {
            printDoors(rPtr, rPtr[i].doors[j].direction, rPtr[i].doors[j].spaces, rPtr[i].y, rPtr[i].x, rPtr[i].h, rPtr[i].l, i, j);
            printRoomHalls(i, j, rPtr[i].doors[j].direction, rPtr, yAdd+6);
        }
        //for printing out all elements in a room
        for (k=0; k<10; k++)
        {
            mCount += printItems(rPtr[i].contents[k].type, rPtr[i].contents[k].y, rPtr[i].contents[k].x, rPtr[i].y, rPtr[i].x, mCount, i, rPtr);
            //store the hero coordinates and room number
            if(rPtr[i].contents[k].type == 'h')
            {
                *hYPos = (rPtr[i].contents[k].y + rPtr[i].y);
                *hXPos = (rPtr[i].contents[k].x + rPtr[i].x);
                *hRNum = i;
            }
        }
        mCount = 0;//reset mCount at the end of each room
    }
    //print hallway framework
    printHalls(rPtr, yAdd, statY);
    
            
}

int checkWindow(Room* rPtr)
{
    int maxXScreen;
    int maxYScreen;
    int maxXRooms;
    int maxYRooms;
    
    //check the max length for both rows
    maxXRooms = (rPtr[2].x + rPtr[2].l + 3);
    if (rPtr[5].x + rPtr[5].l + 3 > maxXRooms)
        maxXRooms = rPtr[5].x + rPtr[5].l + 3;
    
    //check the max length for all 3 columns
    maxYRooms = (rPtr[3].y + rPtr[3].h + 4);
    if (rPtr[4].y + rPtr[4].h + 4 > maxYRooms)
        maxYRooms = rPtr[4].y + rPtr[4].h + 4;
    if (rPtr[5].y + rPtr[5].h + 4 > maxYRooms)
        maxYRooms = rPtr[5].y + rPtr[5].h + 4;
    
    //start ncurses
    initscr();
    noecho();
    cbreak();
    
    //compares the screen size to the room size and prints the information
    getmaxyx(stdscr, maxYScreen, maxXScreen);
    printw("Screen size is %dX%d", maxYScreen, maxXScreen);
    mvprintw(1, 0, "Size necessary for rooms: %dX%d\n", maxYRooms, maxXRooms);
    mvprintw(2, 0, "(Press any key to continue...)");
    getch();
    //end ncurses, free malloc and exit program if window is not big enough
    if (maxXRooms > maxXScreen || (maxYRooms) > maxYScreen)
    {
        endwin();
        free(rPtr);
        printf("Window size not large enough to run game.\n"
                "Exiting program\n");
        exit(3);
    }
    //clear all this info from the screen.
    erase();
    
    return (maxYRooms-1);
}

void printRec(int y, int x, int h, int w)
{
    //print top wall
    for (int i=x-1; i<=w+x; i++)
        mvaddch(y-1, i, '-');

    //print side walls
    for (int i=y; i<h+y; i++)
    {
        mvaddch(i, x-1, '|');
        mvaddch(i, w+x, '|');
    }

    //print bottom wall
    for (int i=x-1; i<=w+x; i++)
        mvaddch(h+y, i, '-');
    
    //fill room with dots
    for (int i=y; i<h+y; i++)
    {
        for (int j=x; j<w+x; j++)
            mvaddch(i, j, '.');     
    }
}


void printDoors(Room* rPtr, char d, int s, int y, int x, int h, int l, int i, int j)
{
        //print top door
        if(d == 'n')
        {
            move(y-1, x+s);
            rPtr[i].doors[j].y = getcury(stdscr);
            rPtr[i].doors[j].x = getcurx(stdscr);
            addch('+');
        }
        //print bottom door
        else if(d == 's')
        {
            move(y+h, x+s);
            rPtr[i].doors[j].y = getcury(stdscr);
            rPtr[i].doors[j].x = getcurx(stdscr);
            addch('+');
        }
        //print right door
        else if(d == 'e')
        {
            move(y+s, x+l);
            rPtr[i].doors[j].y = getcury(stdscr);
            rPtr[i].doors[j].x = getcurx(stdscr);
            addch('+');
        }
        //print left door
        else if(d == 'w')
        {
            move(y+s, x-1);
            rPtr[i].doors[j].y = getcury(stdscr);
            rPtr[i].doors[j].x = getcurx(stdscr);
            addch('+');
        }    
}

int printItems(char t, int iy, int ix, int ry, int rx, int mCount, int i, Room* rPtr)
{
    //int for storing randomly generated weak monster
    int monType;
    monType = 0;
    
    //switch for printing out each different element character
    switch (t)
    {
        case 'h':
            mvaddch(ry + iy, rx + ix, '@');
            break;
        
        case 'z':
            mvaddch(ry + iy, rx + ix, '<');
            break;
        
        case 'a':
            mvaddch(ry + iy, rx + ix, '>');
            break;
        
        case 'g':
            mvaddch(ry + iy, rx + ix, '*');
            break;
        
        case 'G':
            mvaddch(ry + iy, rx + ix, '8');
            break;
    
        case 'w':
            mvaddch(ry + iy, rx + ix, ')');
            break;
    
        case 'W':
            mvaddch(ry + iy, rx + ix, '(');
            break;
        
        //randomly generate a number to determine which weak monster is printed
        case 'm':
            move(ry + iy, rx + ix);
            rPtr[i].mon[mCount].ypos = getcury(stdscr);
            rPtr[i].mon[mCount].xpos = getcurx(stdscr);
            
            //assign stats to each monster based on their randomly generated type
            monType = (rand() % 4) + 1;
            if (monType == 1)
            {
                rPtr[i].mon[mCount].health  = 5;
                rPtr[i].mon[mCount].attack  = 1;
                rPtr[i].mon[mCount].speed   = 2;
                rPtr[i].mon[mCount].isAlive = 1;
                rPtr[i].mon[mCount].type    = 'A';
                rPtr[i].mon[mCount].move    = 1;
                addch('A');
                return 1;//return 1 to increase mCount
            }
            else if (monType == 2)
            {
                rPtr[i].mon[mCount].health  = 2;
                rPtr[i].mon[mCount].attack  = 5;
                rPtr[i].mon[mCount].speed   = 4;
                rPtr[i].mon[mCount].isAlive = 1;
                rPtr[i].mon[mCount].type    = 'B';
                rPtr[i].mon[mCount].move    = 1;
                addch('B');
                return 1;
            }    
            else if (monType == 3)
            {
                rPtr[i].mon[mCount].health  = 5;
                rPtr[i].mon[mCount].attack  = 5;
                rPtr[i].mon[mCount].speed   = 4;
                rPtr[i].mon[mCount].isAlive = 1;
                rPtr[i].mon[mCount].type    = 'S';
                rPtr[i].mon[mCount].move    = 1;
                addch('S');
                return 1;
            }
            else if (monType == 4)
            {
                rPtr[i].mon[mCount].health  = 15;
                rPtr[i].mon[mCount].attack  = 5;
                rPtr[i].mon[mCount].speed   = 2;
                rPtr[i].mon[mCount].isAlive = 1;
                rPtr[i].mon[mCount].type    = 'Z';
                rPtr[i].mon[mCount].move    = 1;
                addch('Z');
                return 1;
            }    
            break;
        
        case 'M':
            move(ry + iy, rx + ix);
            
            rPtr[i].mon[mCount].ypos = getcury(stdscr);
            rPtr[i].mon[mCount].xpos = getcurx(stdscr);
            
            rPtr[i].mon[mCount].health  = 50;
            rPtr[i].mon[mCount].attack  = 5;
            rPtr[i].mon[mCount].speed   = 3;
            rPtr[i].mon[mCount].isAlive = 1;
            rPtr[i].mon[mCount].type    = 'T';
            rPtr[i].mon[mCount].move    = 1;
            addch('T');
            return 1;
            break;
    
        case 'e':
            mvaddch(ry + iy, rx + ix, ']');
            break;
            
        case 'p':
            mvaddch(ry + iy, rx + ix, '!');
            break;
    }
    return 0;       
}

void printHalls(Room* rPtr, int yAdd, int* statY)
{
    int i;
    int j;
    int maxXRooms;
    yAdd += 4;
    
    maxXRooms = (rPtr[2].x + rPtr[2].l);
    if (rPtr[5].x + rPtr[5].l > maxXRooms)
        maxXRooms = rPtr[5].x + rPtr[5].l;
    
    //printing out the basic framework for the hallways
    for(i=0; i<2; i++)
    {
        for(j=0; j<(yAdd-1); j++)
        {
            mvaddch((3 + j), (rPtr[i].x + rPtr[i].l + 4), '#');
        }
    }
    for(i=3; i<5; i++)
    {
        for(j=(yAdd); j<(*statY-4); j++)
        {
            mvaddch((2 + j), (rPtr[i].x + rPtr[i].l + 4), '#');
        }
    }
    for(i = 2; i<maxXRooms+1; i++)
        mvaddch(yAdd+2, i, '#');
}

void printRoomHalls(int rNum, int dNum, char dir, Room* rPtr, int yAdd)
{
    int i;
    i = 0;
    switch(rNum)
    {
        //print doors for first room
        case 0:
            switch(dir)
            {
                case 'n':
                    mvaddch(2, rPtr[rNum].doors[dNum].x, '#');
                    mvaddch(2, rPtr[rNum].x + rPtr[rNum].l + 4, '#');
                    for(i=rPtr[rNum].doors[dNum].x; i<rPtr[rNum].x + rPtr[rNum].l + 5; i++)
                        mvaddch(rPtr[rNum].doors[dNum].y-2, i, '#');
                    break;
                case 's':
                    for(i=rPtr[rNum].doors[dNum].y + 1; i<yAdd; i++)
                        mvaddch(i, rPtr[rNum].doors[dNum].x, '#');
                    break;
                case 'w':
                    mvaddch(rPtr[rNum].doors[dNum].y, rPtr[rNum].doors[dNum].x-1, '#');
                    mvaddch(yAdd, rPtr[rNum].doors[dNum].x-1, '#');
                    for(i=rPtr[rNum].doors[dNum].y; i<yAdd+1; i++)
                        mvaddch(i, rPtr[rNum].doors[dNum].x-2, '#');
                    break;
                case 'e':
                    for(i=rPtr[rNum].doors[dNum].x + 1; i<rPtr[rNum].x + rPtr[rNum].l + 5; i++)
                        mvaddch(rPtr[rNum].doors[dNum].y, i, '#');
                    break;
            }        
            break;
        //print doors for second room    
        case 1:
            switch(dir)
            {
                case 'n':
                    mvaddch(2, rPtr[rNum].doors[dNum].x, '#');
                    mvaddch(2, rPtr[rNum].x + rPtr[rNum].l + 4, '#');
                    for(i=rPtr[rNum].doors[dNum].x; i<rPtr[rNum].x + rPtr[rNum].l + 5; i++)
                        mvaddch(rPtr[rNum].doors[dNum].y-2, i, '#');
                    break;
                case 's':
                    for(i=rPtr[rNum].doors[dNum].y + 1; i<yAdd; i++)
                        mvaddch(i, rPtr[rNum].doors[dNum].x, '#');
                    break;
                case 'w':
                    for(i=rPtr[rNum].doors[dNum].x-1; i>rPtr[rNum].x - 5; i--)
                        mvaddch(rPtr[rNum].doors[dNum].y, i, '#');
                    break;
                case 'e':
                    for(i=rPtr[rNum].doors[dNum].x + 1; i<rPtr[rNum].x + rPtr[rNum].l + 5; i++)
                        mvaddch(rPtr[rNum].doors[dNum].y, i, '#');
                    break;
            } 
            break;
        //print doors for third room        
        case 2:
            switch(dir)
            {
                case 'n':
                    mvaddch(2, rPtr[rNum].doors[dNum].x, '#');
                    mvaddch(2, rPtr[rNum].x - 5, '#');
                    for(i=rPtr[rNum].doors[dNum].x; i>=rPtr[rNum].x -5; i--)
                        mvaddch(1, i, '#');
                    break;
                case 's':
                    for(i=rPtr[rNum].doors[dNum].y + 1; i<yAdd; i++)
                        mvaddch(i, rPtr[rNum].doors[dNum].x, '#');
                    break;
                case 'w':
                    for(i=rPtr[rNum].doors[dNum].x-1; i>rPtr[rNum].x - 5; i--)
                        mvaddch(rPtr[rNum].doors[dNum].y, i, '#');
                    break;
                case 'e':
                    mvaddch(rPtr[rNum].doors[dNum].y, rPtr[rNum].doors[dNum].x+1, '#');
                    mvaddch(yAdd, rPtr[rNum].doors[dNum].x+1, '#');
                    for(i=rPtr[rNum].doors[dNum].y; i<yAdd+1; i++)
                        mvaddch(i, rPtr[rNum].doors[dNum].x + 2, '#');
                    break;
            } 
            break;
        //print doors for fourth room    
        case 3:
            switch(dir)
            {
                case 'n':
                    for(i=rPtr[rNum].doors[dNum].y - 1; i>yAdd; i--)
                        mvaddch(i, rPtr[rNum].doors[dNum].x, '#');
                    break;
                case 's':
                    mvaddch(rPtr[rNum].doors[dNum].y + 1, rPtr[rNum].doors[dNum].x, '#');
                    mvaddch(rPtr[rNum].doors[dNum].y + 1, rPtr[rNum].x + rPtr[rNum].l + 4, '#');
                    for(i=rPtr[rNum].doors[dNum].x; i<rPtr[rNum].x + rPtr[rNum].l + 5; i++)
                        mvaddch(rPtr[rNum].doors[dNum].y+2, i, '#');
                    break;
                case 'w':
                    mvaddch(rPtr[rNum].doors[dNum].y, rPtr[rNum].doors[dNum].x-1, '#');
                    mvaddch(yAdd, rPtr[rNum].doors[dNum].x-1, '#');
                    for(i=rPtr[rNum].doors[dNum].y; i>yAdd-1; i--)
                        mvaddch(i, rPtr[rNum].doors[dNum].x-2, '#');
                    break;
                case 'e':
                    for(i=rPtr[rNum].doors[dNum].x + 1; i<rPtr[rNum].x + rPtr[rNum].l + 5; i++)
                        mvaddch(rPtr[rNum].doors[dNum].y, i, '#');
                    break;
            } 
            break;
        //print doors for fifth room
        case 4:
            switch(dir)
            {
                case 'n':
                    for(i=rPtr[rNum].doors[dNum].y - 1; i>yAdd; i--)
                        mvaddch(i, rPtr[rNum].doors[dNum].x, '#');
                    break;
                case 's':
                    mvaddch(i=rPtr[rNum].doors[dNum].y + 1, rPtr[rNum].doors[dNum].x, '#');
                    mvaddch(i=rPtr[rNum].doors[dNum].y + 1, rPtr[rNum].x - 5, '#');
                    for(i=rPtr[rNum].doors[dNum].x; i>=rPtr[rNum].x -5; i--)
                        mvaddch(rPtr[rNum].doors[dNum].y + 2, i, '#');
                    break;
                case 'w':
                    for(i=rPtr[rNum].doors[dNum].x-1; i>rPtr[rNum].x - 5; i--)
                        mvaddch(rPtr[rNum].doors[dNum].y, i, '#');
                    break;
                case 'e':
                    for(i=rPtr[rNum].doors[dNum].x + 1; i<rPtr[rNum].x + rPtr[rNum].l + 5; i++)
                        mvaddch(rPtr[rNum].doors[dNum].y, i, '#');
                    break;
            } 
            break;
        //print doors for sixth room
        case 5:
            switch(dir)
            {
                case 'n':
                    for(i=rPtr[rNum].doors[dNum].y - 1; i>yAdd; i--)
                        mvaddch(i, rPtr[rNum].doors[dNum].x, '#');
                    break;
                case 's':
                    mvaddch(i=rPtr[rNum].doors[dNum].y + 1, rPtr[rNum].doors[dNum].x, '#');
                    mvaddch(i=rPtr[rNum].doors[dNum].y + 1, rPtr[rNum].x - 5, '#');
                    for(i=rPtr[rNum].doors[dNum].x; i>=rPtr[rNum].x -5; i--)
                        mvaddch(rPtr[rNum].doors[dNum].y + 2, i, '#');
                    break;
                case 'w':
                    for(i=rPtr[rNum].doors[dNum].x-1; i>rPtr[rNum].x - 5; i--)
                        mvaddch(rPtr[rNum].doors[dNum].y, i, '#');
                    break;
                case 'e':
                    mvaddch(rPtr[rNum].doors[dNum].y, rPtr[rNum].doors[dNum].x+1, '#');
                    mvaddch(yAdd, rPtr[rNum].doors[dNum].x+1, '#');
                    for(i=rPtr[rNum].doors[dNum].y; i>yAdd-1; i--)
                        mvaddch(i, rPtr[rNum].doors[dNum].x + 2, '#');
                    break;
            } 
        break;
    }
}
