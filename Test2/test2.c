#include <stdio.h>
#include <time.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define false 0
#define true 1

#define LEFT 75 // ����Ű
#define RIGHT 77
#define UP 72
#define DOWN 80

#define SPACE 32 // �Է�Ű
#define p 112 //�Ͻ����� 
#define P 80 //�Ͻ�����
#define ESC 27 //�������� 

void gotoxy(int x, int y);
int getX(int block[4][2], int index); // ����� ��� ��ǥ
int getY(int block[4][2], int index);
int setX(int block[4][2], int index, int value); // ��� ��ǥ ����
int setY(int block[4][2], int index, int value);
void getShape(); // ��� ��� ��������
void Piece(); // ��� �׸���
void resetPiece(); // ��� �ʱ�ȭ
void rotateRight(); // ��� ȸ��
void curPiece(int block[4][2], int index); // ���� â������ ��� ���� ��ġ

void setMain(); // ���� â ����
void drawMain(); // ���� â �׸���

int check_crush(); // �浹 Ȯ��
void key_check(); // Ű �Է� ȭ��
void dropDown(); // ��� �� ĭ �� ������
void check_gameOver(); // ���� ���� Ȯ��
void check_line(); // ������ ���� á���� �̺�Ʈ ó��

void getScore();
void pause();

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //Ŀ�������
void setcursortype(CURSOR_TYPE c) { //Ŀ������� �޼��� 
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
		{{0, -1}, {0, 0}, {-1, 0}, {-1, 1}}, // 1. ������
		{{0, -1}, {0, 0}, {1, 0}, {1, 1}}, // 2. ����
		{{0, -1}, {0, 0}, {0, 1}, {0, 2}}, // 3. ��
		{{-1, 0}, {0, 0}, {1, 0}, {0, 1}}, // 4. ��
		{{0, 0}, {1, 0}, {0, 1}, {1, 1}}, // 5. �׸�
		{{-1, -1}, {0, -1}, {0, 0}, {0, 1}}, // 6. ����
		{{1, -1}, {0, -1}, {0, 0}, {0, 1}}, // 7. ��
};


int main_width = 11; // ���� ��, ����
int main_height = 22;

int main_x = 1; // ���� â ��ġ
int main_y = 2;

int screen[22][11]; // ���� â
int screen_cpy[22][11];

int block[4][2]; // ���
int block_cpy[4][2];
int next_block[4][2]; // ���� ���

int isSquare = 0; // �׸� ����϶� ȸ���� ���� �ʰ� �ϱ� ���ؼ�

int bx, by; // ��� ��ġ ��ǥ
int x, y; // ��� ��� ��ǥ

int curPieceX;
int curPieceY;

int score = 0; // ����

void main()
{
	system("cls");
	setMain();
	drawMain();

	srand((unsigned)time(NULL));
	setcursortype(NOCURSOR);
	getShape();

	while (1) {
		for (int i = 0; i < 4; i++) {
			drawMain();
			Piece();
			key_check();
			Sleep(100);
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
	for (int i = 0; i < 4; i++){
		curPiece(block, i);
		screen[curPieceY][curPieceX] = 3;
	}
	/*
	for (int i = 0; i < 4; i++) {
		curPiece(block, i);
		for (int i = curPieceY + 1; i < main_height; i++) {
			
		}
	}
	*/
}
void resetPiece()
{
	for (int i = 0; i < 4; i++) {
		curPiece(block, i);
		screen[curPieceY][curPieceX] = 0;
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
		curPiece(block_cpy, i);
		if (screen[curPieceY][curPieceX] == 1
			|| screen[curPieceY][curPieceX] == 2) {
			return;
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
					printf("��");
					break;
				case 0:
					printf("  ");
					break;
				case 2:
					printf("��");
					break;
				case 3:
					printf("��");
					break;
				case -2:
					printf("��");
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
			do { key = getch(); } while (key == 224); // ����Ű ������ ������
			switch (key) {
			case LEFT:
				for (int i = 0; i < 4; i++) {
					curPiece(block, i);
					if (screen[curPieceY][curPieceX-1] == 1
						|| screen[curPieceY][curPieceX - 1] == -2) {
						return;
					}
				}
				resetPiece();
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
				resetPiece();
				bx++;
				Piece();
				break;
			case UP:
				resetPiece();
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
				resetPiece();
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
	}
	else {
		resetPiece();
		by++;
		Piece();
	}
}
void check_gameOver()
{
	for (int i = 1; i < main_width - 2; i++) {
		if (screen[2][i] < 0) {
			gotoxy(main_x, main_y + 5); printf("�ǢǢǢǢǢǢǢǢǢǢ�");
			gotoxy(main_x, main_y + 6);	printf("��    GAME OVER     �� ");
			gotoxy(main_x, main_y + 7); printf("�ǢǢǢǢǢǢǢǢǢǢ�");
			Sleep(100000);
		}
	}
}
void check_line()
{
	int b_cnt; // ��ϰ���

	for (int i = main_height - 2; i > 3; i--) {
		b_cnt = 0;
		for (int j = 1; j < main_width - 1; j++) {
			if (screen[i][j] == -2) b_cnt++;
		}
		if (b_cnt == main_width - 2) { // ������ ���� á����
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
	gotoxy(13, 4);
	printf("Score : %d", score);
}
void pause()
{
	getch();
}