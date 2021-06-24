//블럭있는 틀? (18,1)부터 (36,20)까지 x +2, y +1 증가시켜 이용    

#include "head.h"

void gotoxy(int, int);
void init();
void title();
void start();
void play_menu();
void draw_frame();
void game_over();
void exit_game();
void player_move(int);

int x = 26, y = 1;           
int key;
bool block[20][10];
int score = 0;               // 점수

int main() {
    int ch;
    init();
    title();
    start();

    while (1) {                       ///////     게임 중 모든 클릭을 담당
        if (_kbhit()) {
            ch = _getch();
            if (ch == ESC) {     //메뉴
                play_menu();
            }
            else if (ch == 224) {                       //*방향키 입력
                ch = _getch();
                player_move(ch);
            }
            //else if (ch == SPACE)                 @todo: 하드드롭 구현시 사용 
                                                        
                                                //@todo: 홀드 구현시 shift 추가 

        }
    }

    return 0;
}

void gotoxy(int a, int b) {
    COORD CurPos = { a, b };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CurPos);
}

void init() {
    system("mode con cols=60 lines=25 | title TETRIS SproutThon");
    
    ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL);    //고속 cout을 위한 코드

    CONSOLE_CURSOR_INFO curInfo;                        //커서를 숨기는 코드
    curInfo.dwSize = 1;
    curInfo.bVisible = false;                           //false이면 invisible, true이면 visible
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

void player_move(int ch) {       //*다 쌓이면 game_over로 이동

    cout << "\b\b";
    cout << NONE_MARK;
    
    //@todo: block배열 비교로 못 움직이게 추가
    //@todo: space로 hard drop 추가
    switch (ch) {                      
    case DOWN:
        y++;
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

    gotoxy(x, y);
    cout << BLOCK_MARK;        //처음 플레이어 위치 표시용 (플레이어 움직임 구현 시연용)
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
            if (block[i][j])                      //블럭 유무 칸그리기
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

    //@todo: 다음에 나올 블럭 표시 추가 + 홀드 구현 추가?
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
    cout << "                         SCORE: " << score << "\n\n";          //*계산한 score로 변경
    cout << "                    Continue? ( y / n )";

    while (ask != 'y' && ask != 'n' && ask != ESC) {
        ask = _getch();
    }
    if (ask == 'y') {                              
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 10; j++) {
                block[i][j] = 0;
            }
        }
        start();
    }
    else if (ask == 'n' || ask == 27) {
        exit_game();
    }
}