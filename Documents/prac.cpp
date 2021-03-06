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
	{},
	{},
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
			{0, 1}, {-1, 0}, {0, 0}, {0, -1}
		}
	},	//T
	{	//O
		{	//rot = 0
			{0, 1}, {1, 1}, {0, 0}, {1, 0}
		},
		{	//rot = Right = 1
			{0, 1}, {1, 1}, {0, 0}, {1, 0}
		},
		{	//rot = 2
			{0, 1}, {1, 1}, {0, 0}, {1, 0}
		},
		{	//rot = Left = 3
			{0, 1}, {1, 1}, {0, 0}, {1, 0}
		}
	},	//O
	{},
	{},
	{},
	{	//S
		{	//rot = 0
			{0, 1}, {1, 1}, {-1, 0}, {0, 0}
		},
		{	//rot = Right = 1
			{0, 1}, {0, 0}, {1, 0}, {1, -1}
		},
		{	//rot = 2
			{0, 0}, {1, 0}, {-1, -1}, {0, -1}
		},
		{	//rot = Left = 3
			{-1, 1}, {-1, 0}, {0, 0}, {0, -1}
		}
	},	//S
	{	//I
		{	//rot = 0
			{-1, 0}, {0, 0}, {1, 0}, {2, 0}
		},
		{	//rot = Right = 1
			{1, 1}, {1, 0}, {1, -1}, {1, -2}
		},
		{	//rot = 2
			{-1, -1}, {0, -1}, {1, -1}, {2, -1}
		},
		{	//rot = Left = 3
			{-1, 1}, {-1, 0}, {-1, -1}, {-1, -2}
		}
	},	//I
	{	//L
		{	//rot = 0
			{-1, 0}, {0, 0}, {1, 0}, {1, 1}
		},
		{	//rot = Right = 1
			{0, 1}, {0, 0}, {0, -1}, {1, -1}
		},
		{	//rot = 2
			{-1, 0}, {0, 0}, {1, 0}, {-1, -1}
		},
		{	//rot = Left = 3
			{-1, 1}, {0, 1}, {0, 0}, {0, -1}
		}
	},	//L
};

//블럭있는 보드 좌표 (18,1)부터 (36,20)까지 x +2, y +1 증가시켜 이용    
//mapData는 보드 아래부터 인덱스 시작

void gotoxy(int, int);
void init();
void title();
void start();
void fillNextBlock();
void play_menu();
void draw_frame();
void game_over();
void exit_game();
void player_move(int);
void draw_board(bool);


typedef struct BlockData {
    int type;
    int rot = 0;
};

int x = 26, y = 1;
int key;
int cnt = 0;
bool fall = false;
int score = 0;               // 점수
vector<pair<vector<bool>, int>> mapData;
vector<bool> emptyLine(10, false);	//vector constructor 중 하나 : 변수명(개수, 초기화할 데이터);
deque<BlockData> nextBlock;
BlockData fallingBlock;
BlockData holdBlock;

int main() {
    int ch;
    float ti;
    clock_t st, end;
    st = clock();

    init();
    title();
    start();

    while (1) {                       ///////     게임 중 모든 클릭을 담당
        if (_kbhit()) {
            ch = _getch();
            if (ch == ESC) {     //메뉴
                play_menu();
            }
            else if (ch == 224) {                       //방향키 입력
                ch = _getch();
                player_move(ch);               //방향키는 224->n으로 입력되어 space는 따로 구현
            }
            else if (ch == SPACE) {
                while (true) {
                    player_move(DOWN);
                    if (x == 26 && y == 1)
                        break;
                }
            }
                                                //@todo: 홀드 구현시 shift 추가 

        }

        for (int i = 0; i < 20; i++) {          //다 채워지면 한 줄 지우기
            if (mapData[i].second == 10) {
                mapData.erase(mapData.begin() + i);
                mapData.push_back(make_pair(emptyLine, 0));
                draw_board(0);                   //전체 다시 그리기
                score += 20;                //줄 지워서 점수 획득
            }
        }
        
        end = clock();
        ti = ((float)(end - st) / CLOCKS_PER_SEC);
        if (ti >= 1) {
            player_move(DOWN);
            st = clock(); 
        }
    }

    return 0;
}

void gotoxy(int a, int b) {
    COORD CurPos = { a, b };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CurPos);
}

