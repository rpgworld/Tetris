#include <stdio.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define false 0
#define true 1

#define LEFT 75 // 방향키
#define RIGHT 77
#define UP 72
#define DOWN 80

#define SPACE 32 // 입력키
#define p 112 //일시정지 
#define P 80 //일시정지
#define ESC 27 //게임종료 

void gotoxy(int x, int y);
int getX(int block[4][2], int index); // 블록의 모양 좌표
int getY(int block[4][2], int index);
int setX(int block[4][2], int index, int value); // 블록 좌표 변경
int setY(int block[4][2], int index, int value);
void getShape(); // 블록 모양 가져오기
void new_block();
void Piece(); // 블록 그리기
void resetPiece(int block[4][2]); // 블록 초기화
void rotateRight(); // 블록 회전
void curPiece(int block[4][2], int index); // 게임 창에서의 블록 실제 위치
void nextPiece(int block[4][2], int index);
void shadowPiece(int block[4][2], int index);
int getShadow();
void resetShadow();

void setMain(); // 게임 창 세팅
void drawMain(); // 게임 창 그리기

int check_crush(); // 충돌 확인
void key_check(); // 키 입력 화인
void dropDown(); // 블록 한 칸 씩 내리기
void check_gameOver(); // 게임 오버 확인
void check_line(); // 한줄이 가득 찼을때 이벤트 처리

void getScore();
void pause();
void reset();

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //커서숨기기
void setcursortype(CURSOR_TYPE c) { //커서숨기는 메서드 
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c) {
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}
int shape[7][4][2] = {
		{{0, -1}, {0, 0}, {-1, 0}, {-1, 1}}, // 1. 역번개
		{{0, -1}, {0, 0}, {1, 0}, {1, 1}}, // 2. 번개
		{{0, -1}, {0, 0}, {0, 1}, {0, 2}}, // 3. ㅣ
		{{-1, 0}, {0, 0}, {1, 0}, {0, 1}}, // 4. ㅗ
		{{0, 0}, {1, 0}, {0, 1}, {1, 1}}, // 5. 네모
		{{-1, -1}, {0, -1}, {0, 0}, {0, 1}}, // 6. 역ㄱ
		{{1, -1}, {0, -1}, {0, 0}, {0, 1}}, // 7. ㄱ
};


int main_width = 11; // 게임 폭, 높이
int main_height = 22;

int main_x = 1; // 게임 창 위치
int main_y = 2;
int status_x = 13;

int screen[22][11]; // 게임 창
int screen_cpy[22][11];

int block[4][2]; // 블록
int block_cpy[4][2];
int next_block[4][2]; // 다음 블록
int shadow_block[4][2]; // 그림자 블록

int isSquare = 0; // 네모 모양일때 회전이 되지 않게 하기 위해서
int nSquare = 0;

int bx, by; // 블록 위치 좌표
int x, y; // 블록 모양 좌표

int curPieceX;
int curPieceY;

int nextPieceX;
int nextPieceY;

int shadowPieceX;
int shadowPieceY;
int shadowY = 5;

int score = 0; // 점수
int lastScore = 0;
int bestScore = 0; // 최고 점수

int level; // 레벨


void main()
{
	system("cls");
	setMain();
	drawMain();

	srand(time(NULL));
	setcursortype(NOCURSOR);
	new_block();
	getShape();
	new_block();

	while (1) {
		for (int i = 0; i < 4; i++) {
			drawMain();
			Piece();
			key_check();
			Sleep(100-level*3);
			if (check_crush()) Sleep(50);
		}

		dropDown();
		check_line();
		getScore();
		check_gameOver();

	}
}

