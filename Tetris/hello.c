#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>

void gotoxy(int x, int y);
void getShape();
int getX(int index);
int getY(int index);
int setX(int coords[4][2], int index, int value);
int setY(int coords[4][2], int index, int value);
void Piece();
void leftRotation();

int shape[7][4][2] = {
		{{0, -1}, {0, 0}, {-1, 0}, {-1, 1}},
		{{0, -1}, {0, 0}, {1, 0}, {1, 1}},
		{{0, -1}, {0, 0}, {0, 1}, {0, 2}},
		{{-1, 0}, {0, 0}, {1, 0}, {0, 1}},
		{{0, 0}, {1, 0}, {0, 1}, {1, 1}},
		{{-1, -1}, {0, -1}, {0, 0}, {0, 1}},
		{{1, -1}, {0, -1}, {0, 0}, {0, 1}},
};
int coords[4][2];
int cpy[4][2];
int x, y;
int bx, by; // ºí·Ï ÁÂÇ¥

void main()
{
	bx = 5, by = 5;
	getShape();
	while (1) {
		system("cls");
		Piece();

		if (kbhit()) {
			leftRotation();
			system("cls");
			Piece();
			while (kbhit()) getch();
		}
		Sleep(300);
		by++;
	}
}
void gotoxy(int x, int y)
{
	COORD Pos = { 2 * x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void getShape()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			coords[i][j] = shape[6][i][j];
		}
	}
}
int getX(int index)
{
	int x = coords[index][0];
	return x;
}
int getY(int index)
{
	int y = coords[index][1];
	return y;
}
int setX(int coords[4][2], int index, int value)
{
	coords[index][0] = value;
}
int setY(int coords[4][2], int index, int value)
{
	coords[index][1] = value;
}
void Piece()
{
	for (int i = 0; i < 4; i++) {
		x = getX(i);
		y = getY(i);

		gotoxy(bx + x, by - y);
		printf("%d", 1);
	}
}
void leftRotation() {
	for (int i = 0; i < 4; i++) {
		setX(cpy, i, -getY(i));
		setY(cpy, i, getX(i));
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			coords[i][j] = cpy[i][j];
		}
	}
}