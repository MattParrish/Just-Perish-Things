///////////////////////////////////////////////
// Matthew Parrish                           //
// CS301, Project 1                          //
// Frogger in Assembly. Assembgger? Frogmbly?//
// main.cpp                                  //
///////////////////////////////////////////////

extern "C" int getMapLine(int);
extern "C" void mapAdvance();
extern "C" char movToon();
extern "C" int getToonY();
extern "C" int getToonX();
extern "C" int getAlive();
extern "C" void setDead();
extern "C" char getStartChar();
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string>
using std::cin;
using std::cout;
using std::endl;

void printMap();
void printTBEdge();
void printRow(int, int, char, char);
void startUp();
void pretendClearer();
void printStartMenu();
void getStartOption(char&);

int main()
{
    char c;
    bool alive = true;

    //KEEP IF CAN'T USE GETCH//
    int getCharOffset = 1;
    ///////////////////////////

    pretendClearer();
    printStartMenu();
    getStartOption(c);

    if(c == 'n' || c == 'N' )
    {
        cout << "Awwww. Kay bye. :(" << endl;
        return 0xD4a7;
    }
    startUp();

    cout << "All right! Here we go!" << endl;
    printMap();
    while(true)
    {
        //USE IF COMPUTER DOES SUPPORT GETCH//
        //c = movToon();                    //
        //mapAdvance();                     //
        //printMap();                       //
        //////////////////////////////////////

        //USE IF COMPUTER DOESN'T SUPPORT GETCH//
        c = movToon();
        if(getCharOffset % 2 == 0)
        {
            pretendClearer();
            mapAdvance();
            printMap();
        }
        /////////////////////////////////////////

        //BEGIN END CHECKING:
        if(c == 'q' || c == 'Q')
        {
            cout << "Quitting.";
            break;
        }

        if(!getAlive())
        {
            cout << "Oh no! You died!" << endl;
            break;
        }

        if(getAlive() && getToonY() == 0)
        {
            cout << "Congratulations! You win!" << endl;
            break;
        }
        //END END CHECKING

        ++getCharOffset;
    }

    return 0xD043;

}

//Pre condition: None
//Post condition: returns an N or Y to the user, capital or lower case
void getStartOption(char &starterValue)
{
    int getCharOffSet = 0;
    while(true)
    {
        starterValue = getStartChar();
        if(starterValue == 'y' || starterValue == 'Y' || starterValue == 'n' || starterValue == 'N')
            break;
        if(getCharOffSet % 2 == 0)
        {
            cout << endl;
            cout << "Please enter a proper character: ";
        }
        getCharOffSet++;
    }
    return;
}

//Pre condition: None
//Post condition: Prints the splash 'art', and prompts for input
void printStartMenu()
{
    cout << "     Welcome,                                          " << endl;
    cout << "           TO!                                         " << endl;
    cout << "                                                       " << endl;
    cout << "     ======                                            " << endl;
    cout << "    xxxxxxxxxxxxxxxx                                   " << endl;
    cout << "    ||====  ====    xxxxxx  ==     ==    ^^^^   ===    " << endl;
    cout << "    ||     ||   \\  ||  ||xxxxxx| ||  || ||--|| ||  \\ " << endl;
    cout << "    ||     ||        ==     ==||xxxxxx| ||__   ||      " << endl;
    cout << "                              ||     |x                " << endl;
    cout << "                           |  ||  |  ||    /|\\        " << endl;
    cout << "                            ==     ==       |          " << endl;
    cout << "         ||  ||   ===   ||===   ||===       |          " << endl;
    cout << "         ||  ||  || ||  ||   |  ||   |   ___/          " << endl;
    cout << "         ||==||  || ||  ||===   ||===                  " << endl;
    cout << "         ||  ||  || ||  ||      ||                     " << endl;
    cout << "         ||  ||   ===   ||      ||                     " << endl;
    cout << endl;
    cout << endl;
    cout << "  Would you like to play? (y/n): ";
    return;
}

//Pre condition: None
//Post condition: 'clears' the output screen
void pretendClearer()
{
    cout << std::string(50, '\n');
    return;
}

//Pre condition: None
//Post condition: Advances the map a random number of turns, to semi-randomize the starting map
void startUp()
{
    srand(time(NULL));
    int shuffle = rand() % 10;
    for(int a = 0; a<shuffle;a++)
        mapAdvance();
    pretendClearer();
    return;
}

//Pre condition: None
//Post condition: Prints the top or bottom row of the drawn map
void printTBEdge()
{
        cout<<" ---------------------------" << endl;
        return;
}

//Pre condition: None
//Post condition: Prints inside and left/right boundries of the map
void printMap()
{
    char water = 247;
    char land = 177;
    char car = 232;
    char log = '=';
    int lineData;

    printTBEdge();
    for(int row = 0; row < 13; row++)
    {
        lineData = getMapLine(row);
        if(row > 6)
            printRow(lineData, row, land, car);
        else if(row < 6 && row != 0)
            printRow(lineData, row, log, water);
        else
            printRow(lineData, row, land, water);
    }
    printTBEdge();
    return;
}

//Pre condition: None
//Post condition: Prints the characters of each line on the map
void printRow(int mapLine, int row, char safe, char deadly)
{
    char playerModel = 165;
    int heighestPowOfTwo = 1<<26; //2^26
    int playerX = -1;
    if(getToonY() == row)
        playerX = getToonX();

    cout << "|";
    for(int a = 0; a < 27; a++)
    {
        if((mapLine & heighestPowOfTwo) > 0)
        {
            if(a == playerX)
                std::cout << playerModel;
            else
                std::cout << safe;
        }
        else
        {
            std::cout << deadly;
            if(a == playerX)
               setDead();
        }

        heighestPowOfTwo /= 2;
    }
    cout << "|" << endl;
    return;
}
