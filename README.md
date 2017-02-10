# Rogue
Small terminal game made for school assignment through ncurses library

************
Compilation
************

Compile the program using the command:
make

*********
Usage
*********

 - If using the provided level file, run the executable using the command:
./rogue assets/levelFile.txt

 - If using your own level file, place it in the assets folder and use the command:
./rogue assets/"level file"

   where "level file" is the name of your file.

*********
Testing
*********
Tested on a Raspberry Pi 2 Model B by:
A2 Tests:
 - Tested execution with more and less than 1 file to check if the program would exit
 - Tested with a file that does not exist to see if the program would exit
 - Tested with various room sizes to see if the rooms printed properly, and also to
    see if the program exits when the window is not large enough
 - Tested with various gold pieces to see if total was adding up
 - Tested with various weak monsters to see if they were being randomized
 - Tested movement to make sure hero character would overwrite everything except for
    walls and monsters

A3 Tests:
 - Held down the enter key to make sure monster patrols stay within rooms and do not
    pick up items
 - Tested combat to make sure monsters would die, as well as if the hero would die
 - Tested the Aquator to make sure it wouldn't drop attack below 0
 - Tested the value of items printed out at the end to make sure they matched their
    type
 - Tested to make sure enemies could not walk on hero or start combat
 - Tested to make sure walking through hallways was successful and the proper door/
    hallway character was being printed out behind the hero.
 - Tested with various screen sizes to make sure the program would exit if there was
    not enough room for hallways/status bar. 

*****************
Known Limitations
*****************
A2 Limitations:
 - The level file must be formatted in the way specified in the A2 outline. Any 
    changes will most likely result in an error or segmentation fault.
 - The game can only be played on windows that are large enough to fit all the rooms,
    which means screens with lower resolutions cannot play the same levels as screens
    with higher resolution
 - The program can only be played with one level file, and the level file must
    conrtain exactly six rooms/lines.

A3 Limitations:
 - If you try to walk into a wall while standing on a door, the door will not be
    printed out again, a dot will be added instead.

***********
References
***********
- Re-used my checkArg function, my nullFile function and some file parsing code from A1.
- Used Justin's print rectangle and print message function that he provided on moodle.
- Used the singleMove, moveCharacter and canMove functions that Jacob showed me in Lab 5.
- Resused A2 to create this program.
