#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>

#include <iostream>
#include <string>
#include <vector>
#include <deque>

using namespace std;

#define curColor(colorCode) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
#define NORMAL 15 //NORMAL
#define J 1
#define S 10
#define I 11
#define Z 4
#define T 5
#define O 6
#define L 12

const string NONE_MARK  = "  ";
const string BLOCK_MARK = "бс";
const string WALL_MARK  = "бр";

const int FR_SIZE_WIDTH  = 10;
const int FR_SIZE_HEIGHT = 20;

const int DOWN	= 80;
const int LEFT	= 75;
const int RIGHT = 77;
const int SPACE = 32;
const int ESC	= 27;