void gotoxy(int x, int y)
{
	COORD Pos = { 2 * x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
int getX(int block[4][2], int index)
{
	int x = block[index][0];
	return x;
}
int getY(int block[4][2], int index)
{
	int y = block[index][1];
	return y;
}
int setX(int block[4][2], int index, int value)
{
	block[index][0] = value;
}
int setY(int block[4][2], int index, int value)
{
	block[index][1] = value;
}
void getShape()
{
	bx = (main_width / 2) + 1;
	by = 3;
	isSquare = nSquare;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			block[i][j] = next_block[i][j];
		}
	}
}
void new_block()
{
	int r = rand() % 7;

	if (r == 4) {
		nSquare = 1;
	}
	else {
		nSquare = 0;
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			next_block[i][j] = shape[r][i][j];
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			gotoxy(15 + i, 8 + j);
			printf("  ");
		}
	}
	for (int i = 0; i < 4; i++) {
		nextPiece(next_block, i);
		gotoxy(16 + nextPieceX, 9 + nextPieceY);
		printf("■");
	}

}
void Piece()
{
	if (by == 3) { // 아무 키도 누르지 않고 다음 블록을 가져왔을때, shadowY가 초기화 되지 않는걸 방지
		shadowY = 3;
	}
	for (int i = 0; i < 4; i++) {
		curPiece(block, i);
		screen[curPieceY][curPieceX] = 3;
	}

	while (getShadow()) {
		shadowY++;
	}
	shadowY--;
	if (shadowY > by) {
		for (int i = 0; i < 4; i++) {
			shadowPiece(block, i);
			screen[shadowPieceY + shadowY][shadowPieceX] = 3;
		}
	}
}
int getShadow() {
	int cnt = 0;
	for (int i = 0; i < 4; i++) {
		shadowPiece(block, i);
		if (screen[shadowPieceY + shadowY][shadowPieceX] == 2
			|| screen[shadowPieceY + shadowY][shadowPieceX] == -2) {
			cnt++;
		}
	}
	if (cnt == 0) {
		return true;
	}
	else {
		return false;
	}
}
void nextPiece(int block[4][2], int index) {
	nextPieceX = getX(block, index);
	nextPieceY = getY(block, index);
}
void shadowPiece(int block[4][2], int index) {
	shadowPieceX = getX(block, index) + bx - main_x;
	shadowPieceY = getY(block, index) - main_y;
}
void resetShadow()
{
	for (int i = 0; i < 4; i++) {
		shadowPiece(block, i);
		screen[shadowPieceY + shadowY][shadowPieceX] = 0;
	}

	shadowY = 5;
}
void resetPiece(int block[4][2])
{
	for (int i = 0; i < 4; i++) {
		curPiece(block, i);
		screen[curPieceY][curPieceX] = 0;
	}

	for (int i = 0; i < 4; i++) {
		shadowPiece(block, i);
		screen[shadowPieceY + shadowY][shadowPieceX] = 0;
	}

	shadowY = 5;
}
void rotateRight()
{
	if (isSquare == 1) return;
	for (int i = 0; i < 4; i++) { // 양쪽에 벽이 있을경우 무한 반복에 빠짐 -> 처리!
		curPiece(block_cpy, i);
		if ((screen[curPieceY][curPieceX + 1] == 1
			|| screen[curPieceY][curPieceX + 1] == -2)
			&& (screen[curPieceY][curPieceX - 1] == 1
				|| screen[curPieceY][curPieceX - 1] == -2)) {
			return;
		}
	}
	for (int i = 0; i < 4; i++) { // 바닥이나 굳은 블럭과 부딪쳤을때 회전시 정지상태에 빠짐 -> 처리!
		curPiece(block_cpy, i);
		if ((screen[curPieceY+1][curPieceX] == 2
			|| screen[curPieceY+1][curPieceX] == -2)) {
			return;
		}
	}

	for (int i = 0; i < 4; i++) {
		setX(block_cpy, i, getY(block, i));
		setY(block_cpy, i, -getX(block, i));
	}

	for (int i = 0; i < 4; i++) { // 양쪽에 벽이 있을경우 무한 반복에 빠짐
		curPiece(block_cpy, i);
		if (screen[curPieceY][curPieceX] == 1
			|| screen[curPieceY][curPieceX] == -2) {
			if (bx < 6) {
				bx++;
				i--;
				continue;
			}
			else {
				bx--;
				i--;
				continue;
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			block[i][j] = block_cpy[i][j];
		}
	}
}
void curPiece(int block[4][2], int index)
{
	curPieceX = getX(block, index) + bx - main_x;
	curPieceY = getY(block, index) + by - main_y;
}
void setMain()
{
	for (int i = 0; i < main_height; i++) {
		for (int j = 0; j < main_width; j++) {
			if (i == 0) {
				screen[i][j] = 0;
			}
			else if (i == main_height - 1) {
				if (j == 0 || j == main_width - 1) {
					screen[i][j] = 1;
				}
				else {
					screen[i][j] = 2;
				}
			}
			else {
				if (j == 0 || j == main_width - 1) {
					screen[i][j] = 1;
				}
				else {
					screen[i][j] = 0;
				}
			}
		}
	}
}
void drawMain()
{
	for (int i = 0; i < main_height; i++) {
		for (int j = 0; j < main_width; j++) {
			if (screen[i][j] != screen_cpy[i][j]) {
				gotoxy(main_x + j, main_y + i);
				switch (screen[i][j]) {
				case 1:
					printf("▨");
					break;
				case 0:
					printf("  ");
					break;
				case 2:
					printf("▨");
					break;
				case 3:
					printf("■");
					break;
				case -2:
					printf("□");
					break;
				case -1:
					printf(". ");
					break;

				}
			}
		}
	}
	for (int i = 0; i < main_height; i++) {
		for (int j = 0; j < main_width; j++) {
			screen_cpy[i][j] = screen[i][j];
		}
	}
}
int check_crush()
{
	for (int i = 0; i < 4; i++) {
		curPiece(block, i);
		if (screen[curPieceY + 1][curPieceX] == 2
			|| screen[curPieceY + 1][curPieceX] == -2) {
			return true;
		}
	}
	return false;
}
void key_check()
{
	if (kbhit()) {
		int key = getch();
		if (key == 224) {
			do { key = getch(); } while (key == 224); // 방향키 지시자 버리기
			switch (key) {
			case LEFT:
				for (int i = 0; i < 4; i++) {
					curPiece(block, i);
					if (screen[curPieceY][curPieceX - 1] == 1
						|| screen[curPieceY][curPieceX - 1] == -2) {
						return;
					}
				}
				resetPiece(block);
				bx--;
				Piece();
				break;
			case RIGHT:
				for (int i = 0; i < 4; i++) {
					curPiece(block, i);
					if (screen[curPieceY][curPieceX + 1] == 1
						|| screen[curPieceY][curPieceX + 1] == -2) {
						return;
					}
				}
				resetPiece(block);
				bx++;
				Piece();
				break;
			case UP:
				resetPiece(block);
				rotateRight();
				Piece();
				break;
			case DOWN:
				for (int i = 0; i < 4; i++) {
					curPiece(block, i);
					if (screen[curPieceY + 1][curPieceX] == 2
						|| screen[curPieceY + 1][curPieceX] == -2) {
						return true;
					}
				}
				resetPiece(block);
				by++;
				Piece();
				break;
			}
		}
		else {
			switch (key) {
			case SPACE:
				while (!check_crush()) {
					dropDown();
				}
				break;
			case p:
			case P:
				pause();
				break;
			}
		}
	}
	while (kbhit()) getch();
}
void dropDown()
{
	if (check_crush()) {
		for (int i = 0; i < 4; i++) {

			for (int j = 0; j < 2; j++) {
				curPiece(block, i);
				screen[curPieceY][curPieceX] = -2;
			}
		}
		getShape();
		new_block();
	}
	else {
		resetPiece(block);
		by++;
		Piece();
	}
}
void check_gameOver()
{
	for (int i = 1; i < main_width - 2; i++) {
		if (screen[2][i] == -2) {
			gotoxy(main_x + 3, main_y + 5); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤"); //게임오버 메세지 
			gotoxy(main_x + 3, main_y + 6); printf("▤                              ▤");
			gotoxy(main_x + 3, main_y + 7); printf("▤  +-----------------------+   ▤");
			gotoxy(main_x + 3, main_y + 8); printf("▤  |  G A M E  O V E R..   |   ▤");
			gotoxy(main_x + 3, main_y + 9); printf("▤  +-----------------------+   ▤");
			gotoxy(main_x + 3, main_y + 10); printf("▤   YOUR SCORE: %6d         ▤", score);
			gotoxy(main_x + 3, main_y + 11); printf("▤                              ▤");
			gotoxy(main_x + 3, main_y + 12); printf("▤  Press any key to restart..  ▤");
			gotoxy(main_x + 3, main_y + 13); printf("▤                              ▤");
			gotoxy(main_x + 3, main_y + 14); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

			Sleep(1000);
			while (kbhit()) getch();
			int key = getch();
			reset();
		}
	}
}
void check_line()
{
	int b_cnt; // 블록갯수

	for (int i = main_height - 2; i > 3; i--) {
		b_cnt = 0;
		for (int j = 1; j < main_width - 1; j++) {
			if (screen[i][j] == -2) b_cnt++;
		}
		if (b_cnt == main_width - 2) { // 한줄이 가득 찼을때
			for (int k = i; k > 2; k--) {
				for (int l = 1; l < main_width - 1; l++) {
					screen[k][l] = screen[k - 1][l];
				}
			}
			score += 100;
		}
	}
}
void getScore()
{
	level = (score / 1000) + 1;

	gotoxy(status_x, main_y + 2);
	printf("LEVEL : %5d", level);

	gotoxy(status_x, main_y + 3);
	printf("SCORE : %5d", score);

	gotoxy(status_x, main_y + 5); printf("┌NEXT BLOCK┐");
	for (int i = 0; i < 4; i++) {
		gotoxy(status_x, main_y + 6 + i); printf("│");
		gotoxy(status_x + 6, main_y + 6 + i); printf("│");
	}
	gotoxy(status_x , main_y + 10); printf("└─────┘");
	gotoxy(status_x, main_y + 12); printf("BEST SCORE : %5d", bestScore);
	gotoxy(status_x, main_y + 13); printf("LAST SCORE : %5d", lastScore);
	gotoxy(status_x, main_y + 15); printf("  △   : Lotate ");
	gotoxy(status_x, main_y + 16); printf("◁  ▷ : Left / Right");
	gotoxy(status_x, main_y + 17); printf("  ▽   : Soft Drop");
	gotoxy(status_x, main_y + 19); printf("SPACE : Hard Drop");
	gotoxy(status_x, main_y + 20); printf("P   : Pause");
	gotoxy(status_x, main_y + 21); printf("ESC  : Quit");
}
void pause()
{
	getch();
}

void reset()
{
	lastScore = score;
	if (bestScore < score) {
		bestScore = score;
	}
	score = 0;

	system("cls");
	setMain();

	// 기존에 있는 screen_cpy 를 비워줍니다.
	for (int i = 0; i < main_height; i++) {
		for (int j = 0; j < main_width; j++) {
			screen_cpy[i][j] = 0;
		}
	}

	drawMain();

	getShape();
}