//블럭있는 보드 좌표 (18,1)부터 (36,20)까지 x +2, y +1 증가시켜 이용    
//mapData는 보드 아래부터 인덱스 시작

//@todo: 점수 카운트, 블럭 벡터, 블럭 벡터 이동 제한, 블럭쌓기 구현 (회전은 쌓기 구현 이후)
//@todo: 다음에 나올 블럭 표시 추가 + 홀드 구현 추가?        <-좌표 이용 새로운 함수 생성

#include "head.h"

void gotoxy(int, int);
void init();
void title();
void start();
void fillNextBlock();
void play_menu();
void draw_frame();
void draw_block(BlockData, int, int);
void game_over();
void exit_game();
void player_move(int);
void draw_board(int);

typedef struct BlockData {
    int type;
    int rot = 0;
};

int x = 26, y = 1;
int key;
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
            //else if (ch == SPACE)                     //@todo: space로 hard drop 구현

                                                //@todo: 홀드 구현시 shift 추가 

        }

        for (int i = 0; i < 20; i++) {          //다 채워지면 한 줄 지우기
            if (mapData[i].second == 10) {
                mapData.erase(mapData.begin() + i);
                mapData.push_back(make_pair(emptyLine, 0));
                draw_board(i);                   //다시 그리기
            }
        }

        end = clock();                              //1초마다 블록 낙하
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

void draw_board(int n) {            //@todo: 보드를 다시 그릴 때 이동중인 블럭이 지워지지 않는지 확인

    while (mapData[n].second != 0){            //빈 줄이 나올 때까지 올라가면서 다시 출력
        gotoxy(18, 21 - n);              //mapData는 아래부터 시작, 좌표는 위부터 시작으로 계산
        for (int i = 0; i < FR_SIZE_WIDTH; i++) {
            if (mapData[n].first[i])
                cout << BLOCK_MARK;
            else
                cout << NONE_MARK;
        }
        n++;
        if (n > 20)
            break;
    }
}

void player_move(int ch) {       //*다 쌓이면 game_over로 이동
    int a = x;              //원래 xy좌표 저장
    int b = y;

    cout << "\b\b";
    cout << NONE_MARK;

    //@todo: up으로 회전 추가

    switch (ch) {
    case UP:
        break;
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

    if (mapData[21 - y].first[x - 18])          //@todo: mapData 비교로 못 움직이게 추가
        x = a, y = b;

    gotoxy(x, y);
    cout << BLOCK_MARK;        //처음 플레이어 위치 표시용 (플레이어 움직임 구현 시연용)
                                
                               //@todo: 옮길 수 있는 것인지에 대한 체크가 선행되어야 함
                                //@todo: draw_block으로 블럭 그리기를 할 예정
}

void draw_block(BlockData block, int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {

        }
    }
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