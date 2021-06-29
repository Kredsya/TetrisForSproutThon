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
const string WALL_MARK = "��";
const string BLOCK_MARK = "��";

const int FR_SIZE_WIDTH = 10;
const int FR_SIZE_HEIGHT = 20;

const int UP = 72;
const int DOWN = 80;
const int LEFT = 75;
const int RIGHT = 77;
const int SPACE = 32;
const int ESC = 27;

//@todo: �۾����Դϴ�
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

//���ִ� ���� ��ǥ (18,1)���� (36,20)���� x +2, y +1 �������� �̿�    
//mapData�� ���� �Ʒ����� �ε��� ����

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
int score = 0;               // ����
vector<pair<vector<bool>, int>> mapData;
vector<bool> emptyLine(10, false);	//vector constructor �� �ϳ� : ������(����, �ʱ�ȭ�� ������);
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

    while (1) {                       ///////     ���� �� ��� Ŭ���� ���
        if (_kbhit()) {
            ch = _getch();
            if (ch == ESC) {     //�޴�
                play_menu();
            }
            else if (ch == 224) {                       //����Ű �Է�
                ch = _getch();
                player_move(ch);               //����Ű�� 224->n���� �ԷµǾ� space�� ���� ����
            }
            else if (ch == SPACE) {
                while (true) {
                    player_move(DOWN);
                    if (x == 26 && y == 1)
                        break;
                }
            }
                                                //@todo: Ȧ�� ������ shift �߰� 

        }

        for (int i = 0; i < 20; i++) {          //�� ä������ �� �� �����
            if (mapData[i].second == 10) {
                mapData.erase(mapData.begin() + i);
                mapData.push_back(make_pair(emptyLine, 0));
                draw_board(0);                   //��ü �ٽ� �׸���
                score += 20;                //�� ������ ���� ȹ��
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
    for (int i = 0; i < 23; i++) {      //mapData ����
        mapData.push_back(make_pair(emptyLine, 0));	//make_pair : pair�� ����� return�ϴ� �Լ�
    }

    srand((unsigned int)time(0));

    fillNextBlock();
    fillNextBlock();

    system("mode con cols=60 lines=25 | title TETRIS SproutThon");

    ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL);    //��� cout�� ���� �ڵ�

    CONSOLE_CURSOR_INFO curInfo;                        //Ŀ���� ����� �ڵ�
    curInfo.dwSize = 1;
    curInfo.bVisible = true;                           //false�̸� invisible, true�̸� visible
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void title() {
    cout << "\n\n\n\n\n";
    cout << "    ����   ����   ����   ����   ����   ����\n";
    cout << "      ��     ��         ��     ��  ��     ��     ��\n";
    cout << "      ��     ����     ��     ����     ��     ����\n";
    cout << "      ��     ��         ��     �� ��      ��         ��\n";
    cout << "      ��     ����     ��     ��  ��   ����   ����\n\n\n\n";
    cout << "                      �� �� : MOVE\n";
    cout << "                        ��  : SOFT DROP\n";
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

void draw_board(bool tf) {            //@todo: ���带 �ٽ� �׸� �� �̵����� ���� �������� �ʴ��� Ȯ��
        //true: �׾Ƽ� �׸��⸸ false: ��ü �ٽ� �׸���
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

void player_move(int ch) {       //*�� ���̸� game_over�� �̵�
    int a = x;
    int b = y;

    cout << "\b\b";                    
    cout << NONE_MARK;

    //@todo: mapData �񱳷� �� �����̰� �߰�
    //@todo: up���� ȸ�� �߰�

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

    if (mapData[20 - y].first[(x - 18) / 2])          //@todo: mapData �񱳷� �� �����̰� �߰�
        x = a, y = b;

    if (x == a && y == b && fall) { //�������� ������ ���
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
        score += 10;                //�� �׾Ƽ� ���� ȹ��
    }

    gotoxy(x, y);

    //draw_block();
    cout << BLOCK_MARK;        //ó�� �÷��̾� ��ġ ǥ�ÿ� (�÷��̾� ������ ���� �ÿ���)
                               //@todo: �ű� �� �ִ� �������� ���� üũ�� ����Ǿ�� ��
                                //@todo: draw_block���� �� �׸��⸦ �� ����
}


void draw_frame() {         //���� ȭ�� Ʋ
    system("cls");
    x = 26, y = 1;           //��ǥ �ʱ�ȭ

    for (int i = 0; i < 8; i++)
        cout << NONE_MARK;
    for (int i = 0; i < FR_SIZE_WIDTH + 2; i++)
        cout << WALL_MARK;

    cout << "\n";

    for (int i = 0; i < FR_SIZE_HEIGHT; i++) {
        for (int j = 0; j < 8; j++)
            cout << NONE_MARK;

        cout << WALL_MARK;                          //���� ��
        for (int j = 0; j < FR_SIZE_WIDTH; j++) {
            if (mapData[i].first[j])
                cout << BLOCK_MARK;
            else
                cout << NONE_MARK;
        }
        cout << WALL_MARK;                        //������ ��

        cout << "\n";
    }

    for (int i = 0; i < 8; i++)
        cout << NONE_MARK;
    for (int i = 0; i < FR_SIZE_WIDTH + 2; i++)
        cout << WALL_MARK;

    gotoxy(x, y);
    cout << BLOCK_MARK;                 //ó�� �÷��̾� ��ġ ǥ�ÿ� (�÷��̾� ������ �ÿ���)


}

void start() {
    system("cls");
    score = 0; //���� �ʱ�ȭ

    cout << "\n\n\n\n\n";
    cout << "                            ���\n";
    cout << "                          ��   ��\n";
    cout << "                             ��\n";
    cout << "                          ��   ��\n";
    cout << "                            ���\n\n\n\n";
    Sleep(500);

    system("cls");
    cout << "\n\n\n\n\n";
    cout << "                            ���\n";
    cout << "                          ��   ��\n";
    cout << "                             ��\n";
    cout << "                            ��\n";
    cout << "                          �����\n\n\n\n";
    Sleep(500);

    system("cls");
    cout << "\n\n\n\n\n";
    cout << "                             ��\n";
    cout << "                           ���\n";
    cout << "                             ��\n";
    cout << "                             ��\n";
    cout << "                           ����\n\n\n\n";
    Sleep(500);

    draw_frame();
}

void play_menu() {
    char ch = 0;
    system("cls");
    cout << "\n\n\n\n\n";
    cout << "           ��  ��     ����   ��   ��   ��    ��\n";
    cout << "          �� �� ��    ��       ��� ��   ��    ��\n";
    cout << "         ��  ��  ��   ����   �� ���   ��    ��\n";
    cout << "         ��      ��   ��       ��   ��    ��  ��\n";
    cout << "         ��      ��   ����   ��   ��     ���\n\n\n\n";
    cout << "                  r: RESUME     q: EXIT";

    while (ch != 'r' && ch != 'q') {
        ch = _getch();
    }
    if (ch == 'r') {            //���� ����
        x = 26;    y = 1;             //��ǥ �ʱ�ȭ         
        draw_frame();
    }
    else if (ch == 'q')
        game_over();       //q ���� ����
}

void exit_game() {
    system("cls");
    cout << "\n\n\n\n\n";
    cout << "                ����    ��  ��   ����\n";
    cout << "                ��   ��   ��  ��   ��\n";
    cout << "                ����     ���    ����\n";
    cout << "                ��   ��     ��     ��\n";
    cout << "                ����      ��     ����\n\n\n\n";
    exit(0);
}

void game_over() {
    char ask = 0;
    system("cls");
    cout << "\n\n\n\n\n";
    cout << "  ����   ��   ��  �� ����    ���  ��  �� ���� ����\n";
    cout << " ��      ��  �� ���� ��       ��  �� ��  �� ��     ��  ��\n";
    cout << " �� ��� ���� ��  �� ����   ��  �� ��  �� ���� ����\n";
    cout << " ��   �� ��  �� ��  �� ��       ��  ��  ���  ��     �� ��\n";
    cout << "  ���� ��  �� ��  �� ����    ���    ��   ���� ��  ��\n\n\n\n";
    cout << "                         SCORE: " << score << "\n\n";
    cout << "                    Continue? ( y / n )";

    while (ask != 'y' && ask != 'n' && ask != ESC) {
        ask = _getch();
    }
    if (ask == 'y') {

        // mapData �ʱ�ȭ
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