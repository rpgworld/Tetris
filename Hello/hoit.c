#include <stdio.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define false 0
#define true 1

void circleTest();
void random();

void gotoxy(int x, int y);
int getX(int block[4][2], int index);
int getY(int block[4][2], int index);
int setX(int block[4][2], int index, int value);
int setY(int block[4][2], int index, int value);
void getShape();
void Piece();
void rotationRight();
void setMain();
void drawMain();
int crush_check();

int shape[7][4][2] = {
		{{0, -1}, {0, 0}, {-1, 0}, {-1, 1}}, // 1. 역번개
		{{0, -1}, {0, 0}, {1, 0}, {1, 1}}, // 2. 번개
		{{0, -1}, {0, 0}, {0, 1}, {0, 2}}, // 3. ㅣ
		{{-1, 0}, {0, 0}, {1, 0}, {0, 1}}, // 4. ㅗ
		{{0, 0}, {1, 0}, {0, 1}, {1, 1}}, // 5. 네모
		{{-1, -1}, {0, -1}, {0, 0}, {0, 1}}, // 6. 역ㄱ
		{{1, -1}, {0, -1}, {0, 0}, {0, 1}}, // 7. ㄱ
};

struct point
{
	int xpos;
	int ypos;
};
struct circle
{
	double radius;
	struct point* center;
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

int bx= 5, by = 10; // 블록 위치 좌표
int x, y; // 블록 모양 좌표

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

		if (kbhit()) {
			system("cls");
			rotationRight();
			Piece();
		}
		while (kbhit()) getch();

		if (crush_check()) {
			Sleep(100000);
		}

		by++;

	}
}

void circleTest()
{
	struct point cen = { 2, 6 };
	double rad = 5.5;
	struct circle ring = { rad, &cen };
	printf("원의 반지름 : %g \n", ring.radius);
	printf("원의 중심 : (%d, %d) \n", (ring.center)->xpos, (ring.center)->ypos);
}
void random()
{
	int i = 1;
	srand(time(NULL));
	while (1) {

		int r = (rand() % 7) + 1;
		printf("%d 번째 값 : %d \n", i, r);
		i++;
		Sleep(1000);
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
	int r = (rand() % 7) + 1;

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
		printf("%d", 1);
	}
}
void rotationRight() 
{	
	if (isSquare == 1) return;
	for (int i = 0; i < 4; i++) {
		setX(block_cpy, i, getY(block, i));
		setY(block_cpy, i, -getX(block, i));
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
				screen[i][j] = 1;
			}
			else if (i == main_height - 1) {
				screen[i][j] = 2;
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
	for (int i = 0; i < main_height; i++) {
		for (int j = 0; j < main_width; j++) {
			screen_cpy[i][j] = screen[i][j];
		}
	}
}
void drawMain()
{
	for (int i = 0; i < main_height; i++) {
		for (int j = 0; j < main_width; j++) {
			gotoxy(main_x + j, main_y + i);
			switch (screen[i][j]) {
			case 1:
				printf("%d", 1);
				break;
			case 0:
				printf("%d", 0);
				break;
			case 2:
				printf("%d", 2);
				break;
			}
		}
	}
}

int crush_check()
{
	for (int i = 0; i < 4; i++) {
		if (screen[getY(block, i) + by - (main_y -1)][getX(block, i) + bx - main_x] == 2) {
			return true;
		}
	}
	return false;
}

