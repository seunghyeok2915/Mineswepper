#include <iostream>
#include <time.h>
#include <Windows.h>
#include <conio.h>

using namespace std;

#define UP		72
#define DOWN	80
#define LEFT	75
#define RIGHT	77

const int MAPX = 20;
const int MAPY = 20;

const int MINECOUNT = 40;

const char NOMINE = '!';
const char HASMINE = '@';

const char OPENED = '#';
const char NOTOPENED = '$';

char mineMap[MAPY][MAPX];
char showMap[MAPY][MAPX];

int currentX, currentY;

void gotoXY(int x, int y)
{
	HANDLE hOut;
	COORD Cur;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(hOut, Cur);
}


void clrscr()
{
	system("cls");
}

void CreateMap()
{
	for (int y = 0; y < MAPY; y++)
	{
		for (int x = 0; x < MAPX; x++)
		{
			showMap[y][x] = NOTOPENED;
			mineMap[y][x] = NOMINE;
		}
	}

	srand((unsigned)time(NULL));

	for (int i = 0; i < MINECOUNT; i++)
	{
		int ranX = rand() % MAPX - 1;
		int ranY = rand() % MAPY - 1;

		if (mineMap[ranY][ranX] == NOMINE)
		{
			mineMap[ranY][ranX] = HASMINE;
		}
		else
		{
			--i;
		}
	}
}

int GetMineCountAround(int x, int y)
{
	int mineCnt = 0;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0) continue;
			if (mineMap[y + i][x + j] == HASMINE)
			{
				mineCnt++;
			}
		}
	}

	return mineCnt;
}

void DisplayMap()
{
	for (int y = 0; y < MAPY; y++)
	{
		for (int x = 0; x < MAPX; x++)
		{
			gotoXY(x * 2, y);

			if (showMap[y][x] == OPENED)
			{
				if (mineMap[y][x] == NOMINE)
				{
					cout << GetMineCountAround(x, y);
				}
				else if (mineMap[y][x] == HASMINE)
				{
					clrscr();
					cout << "Game Over";
					exit(0);
					break;
				}
			}
			else if (showMap[y][x] == NOTOPENED)
			{
				cout << "□";
			}
		}
	}

	gotoXY(currentX * 2, currentY);
	cout << "■";
}

void Move(int dir)
{
	int dir_x = 0, dir_y = 0;

	switch (dir)
	{
	case LEFT:
		dir_x = -1;
		break;
	case RIGHT:
		dir_x = 1;
		break;
	case UP:
		dir_y = -1;
		break;
	case DOWN:
		dir_y = 1;
		break;
	}

	currentX += dir_x;
	currentY += dir_y;
}

void OpenCurrentDir(int x, int y,bool first)
{
	showMap[y][x] = OPENED;

	if (mineMap[y][x] == HASMINE)
	{
		if (first)
		{
			clrscr();
			cout << "Game Over";
			exit(0);
		}
		else
		{
			return;
		}
	}

	if (GetMineCountAround(x, y) != 0)
	{
		if(!first)
			return;
	}

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			//if (i == 0 && j == 0) continue;
			if (GetMineCountAround(x, y) == 0)
			{
				if (showMap[y + i][x + j] == NOTOPENED)
				{
					OpenCurrentDir(x + j, y + i,false);
				}
			}
		}
	}
}

int main()
{
	int ch;
	CreateMap();
	DisplayMap();
	while (true)
	{
		ch = _getch();

		if (ch == 0xE0 || ch == 0)
		{
			ch = _getch();
			switch (ch)
			{
			case LEFT:
			case RIGHT:
			case UP:
			case DOWN:
				Move(ch);
				break;
			}
		}
		else
		{
			ch = tolower(ch);
			if (ch == 'f')
			{
				OpenCurrentDir(currentX, currentY,true);
			}
		}

		clrscr();
		DisplayMap();
		Sleep(10);
	}
}
