//@도엽: 화면 출력을 하나로 묶으면 코드가 깔끔해 질 것 같음

#include "head.h"
#pragma warning(disable:6031)

void title();
void start();
void play_menu(int*);
void draw_frame();
void game_over();
void exit_game();
//void game_move(int*, int*, int*);

int x = 2, y = 1;
int key;
bool block[20][10];
int score = 0;               // 점수

void gotoxy(int a, int b) {
    COORD Cur = { a,b };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

int main() {
    int ch, i = 1;
    system("mode con cols=60 lines=25 | title TETRIS");
    title();
    start();

    while (1)                       ///////     게임 중 모든 클릭을 담당
    {
        if (_kbhit()) {
            ch = _getch();
            if (ch == 109) {     //메뉴
                play_menu(&i);
            }
            else if (ch == 224) {                       //*방향키 입력
                ch = _getch();
                //               game_move(&ch, &x, &y);
                //               gotoxy(x, y);
                //               printf(PLAYER_MARK);
                //               i++; 
            }
        }
    }

    return 0;
}

void title() {
    printf("\n\n\n\n\n");
    printf("    ■■■   ■■■   ■■■   ■■■   ■■■   ■■■\n");
    printf("      ■     ■         ■     ■  ■     ■     ■\n");
    printf("      ■     ■■■     ■     ■■■     ■     ■■■\n");
    printf("      ■     ■         ■     ■ ■      ■         ■\n");
    printf("      ■     ■■■     ■     ■  ■   ■■■   ■■■\n\n\n\n");

    printf("                      ◀ ▶ : MOVE\n");
    printf("                        ▼  : SOFT DROP\n");
    printf("                      SPACE : HARD DROP\n\n\n");
    printf("                   PRESS ANY KEY TO START");
    
    while (1) {
        if (_kbhit()) {
            key = _getch();
            break;
        }
    }
}

void start() {
    system("cls");
    score = 0; //점수 초기화

    printf("\n\n\n\n\n");
    printf("                            ■■\n");
    printf("                          ■   ■\n");
    printf("                             ■\n");
    printf("                          ■   ■\n");
    printf("                            ■■\n\n\n\n");
    Sleep(500);

    system("cls");
    printf("\n\n\n\n\n");
    printf("                            ■■\n");
    printf("                          ■   ■\n");
    printf("                             ■\n");
    printf("                            ■\n");
    printf("                          ■■■■\n\n\n\n");
    Sleep(500);

    system("cls");
    printf("\n\n\n\n\n");
    printf("                             ■\n");
    printf("                           ■■\n");
    printf("                             ■\n");
    printf("                             ■\n");
    printf("                           ■■■\n\n\n\n");
    Sleep(500);

    draw_frame();
}

void draw_frame() {         //게임 화면 틀
    system("cls");
    x = 2, y = 1;           //*좌표 초기화

    for (int i = 0; i < FR_SIZE_WIDTH + 2; i++) {
        printf(WALL_MARK);
    }
    printf("\n");

    for (int i = 0; i < FR_SIZE_HEIGHT; i++) {
        printf(WALL_MARK);                          //왼쪽 벽
        for (int j = 0; j < FR_SIZE_WIDTH; j++) {
            if (block[i][j]) {                     //블럭 유무 칸그리기
                printf(BLOCK_MARK);
            }
            else {
                printf(NONE_MARK);
            }
        }
        printf(WALL_MARK);                         //오른쪽 벽

        if (2 <= i && i <= 9) {                //다음 블럭 보여줄 칸
            printf(NONE_MARK);
            printf(WALL_MARK);
            for (int j = 0; j < 5; j++) {
                if (i == 2 || i == 9) {
                    printf(WALL_MARK);
                }
                else {
                    printf(NONE_MARK);
                }
            }
            printf(WALL_MARK);
        }

        printf("\n");
    }

    for (int i = 0; i < FR_SIZE_WIDTH + 2; i++) {
        printf(WALL_MARK);
    }

    printf("   m: menu");

    //*다음에 나올 블럭 표시 추가       + 홀드 구현 추가?
}

void play_menu(int* i) {
    char ch;
    system("cls");
    printf("\n\n\n\n\n");
    printf("           ■  ■     ■■■   ■   ■   ■    ■\n");
    printf("          ■ ■ ■    ■       ■■ ■   ■    ■\n");
    printf("         ■  ■  ■   ■■■   ■ ■■   ■    ■\n");
    printf("         ■      ■   ■       ■   ■    ■  ■\n");
    printf("         ■      ■   ■■■   ■   ■     ■■\n\n\n\n");
    printf("                  r: RESUME     q: EXIT");

    ch = _getch();
    while (ch != 'r' && ch != 'q') {
        ch = _getch();
    }
    if (ch == 'r') {            //게임 진행
        x = 2;    y = 1;    (*i) = 1;            //*좌표                
        draw_frame();
    }
    else if (ch == 'q')
        game_over();       //q 게임 종료

}


void exit_game() {
    system("cls");
    printf("\n\n\n\n\n");
    printf("                ■■■    ■  ■   ■■■\n");
    printf("                ■   ■   ■  ■   ■\n");
    printf("                ■■■     ■■    ■■■\n");
    printf("                ■   ■     ■     ■\n");
    printf("                ■■■      ■     ■■■\n\n\n\n");
    exit(0);
}

void game_over() {
    char ask;
    system("cls");
    printf("\n\n\n\n\n");
    printf("  ■■■   ■   ■  ■ ■■■    ■■  ■  ■ ■■■ ■■■\n");
    printf(" ■      ■  ■ ■■■ ■       ■  ■ ■  ■ ■     ■  ■\n");
    printf(" ■ ■■ ■■■ ■  ■ ■■■   ■  ■ ■  ■ ■■■ ■■■\n");
    printf(" ■   ■ ■  ■ ■  ■ ■       ■  ■  ■■  ■     ■ ■\n");
    printf("  ■■■ ■  ■ ■  ■ ■■■    ■■    ■   ■■■ ■  ■\n\n\n\n");
    printf("                         SCORE: %d\n\n", 10);               //*계산한 score로 변경
    printf("                    Continue? ( y / n ) : ");

    ask = _getch();
    while (ask != 'y' && ask != 'n' && ask != 27) {             //27 = Escape
        ask = _getch();
    }
    if (ask == 'y') {                               //@도엽: ESC로 메뉴 진입하고 돌아가기도 ESC로 하면 될 듯
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

/*
void game_move(int* ch, int* x, int* y) {
    int p, q;
    p = ((*x) - 2) / 2;
    q = ((*y) - 1);

                    //*다 쌓여서 game_over로 이동

    switch (*ch) {
    case UP:
        (*y)--;
        if ((*y) < 1)
            (*y) = 1;
        break;
    case DOWN:
        (*y)++;
        if ((*y) > frame_size - 1)
            (*y) = frame_size - 1;
        break;
    case LEFT:
        (*x) -= 2;
        if ((*x) < 2)
            (*x) = 2;
        break;
    case RIGHT:
        (*x) += 2;
        if ((*x) > 2 * frame_size - 2)
            (*x) = 2 * frame_size - 2;
        break;
    }
}
*/