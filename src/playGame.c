/**
 * Connor Campbell (2016)
 * ccampb21@mail.uoguelph.ca
 */

#include "preGame.h"
#include "playGame.h"

void gameLoop(Room* rPtr, int heroY, int heroX, int heroRoom, int statY)
{
    //move cursor to current hero position
    move(heroY, heroX);
    
    Hero* hero;
    hero = initHero(50, 5, 0, 1, 0, rPtr);
    
    int currY;
    int currX;
    int smallGold;
    int bigGold;
    int endGame;
    int i;
    int j;
    char input;
    char lastCh;
    

    printStats(hero->health, hero->attack, hero->pCount, hero->iCount, statY);
    currY     = getcury(stdscr);
    currX     = getcurx(stdscr);
    smallGold = 0;
    bigGold   = 0;
    endGame   = 0;
    lastCh    = '\0';

    input     = getch();
    
    //game only ends when player hits q or they encounter a staircase
    while(input != 'q' && endGame != 1)
    {
        //lastCh used to re-print doors and hallways
        lastCh = singleMove(rPtr, input, currY, currX, lastCh, hero);
        //various actions performed based on what the hero stepped on, or at least tried to
        switch (lastCh)
        {
            case '+':
                break;
            case '*':
                smallGold++;
                break;
            case '8':
                bigGold++;
                break;
            case '!':
                hero->pCount++;
                break;
            case ')':
                hero->items[hero->iCount] = ')';
                addVal(rPtr, hero, &hero->attack); //increase attack      
                break;
            case '(':
                hero->items[hero->iCount] = '(';
                addVal(rPtr, hero, &hero->attack);          
                break;
            case ']':
                hero->items[hero->iCount] = ']';
                addVal(rPtr, hero, &hero->defense); //increase defense
                break;
            //potion use
            case 'p':
                if(hero->pCount > 0)
                {
                    hero->pCount--;
                    hero->health = 50;
                    printMsg("Hero health restored.");
                }
                else
                    printMsg("Out of potions.");
                break;
            //death
            case 'D':
                endGame = 1;
                printMsg("Game Over. (Press any key to continue.)");
                break;
            case '>':
                endGame = 1; //end the game loop because of stairs
                printMsg("You've exited the level. Press any key to continue.");
                break;

        }
        //for loop for moving the monsters
        for (i = 0; i<6; i++)
        {
            for (j = 0; j<10; j++)
            {
                if(rPtr[i].mon[j].isAlive == 1)
                    moveMon(rPtr, rPtr[i].mon[j].type, rPtr[i].mon[j].ypos, rPtr[i].mon[j].xpos, rPtr[i].mon[j].move, i, j);
            }
        }
        //re-print stats and clear event notification after each input
        printStats(hero->health, hero->attack, hero->pCount, hero->iCount, statY);
        currY = getcury(stdscr);
        currX = getcurx(stdscr);
        input = getch();
        move(0,0);
        clrtoeol();  //clear an event notification after the hero moves
        move(currY, currX);
    }
    
    //end ncurses
    endwin();
    int gTotal;
    gTotal = 0;
    
    //loop to calculate random values for each gold collected
    for (i = 1; i <= smallGold; i++)
    {
        gTotal += (rand() % 51);
    } 
    for (i = 1; i <= bigGold; i++)
    {
        gTotal += (rand() % 200) + 50;
    } 
    
    //print out final stats and items
    printf("Final Hero Stats:\n"
            "Health: %d\n"
            "Attack: %d\n"
            "Defense: %d\n"
            "Gold: %d\n"
            "Potions: %d\n"
            "Total number of items gathered: %d\n"
            "", hero->health, hero->attack, hero->defense, gTotal, hero->pCount, hero->iCount);
    for (i = 0; i<hero->iCount; i++)       
    {    
        switch(hero->items[i])
        {
            case ')':
                printf("     %d) Common Weapon, %2d attack\n", i+1, hero->val[i]);
                break;
            case '(':
                printf("     %d) Rare Weapon,   %2d attack\n", i+1, hero->val[i]);
                break;
            case ']':
                printf("     %d) Equipment,     %2d defense\n", i+1, hero->val[i]);
                break;
            
        }
    }
    printf("Thanks for playing!\n");
    free(hero);//free hero struct
}

