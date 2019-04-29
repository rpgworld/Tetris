#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>

void gotoxy(int x, int y);
int getX(int block[4][2], int index);
int getY(int block[4][2], int index);
int setX(int block[4][2], int index, int value);
int setY(int block[4][2], int index, int value);
void getShape();

int shape[7][4][2] = {
		{{0, -1}, {0, 0}, {-1, 0}, {-1, 1}},
		{{0, -1}, {0, 0}, {1, 0}, {1, 1}},
		{{0, -1}, {0, 0}, {0, 1}, {0, 2}},
		{{-1, 0}, {0, 0}, {1, 0}, {0, 1}},
		{{0, 0}, {1, 0}, {0, 1}, {1, 1}},
		{{-1, -1}, {0, -1}, {0, 0}, {0, 1}},
		{{1, -1}, {0, -1}, {0, 0}, {0, 1}},
};

int main_x = 11;
int main_y = 22;

int screen[22][11];

int block[4][2];
int block_cpy[4][2];
int next_block[4][2];

void main()
{
	

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
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			block[i][j] = shape[6][i][j];
		}
	}
}