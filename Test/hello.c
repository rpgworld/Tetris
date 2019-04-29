#include <stdio.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32
#define P 80
#define p 112
#define ESC 27

void gotoxy(int x, int y);
void getShape();
void keyTest();

int x = 0, y = 0;

void main()
{
	while (1)
	{

	}
}

void gotoxy(int x, int y)
{
	COORD Pos = { 2 * x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void getShape()
{
	gotoxy(5 + x, 5 + y);
	printf("■");
}

void keyTest()
{
	while (1) {
		getShape();

		if (kbhit()) {
			int key = getch();
			if (key == 224) { // 방향키인 경우?
				do { key = getch(); } while (key == 224);
				switch (key) {
				case LEFT:
					system("cls");
					x--;
					getShape();
					break;
				case RIGHT:
					system("cls");
					x++;
					getShape();
					break;
				case UP:
					system("cls");
					y--;
					getShape();
					break;
				case DOWN:
					system("cls");
					y++;
					getShape();
					break;
				case SPACE:
					system("cls");
					x++;
					y++;
					getShape();
					break;
				}
			}
		}
	}
}