char singleMove(Room* rPtr, char input, int currY, int currX, char lastCh, Hero* hero)
{
    //move left
    if(input == 'a')
        return encounter(currY, currX -1, lastCh, rPtr, hero);
    
    //move down
    else if(input == 's')
        return encounter(currY +1, currX, lastCh, rPtr, hero);
        
    
    //move right
    else if(input == 'd')
        return encounter(currY, currX + 1, lastCh, rPtr, hero);
        
    
    //move up
    else if(input == 'w')
       return encounter(currY - 1, currX, lastCh, rPtr, hero);
        
    else if(input == 'p')
       return 'p';

    return '\0';
}

char encounter(int y, int x, char lastCh, Room* rPtr, Hero* hero)
{
    char spot;
    spot = canMove(y, x);
    
    int currX;
    int currY;
    int result;
    
    currY  = getcury(stdscr);
    currX  = getcurx(stdscr);
    result = 0;
    
    //a loop that will either move the character or enter a combat phase based on char from canMove
    switch(spot)
    {
        case '.':
            if(lastCh == '+')
                moveCharacter(y,x, '+'); //print a door at last location if it was a door
            else
                moveCharacter(y,x, '.');
            break;
            
        case '#':
            if(lastCh == '+')
                moveCharacter(y,x, '+');
            else
                moveCharacter(y,x, '#');
            return '#';
        
        case ' ':
            return '#'; //do not move if there is a space/print # on current location
        
        case '+':
            if(lastCh == '#')
                moveCharacter(y,x, '#');
            else
                moveCharacter(y,x, '.');
            printMsg("Hero opened door");
            return '+';
            
        case '*':
            if(lastCh == '+')
                moveCharacter(y,x, '+');
            else
                moveCharacter(y,x, '.');
            printMsg("Hero picked up small gold");
            return '*';
        
        case '8':
            if(lastCh == '+')
                moveCharacter(y,x, '+');
            else
                moveCharacter(y,x, '.');
            printMsg("Hero picked up big gold");
            return '8';
        
        case '!':
            if(lastCh == '+')
                moveCharacter(y,x, '+');
            else
                moveCharacter(y,x, '.');
            printMsg("Hero picked up potion");
            return '!';
        
        case ')':
            if(lastCh == '+')
                moveCharacter(y,x, '+');
            else
                moveCharacter(y,x, '.');
            printMsg("Hero picked up common weapon");
            return ')';
        
        case '(':
            if(lastCh == '+')
                moveCharacter(y,x, '+');
            else
                moveCharacter(y,x, '.');
            printMsg("Hero picked up rare weapon");
            return '(';
        
        case ']':
            if(lastCh == '+')
                moveCharacter(y,x, '+');
            else
                moveCharacter(y,x, '.');
            printMsg("Hero picked up equipment");
            return ']';
            
        case '<':
            return '>';
            
        case '>':
            return '>';   
            
        case 'A':
            move(y,x);
            result = exchange(rPtr, hero, 1); //special combat type for Aquator
            move(currY, currX);
            if(result == 1)
            {
                moveCharacter(y,x, '.');
                printDam(hero->attack, 2);
            }
            return 'A'; 
        
        case 'B':
            move(y,x);
            result = exchange(rPtr, hero, 0);//combat phase
            move(currY, currX);
            if(result == 1) //monster death
            {
                moveCharacter(y,x, '.');
                printDam(hero->attack, 3);
            }
            //remove hero from screen and return death character
            else if (result == 2)
            {
                addch('.');
                move(currY, currX);
                getch();
                return 'D';
            }
            return 'B'; 
        
        case 'S':
            move(y,x);
            result = exchange(rPtr, hero, 0);
            move(currY, currX);
            if(result == 1)
            {
                moveCharacter(y,x, '.');
                printDam(hero->attack, 4);
            }
            else if (result == 2)
            {
                addch('.');
                move(currY, currX);
                getch();
                return 'D';
            }
            return 'S';
        
        case 'Z':
            move(y,x);
            result = exchange(rPtr, hero, 0);
            move(currY, currX);
            if(result == 1)
            {
                moveCharacter(y,x, '.');
                printDam(hero->attack, 5);
            }
            else if (result == 2)
            {
                addch('.');
                move(currY, currX);
                getch();
                return 'D';
            }
            return 'Z';
        
        case 'T':
            move(y,x);
            result = exchange(rPtr, hero, 0);
            move(currY, currX);
            if(result == 1)
            {
                moveCharacter(y,x, '.');
                printDam(hero->attack, 6);
            }
            else if (result == 2)
            {
                addch('.');
                move(currY, currX);
                getch();
                return 'D';
            }
            return 'T'; 
    }
    return '.'; //don't do anything special by default
}