void init() {
    for (int i = 0; i < 23; i++) {      //mapData 생성
        mapData.push_back(make_pair(emptyLine, 0));	//make_pair : pair를 만들어 return하는 함수
    }

    srand((unsigned int)time(0));

    fillNextBlock();
    fillNextBlock();

    system("mode con cols=60 lines=25 | title TETRIS SproutThon");

    ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL);    //고속 cout을 위한 코드

    CONSOLE_CURSOR_INFO curInfo;                        //커서를 숨기는 코드
    curInfo.dwSize = 1;
    curInfo.bVisible = true;                           //false이면 invisible, true이면 visible
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void title() {
    cout << "\n\n\n\n\n";
    cout << "    ■■■   ■■■   ■■■   ■■■   ■■■   ■■■\n";
    cout << "      ■     ■         ■     ■  ■     ■     ■\n";
    cout << "      ■     ■■■     ■     ■■■     ■     ■■■\n";
    cout << "      ■     ■         ■     ■ ■      ■         ■\n";
    cout << "      ■     ■■■     ■     ■  ■   ■■■   ■■■\n\n\n\n";
    cout << "                      ◀ ▶ : MOVE\n";
    cout << "                        ▼  : SOFT DROP\n";
    cout << "                      SPACE : HARD DROP\n\n\n";
    cout << "                       ESC  : MENU\n\n";
    cout << "                   PRESS ANY KEY TO START";

    while (1) {
        if (_kbhit()) {
            key = _getch();
            break;
        }
    }
}

void fillNextBlock() {
    vector<pair<int, int>> random;
    for (int i = 0; i < 7; i++)
        random.push_back(make_pair(i, rand()));

    sort(random.begin(), random.end(), [](auto a, auto b) {
        if (a.second > b.second) return true;
        else return false;
        });

    for (int i = 0; i < 7; i++) {
        BlockData tmp;
        tmp.type = random[i].first;
        tmp.rot = 0;
        nextBlock.push_back(tmp);
    }
}

void draw_board(bool tf) {            //@todo: 보드를 다시 그릴 때 이동중인 블럭이 지워지지 않는지 확인
        //true: 쌓아서 그리기만 false: 전체 다시 그리기
    if (tf) {
        for (int i = 1; i <= FR_SIZE_HEIGHT; i++) {
            for (int j = 0; j < FR_SIZE_WIDTH; j++) {
                if (mapData[20 - i].first[j]) {
                    gotoxy(18 + 2 * j, i);
                    cout << BLOCK_MARK;
                }
            }
        }
    }
    else {
        for (int i = 1; i <= FR_SIZE_HEIGHT; i++) {
            gotoxy(18, i);
            for (int j = 0; j < FR_SIZE_WIDTH; j++) {
                if (mapData[20 - i].first[j])
                    cout << BLOCK_MARK;
                else
                    cout << NONE_MARK;
            }
        }
    }
}

void player_move(int ch) {       //*다 쌓이면 game_over로 이동
    int a = x;
    int b = y;

    cout << "\b\b";                    
    cout << NONE_MARK;

    //@todo: mapData 비교로 못 움직이게 추가
    //@todo: up으로 회전 추가

    switch (ch) {
    case UP:
        break;
    case DOWN:
        y++;
        fall = true;
        if (y > 20)
            y = 20;
        break;
    case LEFT:
        x -= 2;
        if (x < 18)
            x = 18;
        break;
    case RIGHT:
        x += 2;
        if (x > 36)
            x = 36;
        break;
    }

    if (mapData[20 - y].first[(x - 18) / 2])          //@todo: mapData 비교로 못 움직이게 추가
        x = a, y = b;

    if (x == a && y == b && fall) { //움직이지 못했을 경우
        cnt++;
        fall = false;
    }
       
    if (cnt == 3) {
        cnt = 0;
        fall = false;
        mapData[20 - y].first[(x - 18)/2] = 1;
        mapData[20 - y].second += 1;
        draw_board(1);
        x = 26, y = 1;
        score += 10;                //블럭 쌓아서 점수 획득
    }

    gotoxy(x, y);

    //draw_block();
    cout << BLOCK_MARK;        //처음 플레이어 위치 표시용 (플레이어 움직임 구현 시연용)
                               //@todo: 옮길 수 있는 것인지에 대한 체크가 선행되어야 함
                                //@todo: draw_block으로 블럭 그리기를 할 예정
}


