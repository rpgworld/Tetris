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
#define SPACE 32

void circleTest();
void random();

void gotoxy(int x, int y);
int getX(int block[4][2], int index);
int getY(int block[4][2], int index);
int setX(int block[4][2], int index, int value);
int setY(int block[4][2], int index, int value);
void getShape();
void Piece();
void rotateRight();
void setMain();
void drawMain();
int check_crush();
void key_check();
void dropDown();
void check_gameOver();
void check_line();

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

int screen[22][11]; // 게임 창
int screen_cpy[22][11];

int block[4][2]; // 블록
int block_cpy[4][2];
int next_block[4][2]; // 다음 블록

int isSquare = 0;

int bx, by; // 블록 위치 좌표
int x, y; // 블록 모양 좌표

int crush_on = false;

int score = 0; // 점수

void main()
{
	setMain();
	drawMain();

	srand((unsigned)time(NULL));
	getShape();

	while (1) {
		system("cls");
		drawMain();
		Piece();
		Sleep(500);

		key_check();

		dropDown();
		check_line();
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
	int r = rand() % 7;

	if (r == 4) {
		isSquare = 1;
	}
	else {
		isSquare = 0;
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			block[i][j] = shape[r][i][j];
		}
	}
}
void Piece()
{
	for (int i = 0; i < 4; i++) {
		x = getX(block, i);
		y = getY(block, i);

		gotoxy(bx + x, by + y);
		printf("■");
	}
}
void rotateRight()
{	
	if (isSquare == 1) return;
	for (int i = 0; i < 4; i++) {
		setX(block_cpy, i, getY(block, i));
		setY(block_cpy, i, -getX(block, i));
	}

	for (int i = 0; i < 4; i++) {
		if (screen[getY(block_cpy, i) + by - main_y][getX(block_cpy, i) + bx - main_x] == 1
			|| screen[getY(block_cpy, i) + by - main_y][getX(block_cpy, i) + bx - main_x] == 2) {
			return;
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			block[i][j] = block_cpy[i][j];
		}
	}
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
				} else {
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
			if (screen[i][j]) {
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

				case -2:
					printf("□");
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
		if (screen[getY(block, i) + by - (main_y -1)][getX(block, i) + bx - main_x] == 2
			|| screen[getY(block, i) + by - (main_y - 1)][getX(block, i) + bx - main_x] == -2) {
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
					if (screen[getY(block, i) + by - main_y][getX(block, i) + bx - (main_x + 1)] == 1 
						|| screen[getY(block, i) + by - main_y][getX(block, i) + bx - (main_x + 1)] == -2) {
						return;
					}
				}
				bx--;
				Piece();
				break;
			case RIGHT:
				for (int i = 0; i < 4; i++) {
					if (screen[getY(block, i) + by - main_y][getX(block, i) + bx - (main_x -1)] == 1
						|| screen[getY(block, i) + by - main_y][getX(block, i) + bx - (main_x + 1)] == -2) {
						return;
					}
				}
				bx++;
				Piece();
				break;
			case UP:
				rotateRight();
				Piece();
				break;
			case DOWN:
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
				screen[getY(block, i) + by - main_y][getX(block, i) + bx - main_x] = -2;
			}
		}
		getShape();
	}
	else {
		by++;
	}
}
void check_gameOver()
{
	for (int i = 1; i < main_width - 2; i++) {
		if (screen[3][i] < 0) {
			gotoxy(main_x, main_y + 5); printf("▤▤▤▤▤▤▤▤▤▤");
			gotoxy(main_x, main_y + 6);	printf("▤    GAME OVER   ▤ ");
			gotoxy(main_x, main_y+7); printf("▤▤▤▤▤▤▤▤▤▤");
			Sleep(100000);
		}
	}
}
void check_line()
{
	int b_cnt; // 블록갯수
	
	for (int i = main_height - 2; i > 3;) {
		b_cnt = 0;
		for (int j = 1; j < main_width - 1; j++) {
			if (screen[i][j] == -2) b_cnt++;
		}
		if (b_cnt == main_width - 2) {
			for (int k = i; k > 2; k--) {
				for (int l = 1; l < main_width - 1; l++) {
					screen[k][l] = screen[k - 1][l];
				}
			}
		}
		else {
			i--;
		}
	}
}