void moveCharacter(int updatedY, int updatedX, char spot)
{
    //print a dot in old location and a hero char in new location
    addch(spot);
    mvprintw(updatedY, updatedX, "@");
    move(updatedY, updatedX);
}

char canMove(int toGoY, int toGoX)
{
    int currX;
    int currY;
    char c;
    
    //get current location and save the character that is in the next space
    currX = getcurx(stdscr);
    currY = getcury(stdscr);
    c = (char)mvinch(toGoY, toGoX);
    
    //return cursor
    move(currY, currX);
    return c;
}


void printMsg(const char* msg)
{
    //moves cursor to top of screen to print a message and then returns
    int x, y;
    getyx(stdscr, y, x);
    move(0,0);
    clrtoeol();
    mvprintw(0,0,msg);
    move(y,x);
}

void printStats(int h, int a, int p, int i, int statY)
{
    //moves cursor to top of screen to print a message and then returns
    int x, y;
    getyx(stdscr, y, x);
    move((statY),0);
    clrtoeol();
    printw("Health: %d, Attack: %d, Potions: %d, Inv: %d/5", h, a, p, i);
    move(y,x);
}

Hero* initHero(int h, int a, int d, int p, int i, Room* rPtr)
{
    Hero* tempHero;
    
    //malloc for hero struct, exit program and free memory if it fails
    tempHero = malloc(sizeof(Hero));
    if (!tempHero)
    {
        printf("Malloc failed\n"
                "Exiting program\n");
        free(rPtr);
        exit(7);    
    }
    
    //intialize hero stats
    tempHero->health  = h;
    tempHero->attack  = a;
    tempHero->defense = d;
    tempHero->pCount  = p;
    tempHero->iCount  = i;
    
    return tempHero;
}

int checkRoom(int x, int y, Room* rPtr)
{
    //checks the current coordinates and returns the number of the room that they're
    if (x >= rPtr[0].x && x <= (rPtr[0].x + rPtr[0].l) && y >= rPtr[0].y && y <= (rPtr[0].x + rPtr[0].h))
        return 0;
    else if (x >= rPtr[1].x && x <= (rPtr[1].x + rPtr[1].l) && y >= rPtr[1].y && y <= (rPtr[1].y + rPtr[1].h))
        return 1;
    else if (x >= rPtr[2].x && x <= (rPtr[2].x + rPtr[2].l) && y >= rPtr[2].y && y <= (rPtr[2].y + rPtr[2].h))
        return 2;
    else if (x >= rPtr[3].x && x <= (rPtr[3].x + rPtr[3].l) && y >= rPtr[3].y && y <= (rPtr[3].y + rPtr[3].h))
        return 3;
    else if (x >= rPtr[4].x && x <= (rPtr[4].x + rPtr[4].l) && y >= rPtr[4].y && y <= (rPtr[4].y + rPtr[4].h))
        return 4;
    else if (x >= rPtr[5].x && x <= (rPtr[5].x + rPtr[5].l) && y >= rPtr[5].y && y <= (rPtr[5].y + rPtr[5].h))
        return 5;
    return -1; 
}