void draw_frame() {         //게임 화면 틀
    system("cls");
    x = 26, y = 1;           //좌표 초기화

    for (int i = 0; i < 8; i++)
        cout << NONE_MARK;
    for (int i = 0; i < FR_SIZE_WIDTH + 2; i++)
        cout << WALL_MARK;

    cout << "\n";

    for (int i = 0; i < FR_SIZE_HEIGHT; i++) {
        for (int j = 0; j < 8; j++)
            cout << NONE_MARK;

        cout << WALL_MARK;                          //왼쪽 벽
        for (int j = 0; j < FR_SIZE_WIDTH; j++) {
            if (mapData[i].first[j])
                cout << BLOCK_MARK;
            else
                cout << NONE_MARK;
        }
        cout << WALL_MARK;                        //오른쪽 벽

        cout << "\n";
    }

    for (int i = 0; i < 8; i++)
        cout << NONE_MARK;
    for (int i = 0; i < FR_SIZE_WIDTH + 2; i++)
        cout << WALL_MARK;

    gotoxy(x, y);
    cout << BLOCK_MARK;                 //처음 플레이어 위치 표시용 (플레이어 움직임 시연용)


}

void start() {
    system("cls");
    score = 0; //점수 초기화

    cout << "\n\n\n\n\n";
    cout << "                            ■■\n";
    cout << "                          ■   ■\n";
    cout << "                             ■\n";
    cout << "                          ■   ■\n";
    cout << "                            ■■\n\n\n\n";
    Sleep(500);

    system("cls");
    cout << "\n\n\n\n\n";
    cout << "                            ■■\n";
    cout << "                          ■   ■\n";
    cout << "                             ■\n";
    cout << "                            ■\n";
    cout << "                          ■■■■\n\n\n\n";
    Sleep(500);

    system("cls");
    cout << "\n\n\n\n\n";
    cout << "                             ■\n";
    cout << "                           ■■\n";
    cout << "                             ■\n";
    cout << "                             ■\n";
    cout << "                           ■■■\n\n\n\n";
    Sleep(500);

    draw_frame();
}

void play_menu() {
    char ch = 0;
    system("cls");
    cout << "\n\n\n\n\n";
    cout << "           ■  ■     ■■■   ■   ■   ■    ■\n";
    cout << "          ■ ■ ■    ■       ■■ ■   ■    ■\n";
    cout << "         ■  ■  ■   ■■■   ■ ■■   ■    ■\n";
    cout << "         ■      ■   ■       ■   ■    ■  ■\n";
    cout << "         ■      ■   ■■■   ■   ■     ■■\n\n\n\n";
    cout << "                  r: RESUME     q: EXIT";

    while (ch != 'r' && ch != 'q') {
        ch = _getch();
    }
    if (ch == 'r') {            //게임 진행
        x = 26;    y = 1;             //좌표 초기화         
        draw_frame();
    }
    else if (ch == 'q')
        game_over();       //q 게임 종료
}

void exit_game() {
    system("cls");
    cout << "\n\n\n\n\n";
    cout << "                ■■■    ■  ■   ■■■\n";
    cout << "                ■   ■   ■  ■   ■\n";
    cout << "                ■■■     ■■    ■■■\n";
    cout << "                ■   ■     ■     ■\n";
    cout << "                ■■■      ■     ■■■\n\n\n\n";
    exit(0);
}

void game_over() {
    char ask = 0;
    system("cls");
    cout << "\n\n\n\n\n";
    cout << "  ■■■   ■   ■  ■ ■■■    ■■  ■  ■ ■■■ ■■■\n";
    cout << " ■      ■  ■ ■■■ ■       ■  ■ ■  ■ ■     ■  ■\n";
    cout << " ■ ■■ ■■■ ■  ■ ■■■   ■  ■ ■  ■ ■■■ ■■■\n";
    cout << " ■   ■ ■  ■ ■  ■ ■       ■  ■  ■■  ■     ■ ■\n";
    cout << "  ■■■ ■  ■ ■  ■ ■■■    ■■    ■   ■■■ ■  ■\n\n\n\n";
    cout << "                         SCORE: " << score << "\n\n";
    cout << "                    Continue? ( y / n )";

    while (ask != 'y' && ask != 'n' && ask != ESC) {
        ask = _getch();
    }
    if (ask == 'y') {

        // mapData 초기화
        mapData.clear();
        for (int i = 0; i < 23; i++) {
            mapData.push_back(make_pair(emptyLine, 0));
        }

        start();
    }
    else if (ask == 'n' || ask == 27) {
        exit_game();
    }
}