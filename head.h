#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <utility>
#include <algorithm>

using namespace std;

#define curColor(colorCode) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
#define NORMAL 15 //NORMAL
#define J 1
#define Z 4
#define T 5
#define O 6
#define S 10
#define I 11
#define L 12

const string NONE_MARK = "  ";
const string WALL_MARK = "■";
const string BLOCK_MARK = "□";

const int FR_SIZE_WIDTH = 10;
const int FR_SIZE_HEIGHT = 20;

const int UP = 72;
const int DOWN = 80;
const int LEFT = 75;
const int RIGHT = 77;
const int SPACE = 32;
const int ESC = 27;

//@todo: 작업중입니다
int blockShape[13][4][4][2] = {	// [BlockData.type][BlockData.rot][number of blocks][delta location]
	{},
	{	//J
		{	//rot = 0
			{-1, 1}, {-1, 0}, {0, 0}, {1, 0}
		},
		{	//rot = Right = 1
			{1, 1}, {0, 1}, {0, 0}, {0, -1}
		},
		{	//rot = 2
			{-1, 0}, {0, 0}, {1, 0}, {1, -1}
		},
		{	//rot = Left = 3
			{-1, -1}, {0, -1}, {0, 0}, {0, 1}
		}
	},	//J
	{	//Z
		{	//rot = 0
			{-1, 1}, {0, 1}, {0, 0}, {1, 0}
		},
		{	//rot = Right = 1
			{1, 1}, {0, 0}, {1, 0}, {0, -1}
		},
		{	//rot = 2
			{-1, 0}, {0, 0}, {0, -1}, {1, -1}
		},
		{	//rot = Left = 3
			{0, 1}, {-1, 0}, {0, 0}, {-1, -1}
		}
	},	//Z
	{	//T
		{	//rot = 0
			{0, 1}, {-1, 0}, {0, 0}, {1, 0}
		},
		{	//rot = Right = 1
			{0, 1}, {0, 0}, {0, 1}, {0, -1}
		},
		{	//rot = 2
			{-1, 0}, {0, 0}, {1, 0}, {0, -1}
		},
		{	//rot = Left = 3
			{0, 1}, {0, -1}, {0, 0}, {0, 1}
		}
	},	//T
};