void addVal(Room* rPtr, Hero* hero, int* val)
{ 
    int currX;
    int currY;
    int tempRoom;
    
    currY        = getcury(stdscr);
    currX        = getcurx(stdscr);
    tempRoom     = checkRoom(currX, currY, rPtr); //check which room the hero is in

    //simply removes items from the screen if inventory is full
    if(hero->iCount >= 5)
        printMsg("Hero inventory full. The item broke.");
    //adds the value of the item to either the hero's attack or defense
    
    else
    {
        int i;
        for(i = 0; i < 10; i++)
        {
            if(rPtr[tempRoom].contents[i].X == currX && rPtr[tempRoom].contents[i].Y == currY)
            {
                *val += rPtr[tempRoom].contents[i].val;
                hero->val[hero->iCount] = rPtr[tempRoom].contents[i].val;
                hero->iCount++;
            }
        }
    }
}

int exchange(Room* rPtr, Hero* hero, int type)
{
    int currX;
    int currY;
    int tempRoom;
    int i;
    int hSpeed;
    
    currY        = getcury(stdscr);
    currX        = getcurx(stdscr);
    tempRoom     = checkRoom(currX, currY, rPtr);
    
    //scans through monster array to find the current monster
    for(i = 0; i < 10; i++)
    {
        if(rPtr[tempRoom].mon[i].xpos == currX && rPtr[tempRoom].mon[i].ypos == currY && rPtr[tempRoom].mon[i].isAlive == 1)
        {
            //calculate random speed
            hSpeed = (rand() % 10) + 1;
            
            //monster loses health
            if(hSpeed>rPtr[tempRoom].mon[i].speed)
            {
                rPtr[tempRoom].mon[i].health -= hero->attack;
                printDam(hero->attack, 0);
            }
            
            
            else
            {   
                //hero loses health
                if(type == 0)
                {
                    hero->health -= rPtr[tempRoom].mon[i].attack;
                    printDam(rPtr[tempRoom].mon[i].attack, 1);
                }
                //hero loses attack
                else
                {
                    if (hero->attack>0)
                    {
                        hero->attack -= rPtr[tempRoom].mon[i].attack;
                        printDam(rPtr[tempRoom].mon[i].attack, 7);
                    }
                    //attack is already at 0
                    else
                    {
                        hero->attack = 0;
                        printMsg("Aquator attacks, but Hero's attack cannot drop any farther.");
                    }
                }
            }
            
            //monster dies
            if(rPtr[tempRoom].mon[i].health <= 0)
            {
                rPtr[tempRoom].mon[i].isAlive = 0;
                return 1;//special monster dies return
            }
            
            //hero dies
            else if(hero->health <= 0)
            {
                printDam(rPtr[tempRoom].mon[i].attack, 8);
                hero->health = 0;
                return 2;//special hero dies return
            }
            return 0;
        }
    }
    return 0;
}

void printDam(int dmg, int type)
{
    //moves cursor to top of screen to print a message and then returns
    int x, y;
    getyx(stdscr, y, x);
    move(0,0);
    clrtoeol(); //clear the screen for a new message
    //many differnt types of messages that each use an int to deliver a message
    if (type == 0)
        mvprintw(0,0,"Hero caused %d damage", dmg);
    else if(type == 1)
        mvprintw(0,0,"Hero took %d damage", dmg);
    else if(type == 2)
        mvprintw(0,0,"Hero caused %d damage. Hero killed an Aquator.", dmg);
    else if(type == 3)
        mvprintw(0,0,"Hero caused %d damage. Hero killed a Bat.", dmg);
    else if(type == 4)
        mvprintw(0,0,"Hero caused %d damage. Hero killed a Snake.", dmg);
    else if(type == 5)
        mvprintw(0,0,"Hero caused %d damage. Hero killed a Zombie.", dmg);
    else if(type == 6)
        mvprintw(0,0,"Hero caused %d damage. Hero killed a Troll.", dmg);
    else if(type == 7)
        mvprintw(0,0,"Hero lost %d attack", dmg);
    else if(type == 8)
        mvprintw(0,0,"Hero took %d damage. The Hero has died. (Press an key to Conitnue.)", dmg);
    move(y,x);
}

