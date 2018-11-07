#include "pch.h"
#include <iostream>
#include <windows.h>

using namespace std;

int main()
{
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

	COORD coord;

	coord.X = 0;
	coord.Y = 0;
	int x = coord.X;
	int y = coord.Y;
	while (1) {
		ReadConsoleInput(hin, &InputRecord, 1, &Events);
		switch (InputRecord.EventType) {
		case MOUSE_EVENT: // mouse input 

			SetConsoleTextAttribute(hConsole, 15);
			for (int i = -5; i < 5; i++) {
				for (int j = -5; j < 5; j++) {
					coord.X = x + i;
					coord.Y = y + j;
					SetConsoleCursorPosition(hout, coord);
					cout << " ";
				}
			}

			x = InputRecord.Event.MouseEvent.dwMousePosition.X;
			y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			SetConsoleTextAttribute(hConsole, 196);
			for (int i = -5; i < 5; i++) {
				for (int j = -5; j < 5; j++) {
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


