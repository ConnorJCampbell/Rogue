/**
 * Connor Campbell (2016)
 * ccampb21@mail.uoguelph.ca
 */

#include "preGame.h"

void checkArg(int numArg, char* argNames[])
{
    int i;
    
    //loop prints out file names
    printf("Number of files entered: %d\n\n", numArg - 1);
    for (i = 1; i<numArg; i++)
    {
        printf("File #%d: %s\n\n", i, argNames[i]);
    }
    printf("(Press any key to continue...)\n");
    getchar();
    
    //condition to check if program ran with correct # of arguements
    if (numArg != 2)
    {
        printf("Incorrect number of arguments\n"
                "Exiting program.\n");
        exit(1);
    }
}

void nullFile(FILE* fptr, char* fName)
{
    //checking if files in arguement exist
    if (fptr == NULL)
    {
        printf("Error occured when accessing %s\n"
                "Scanning in level data was unnsuccessful.\n"
                "Exiting program.\n", fName);
        exit(2);
    }   
}

Room* scanLevel(char* file, int* yAdd) {
    
    //open level file and check if it exists
    FILE* levelPtr;
    levelPtr = fopen(file, "r");
    nullFile(levelPtr, file);
    
    //malloc the room pointer for 6 rooms, check if malloc was succesful
    Room* tempRooms;
    tempRooms = malloc(sizeof(Room) * 6);
    if (!tempRooms)
    {
        printf("Malloc failed\n"
                "Exiting program\n");
        fclose(levelPtr);
        exit(2);    
    }
    
    char line[256];
    char* element;
    char* temp; //temporarily stores type and y coordinate for an element
    int xAdd;
    //counters
    int rCount;
    int dCount;
    int iCount;
    int i;
    
    xAdd = 0;
    rCount = 0;
    dCount = 0;
    iCount = 0;
    
    srand(time(NULL));
    
    //loop that reads through the whole level file
    while (fgets(line, 256, levelPtr))
    {
        
        
        char* chunk1;
        char* chunk2;
        char* dimensions;
        //reset counters for doors and items in each new room
        dCount = 0;
        iCount = 0;
        
        chunk1 = strtok(line, "\0");
        dimensions = strtok(chunk1, " ");
        chunk2 = strtok(NULL, "\0"); //stores the rest of the line
        tempRooms[rCount].h = atoi(strtok(dimensions, "X"));
        tempRooms[rCount].l = atoi(strtok(NULL, "X"));
        
        //this last section uses the height and length to assign x and y
        if (rCount == 3)
            xAdd = 0;
        
        //assign y coordinate for first 3 rooms
        if (rCount < 3) 
        {
            tempRooms[rCount].y = 4;
            if (tempRooms[rCount].h > *yAdd) 
                *yAdd = (tempRooms[rCount].h); //yAdd is the largest height
        }
        
        //y coordinate for last 3 rooms
        else if (rCount > 2)
            tempRooms[rCount].y = (9 + *yAdd);
        
        //all x coordinates are assigned the same way
        tempRooms[rCount].x = (3 + xAdd);
        xAdd += (tempRooms[rCount].l + 9);
        
        //while there are more elements in the line
        while(chunk2 != NULL)
        {
            element = strtok(chunk2, " ");
            chunk2 = strtok(NULL, "\0"); //stores the rest of the line
            
            //store element as a door
            if(element[0] == 'd')
            {
                tempRooms[rCount].doors[dCount].direction = element[1];
                strtok(element, &element[1]);
                tempRooms[rCount].doors[dCount].spaces    = atoi(strtok(NULL, " "));
                
                dCount++;
            }
            //store element as an item
            else
            {
                tempRooms[rCount].contents[iCount].type = element[0];
                
                temp = strtok(element, ",");
                //special for loop for y coordinate
                for (i = 0; i < strlen(temp) - 1; i++)
                {
                    temp[i] = temp[i+1];
                }
                temp[strlen(temp) - 1] = '\0'; //add null character
    
                tempRooms[rCount].contents[iCount].y = atoi(temp);
                tempRooms[rCount].contents[iCount].x = atoi(strtok(NULL, " "));

                //assigning values to collectables
                if(tempRooms[rCount].contents[iCount].type == 'w')
                    tempRooms[rCount].contents[iCount].val = (rand() % 9) + 1;
                else if(tempRooms[rCount].contents[iCount].type == 'W')
                    tempRooms[rCount].contents[iCount].val = (rand() % 5) + 10;
                else if(tempRooms[rCount].contents[iCount].type == 'e')
                    tempRooms[rCount].contents[iCount].val = (rand() % 21);
                
                //assigning overall item locations
                tempRooms[rCount].contents[iCount].X = tempRooms[rCount].x + tempRooms[rCount].contents[iCount].x;
                tempRooms[rCount].contents[iCount].Y = tempRooms[rCount].y + tempRooms[rCount].contents[iCount].y;

                iCount++;   
            }
        }
        rCount++; //point to the next room
    }
    //close file and return the freshly parsed room info
    fclose(levelPtr);
    return tempRooms;

}
