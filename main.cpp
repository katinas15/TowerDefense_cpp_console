#include "pch.h"
#include <iostream>
#include <windows.h>

using namespace std;

void clearscreen() {
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hConsole, coordScreen);
	return;
}

int main()
{
	//initialize perkelti i kita faila
	//---------------------------------
	//---------------------------------
	//---------------------------------
	//---------------------------------
	//---------------------------------
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND console = GetConsoleWindow();
	RECT r = { 0,0,	500,500 };
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.top, r.left, r.bottom - r.top, r.right - r.left, TRUE);

	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InputRecord;
	DWORD Events;
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 25;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hout, &cci);
	SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 10;                   // Width of each character in the font
	cfi.dwFontSize.Y = 10;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	SetConsoleTextAttribute(hConsole, 196);
	clearscreen();
	//initialize perkelti i kita faila
	//---------------------------------
	//---------------------------------
	//---------------------------------
	//---------------------------------
	//---------------------------------

	/*int grid[100];
	for (int i = 0; i < 500; i+=5) {
		for (int j = 0; j < 500; j+=5) {
			grid[i] = j;
		}
	}*/

	COORD coord;
	coord.X = 0;
	coord.Y = 0;
	int x = coord.X;
	int y = coord.Y;
	while (1) {
		ReadConsoleInput(hin, &InputRecord, 1, &Events);
		switch (InputRecord.EventType) {
		case MOUSE_EVENT: // mouse input 

			if (x > InputRecord.Event.MouseEvent.dwMousePosition.X) {
				SetConsoleTextAttribute(hConsole, 15);
				int j = x - InputRecord.Event.MouseEvent.dwMousePosition.X;
				coord.X = x + 1;
				for (int i = -2; i < 2; i++) {
					coord.Y = y + i;
					SetConsoleCursorPosition(hout, coord);
					cout << " ";
				}
			}

			else if (x < InputRecord.Event.MouseEvent.dwMousePosition.X) {
				SetConsoleTextAttribute(hConsole, 15);
				coord.X = x - 2;
				for (int i = -2; i < 2; i++) {
					coord.Y = y + i;
					SetConsoleCursorPosition(hout, coord);
					cout << " ";
				}
			}

			else if (y > InputRecord.Event.MouseEvent.dwMousePosition.Y) {
				SetConsoleTextAttribute(hConsole, 15);
				coord.Y = y + 1;
				for (int i = -2; i < 2; i++) {
					coord.X = x + i;
					SetConsoleCursorPosition(hout, coord);
					cout << " ";
				}
			}

			else if (y < InputRecord.Event.MouseEvent.dwMousePosition.Y) {
				SetConsoleTextAttribute(hConsole, 15);
				coord.Y = y - 2;
				for (int i = -2; i < 2; i++) {
					coord.X = x + i;
					SetConsoleCursorPosition(hout, coord);
					cout << " ";
				}
			}
		/*	SetConsoleTextAttribute(hConsole, 15);
			for (int i = -2; i < 2; i++) {
				for (int j = -2; j < 2; j++) {
					coord.X = x + i;
					coord.Y = y + j;
					SetConsoleCursorPosition(hout, coord);
					cout << " ";
				}
			}*/

			//0000000000000000
			//0000000000000000
			//000000000YYYX000
			//0000000000000000
			//0000000000000000

			x = InputRecord.Event.MouseEvent.dwMousePosition.X;
			y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			SetConsoleTextAttribute(hConsole, 196);
			for (int i = -2; i < 2; i++) {
				for (int j = -2; j < 2; j++) {
					coord.X = x + i;
					coord.Y = y + j;
					SetConsoleCursorPosition(hout, coord);
					cout << " ";
				}
			}

			break;


		}
	}

	FlushConsoleInputBuffer(hin);
}


