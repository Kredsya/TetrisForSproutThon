#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>

#include <iostream>
#include <string>
#include <vector>
#include <deque>

using namespace std;

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