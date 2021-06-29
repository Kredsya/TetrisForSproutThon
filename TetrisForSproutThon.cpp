//블럭있는 보드 좌표 (18,1)부터 (36,20)까지 x +2, y +1 증가시켜 이용    
//mapData는 보드 아래부터 인덱스 시작

//@todo: 점수 카운트, 블럭 벡터, 블럭 벡터 이동 제한, 블럭쌓기 구현 (회전은 쌓기 구현 이후)     -> clear?
//@todo: 다음에 나올 블럭 표시 추가 + 홀드 구현 추가?        <-좌표 이용 새로운 함수 생성
//@todo: 게임오버 처리

#include "head.h"

void gotoxy(int, int);
void init();
void title();
void start();
void fillNextBlock();
void play_menu();
void draw_frame();
void draw_block();
void game_over();
void exit_game();
void player_move(int);
void draw_board(bool);
void erase_block(int, int);
void draw_nextBlock();

struct BlockData {
    int type;
    int rot = 0;
};

int x = 26, y = 2;      //y를 1 낮춘곳에서 블럭을 스폰하여 윗줄 지워짐 현상 제거
int key;
int cnt = 0;        //쌓기 판정용
int score = 0;               // 점수
int getOutCnt = 1;
vector<pair<vector<bool>, int>> mapData;
vector<bool> emptyLine(10, false);	//vector constructor 중 하나 : 변수명(개수, 초기화할 데이터);
deque<BlockData> nextBlock;
BlockData fallingBlock;
BlockData holdBlock;
BlockData nowBlock;

int main() {
    int ch;
    float ti;
    clock_t st, end;
    st = clock();

    init();
    title();
    start();

    nowBlock = nextBlock.front();
    nextBlock.pop_front();

    while (1) {                       ///////     게임 중 모든 클릭을 담당
        if (_kbhit()) {
            ch = _getch();
            if (ch == ESC) {     //메뉴
                play_menu();
            }
            else if (ch == zKEY || ch == ZKEY) {
                player_move(ch);
            }
            else if (ch == 224) {                       //방향키 입력
                ch = _getch();
                player_move(ch);               //방향키는 224->n으로 입력되어 space는 따로 구현
            }
            else if (ch == SPACE) {                 //@todo: space로 hard drop 구현
                while (true) {
                    player_move(DOWN);
                    if (x == 26 && y == 2)
                        break;
                }
            }
            //@todo: 홀드 구현시 shift 추가 

        }

        for (int i = 0; i < 20; i++) {          //다 채워지면 한 줄 지우기
            if (mapData[i].second == 10) {
                mapData.erase(mapData.begin() + i);
                mapData.push_back(make_pair(emptyLine, 0));
                draw_board(0);                   //다시 그리기
                score += 20;                //줄 지워서 점수 획득
                gotoxy(0, 0);
                cout << "SCORE: " << score;
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
    cout << "                       ▲   : ROTATION\n";
    cout << "                     ◀  ▶ : MOVE\n";
    cout << "                       ▼   : SOFT DROP\n";
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
    random.push_back(make_pair(1, rand()));
    random.push_back(make_pair(4, rand()));
    random.push_back(make_pair(5, rand()));
    random.push_back(make_pair(6, rand()));
    random.push_back(make_pair(10, rand()));
    random.push_back(make_pair(11, rand()));
    random.push_back(make_pair(12, rand()));
    
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

void draw_board(bool tf) {           
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
    int a = x;              //원래 xy좌표 저장
    int b = y;

    erase_block(x, y);

    switch (ch) {
    case zKEY:
    case ZKEY:
        nowBlock.rot--;
        if (nowBlock.rot == -1)
            nowBlock.rot = 3;
        break;
    case UP:
        nowBlock.rot++;
        if (nowBlock.rot == 4)
            nowBlock.rot = 0;
        break;
    case DOWN:
        y++;
        if (y > 20)
            y = 20;
        break;
    case LEFT:
        x -= 2;
        if (x < 18 && nowBlock.type != I)   //I블럭에 대한 특별 case 마련
            x = 18;
        else if (x == 16)
            x = 16;
        break;
    case RIGHT:
        x += 2;
        if (x > 36)
            x = 36;
        break;
    }

    for (int i = 0; i < 4; i++) {                               //mapData 기반 블럭 움직임 판정 for문
        int dx = blockShape[nowBlock.type][nowBlock.rot][i][0];
        int dy = blockShape[nowBlock.type][nowBlock.rot][i][1];

        if (20 - y - dy < 0 || (x + dx - 18) / 2 < 0 || (x + dx - 18) / 2 >= 10) {
            x = a, y = b;
            break;
        }
        if (mapData[20 - y - dy].first[(x + dx - 18) / 2]) {
            x = a, y = b;
            break;
        }
    }

    if (mapData[19].second != 0 && x == a && y == b && ch == DOWN)
        game_over();

    if (x == a && y == b && ch == DOWN) { //움직이지 못했을 경우 
        cnt++;
    }

    if (cnt == 3) {
        cnt = 0;
        for (int i = 0; i < 4; i++) {       //mapData 갱신
            int dx = blockShape[nowBlock.type][nowBlock.rot][i][0];
            int dy = blockShape[nowBlock.type][nowBlock.rot][i][1];
            mapData[20 - y - dy].first[(x + dx - 18) / 2] = 1;
            mapData[20 - y - dy].second += 1;
        }
        draw_board(true);
        x = 26, y = 2;
        score += 10;                //블럭 쌓아서 점수 획득
        gotoxy(0, 0);
        cout << "SCORE: " << score;
        nowBlock = nextBlock.front();   //다음 블럭을 뽑아놓기
        nextBlock.pop_front();
        getOutCnt++;
        if (getOutCnt % 7 == 0)     //7개 뽑아 썼으면 fillNextBlock 호출
            fillNextBlock();
        draw_nextBlock();
    }

        //cout << BLOCK_MARK;        //처음 플레이어 위치 표시용 (플레이어 움직임 구현 시연용)
    draw_block();
}

void erase_block(int a, int b) {
    int A, B;
    for (int i = 0; i < 4; i++) {
        A = a + blockShape[nowBlock.type][nowBlock.rot][i][0];
        B = b + blockShape[nowBlock.type][nowBlock.rot][i][1];

        gotoxy(A, B);
        cout << NONE_MARK;
    }
}

void draw_nextBlock() {
    int a, b;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            gotoxy(42, 6 * i + 2 + j);
            cout << "            ";
        }
        for (int k = 0; k < 4; k++) {
            a = 44 + blockShape[nextBlock[i].type][0][k][0];
            b = 6 * i + 3 + blockShape[nextBlock[i].type][0][k][1];

            gotoxy(a, b);
            cout << BLOCK_MARK;
        }
    }
}

void draw_block() {
    int X, Y;
    for (int i = 0; i < 4; i++) {
        X = x + blockShape[nowBlock.type][nowBlock.rot][i][0];
        Y = y + blockShape[nowBlock.type][nowBlock.rot][i][1];

        gotoxy(X, Y);
        cout << BLOCK_MARK;
    }
}

void draw_frame() {         //게임 화면 틀
    system("cls");

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
            if (mapData[19 - i].first[j])
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
    
    gotoxy(0, 0);
    cout << "SCORE: " << score;

    draw_nextBlock();
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
    cout << "                    Continue? ( y / n ): ";

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