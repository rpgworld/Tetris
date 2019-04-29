#include <stdio.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>

void main()
{
	int i = 1;
	srand(time(NULL));
	while (1) {

		int r = (rand() % 7) + 1;
		printf("%d ¹øÂ° °ª : %d \n", i, r);
		i++;
		Sleep(1000);
	}
}