void moveMon(Room* rPtr, char type, int monY, int monX, int move, int i, int j)
{
    int x;
    int y;
    char check;
    
    check = '\0';
    getyx(stdscr, y, x);
    
    //movement algorithm for snake
    if (type == 'S')
    {
        //loops through all the possible types of movement for the monster until it finds a direction it can move
        while(check != '.' && check!= '@')
        {
            switch(move)
            {
                case 1:
                    check = canMove(monY, monX+1);
                    //move the monster if they are trying to move onto a dot
                    if(check == '.')
                    {
                        mvaddch(monY, monX, '.');
                        mvprintw(monY, monX+1, "S");
                        rPtr[i].mon[j].xpos = monX+1;
                        break;
                    }
                    //simply do not move and break out of the loop if they try to move on the hero
                    else if(check == '@')
                        break;
                    //in all other cases, try the next type of movement
                    else
                        move++;
                    break;
                case 2:
                    check = canMove(monY, monX+1);
                    if(check == '.')
                    {
                        mvaddch(monY, monX, '.');
                        mvprintw(monY, monX+1, "S");
                        rPtr[i].mon[j].xpos = monX+1;
                        break;
                    }
                    else if(check == '@')
                        break;
                    else
                        move++;
                    break;
                case 3:
                    check = canMove(monY, monX-1);
                    if(check == '.')
                    {
                        mvaddch(monY, monX, '.');
                        mvprintw(monY, monX-1, "S");
                        rPtr[i].mon[j].xpos = monX-1;
                        break;
                    }
                    else if(check == '@')
                        break;
                    else
                        move++;
                    break;
                case 4:
                    check = canMove(monY, monX-1);
                    if(check == '.')
                    {
                        mvaddch(monY, monX, '.');
                        mvprintw(monY, monX-1, "S");
                        rPtr[i].mon[j].xpos = monX-1;
                        break;
                    }
                    else if(check == '@')
                        break;
                    //reset the movement case back to 0 on the final case
                    else
                    {
                        move=0;
                        move++;
                        break;
                    }
                    
            }
        }
        
        //reset both the struct counter and temp counter after reaching the final movement case
        if(move == 4)
        {
            move = 0;
            rPtr[i].mon[j].move = 0; 
        }
        
        //increase the movement case by one for the next move
        rPtr[i].mon[j].move = move + 1;
    }
    
    //movement algorithm for bat
    else if(type == 'B')
    {
        //first randomly calculate a direction that the bat can for sure move in
        char dir;
        while(check != '.' && check!= '@')
        {
            dir = randDir();
            if (dir == 'e')
                check = canMove(monY, monX + 1);
            else if (dir == 'w')
                check = canMove(monY, monX - 1);
            else if (dir == 's')
                check = canMove(monY + 1, monX);
            else if (dir == 'n')
                check = canMove(monY - 1, monX);
        }
        //different movement based on the char returned from check
        switch(dir)
            {
                case 'e':
                    if(check == '.')
                    {
                        mvaddch(monY, monX, '.');
                        mvprintw(monY, monX+1, "B");
                        rPtr[i].mon[j].xpos = monX+1;
                        break;
                    }
                    else if(check == '@')
                        break; //don't move at all if there is a hero
                case 'w':
                    if(check == '.')
                    {
                        mvaddch(monY, monX, '.');
                        mvprintw(monY, monX-1, "B");
                        rPtr[i].mon[j].xpos = monX-1;
                        break;
                    }
                    else if(check == '@')
                        break;
                case 's':
                    if(check == '.')
                    {
                        mvaddch(monY, monX, '.');
                        mvprintw(monY+1, monX, "B");
                        rPtr[i].mon[j].ypos = monY+1;
                        break;
                    }
                    else if(check == '@')
                        break;
                case 'n':
                    if(check == '.')
                    {
                        mvaddch(monY, monX, '.');
                        mvprintw(monY-1, monX, "B");
                        rPtr[i].mon[j].ypos = monY-1;
                        break;
                    }
                    else if(check == '@')
                        break;      
            }
        
    }
    
    //movement algortihtm for zombie
    else if(type == 'Z')
    {
        while(check != '.' && check!= '@')
        {
            switch(move)
            {
                case 1:
                    check = canMove(monY+1, monX);
                    if(check == '.')
                    {
                        mvaddch(monY, monX, '.');
                        mvprintw(monY+1, monX, "Z");
                        rPtr[i].mon[j].ypos = monY+1;
                        break;
                    }
                    else if(check == '@')
                        break;
                    else
                        move++;
                    break;
                case 2:
                    check = canMove(monY-1, monX);
                    if(check == '.')
                    {
                        mvaddch(monY, monX, '.');
                        mvprintw(monY-1, monX, "Z");
                        rPtr[i].mon[j].ypos = monY-1;
                        break;
                    }
                    else if(check == '@')
                        break;
                    else
                    {
                        move=0;
                        move++;
                        break;
                    }       
            }
        }
        if(move == 2)
        {
            move = 0;
            rPtr[i].mon[j].move = 0;
        }
        
        rPtr[i].mon[j].move = move + 1; 
    }
    
    //movement algortihtm for troll
    else if (type == 'T')
    {
        while(check != '.' && check!= '@')
        {
            switch(move)
            {
                case 1:
                    check = canMove(monY+1, monX);
                    if(check == '.')
                    {
                        mvaddch(monY, monX, '.');
                        mvprintw(monY+1, monX, "T");
                        rPtr[i].mon[j].ypos = monY+1;
                        break;
                    }
                    else if(check == '@')
                        break;
                    else
                        move++;
                    break;
                case 2:
                    check = canMove(monY, monX+1);
                    if(check == '.')
                    {
                        mvaddch(monY, monX, '.');
                        mvprintw(monY, monX+1, "T");
                        rPtr[i].mon[j].xpos = monX+1;
                        break;
                    }
                    else if(check == '@')
                        break;
                    else
                        move++;
                    break;
                case 3:
                    check = canMove(monY-1, monX);
                    if(check == '.')
                    {
                        mvaddch(monY, monX, '.');
                        mvprintw(monY-1, monX, "T");
                        rPtr[i].mon[j].ypos = monY-1;
                        break;
                    }
                    else if(check == '@')
                        break;
                    else
                        move++;
                    break;
                case 4:
                    check = canMove(monY, monX-1);
                    if(check == '.')
                    {
                        mvaddch(monY, monX, '.');
                        mvprintw(monY, monX-1, "T");
                        rPtr[i].mon[j].xpos = monX-1;
                        break;
                    }
                    else if(check == '@')
                        break;
                    else
                    {
                        move=0;
                        move++;
                        break;
                    }
                    
            }
        }
        if(move == 4)
        {
            move = 0;
            rPtr[i].mon[j].move = 0;
        }
        
        rPtr[i].mon[j].move = move + 1;
    }
    move(y,x);
}

char randDir()
{
    //randomly generates a direction for the bat to move in
    int num;
    num = (rand() % 4) + 1;
    
    if (num == 1)
        return 'e';
    else if(num == 2)
        return 'w';
    else if(num == 3)
        return 's';
    else if(num == 4)
        return 'n';
  return 's';
}
