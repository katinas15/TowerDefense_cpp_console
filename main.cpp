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
	/*HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND console = GetConsoleWindow();
	HDC hdc = GetDC(console);
	RECT r = { 0,0,	500,500 };
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.top, r.left, r.bottom - r.top, r.right - r.left, TRUE);
	*/

	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InputRecord;
	DWORD Events;
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 25;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hout, &cci);
	SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

	/*	CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 10;                   // Width of each character in the font
		cfi.dwFontSize.Y = 10;                  // Height
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
		SetConsoleTextAttribute(hConsole, 196);*/
	clearscreen();
	//initialize perkelti i kita faila
	//---------------------------------
	//---------------------------------
	//---------------------------------
	//---------------------------------
	//---------------------------------

		//Get a console handle
	HWND myconsole = GetConsoleWindow();
	//Get a handle to device context
	HDC mydc = GetDC(myconsole);

	ReadConsoleInput(hin, &InputRecord, 1, &Events);
	//COORD coord;
	COLORREF COLOR = RGB(255, 255, 255);
	POINT p;
	GetCursorPos(&p);
	int x = p.x;
	int y = p.y;
	while (1) {
		GetCursorPos(&p);
		switch (InputRecord.EventType) {
		case MOUSE_EVENT: // mouse input 

			//eina i kaire
			if (x > InputRecord.Event.MouseEvent.dwMousePosition.X) {
				COLOR = RGB(0, 0, 0);
				for (int j = x - InputRecord.Event.MouseEvent.dwMousePosition.X + 2; j >= 0; j--) {
					p.x = x + j;
					for (int i = -50; i < 50; i++) {
						p.y = y + i;
						SetPixel(mydc, p.x, p.y, COLOR);
					}
				}


			}

			//eina i desine
			if (x < InputRecord.Event.MouseEvent.dwMousePosition.X) {
				COLOR = RGB(0, 0, 0);
				for (int j = -1 * (InputRecord.Event.MouseEvent.dwMousePosition.X - x) - 2; j <= 0; j++) {
					p.x = x + j;
					for (int i = -50; i < 50; i++) {
						p.y = y + i;
						SetPixel(mydc, p.x, p.y, COLOR);
					}
				}
			}

			//eina aukstyn
			if (y > InputRecord.Event.MouseEvent.dwMousePosition.Y) {
				COLOR = RGB(0, 0, 0);
				for (int j = y - InputRecord.Event.MouseEvent.dwMousePosition.Y + 2; j >= 0; j--) {
					p.y = y + j;
					for (int i = -50; i < 50; i++) {
						p.x = x + i;
						SetPixel(mydc, p.x, p.y, COLOR);
					}
				}
			}

			//eina zemyn
			if (y < InputRecord.Event.MouseEvent.dwMousePosition.Y) {
				COLOR = RGB(0, 0, 0);
				for (int j = -1 * (InputRecord.Event.MouseEvent.dwMousePosition.Y - y) - 2; j <= 0; j++) {
					p.y = y + j;
					for (int i = -50; i < 50; i++) {
						p.x = x + i;
						SetPixel(mydc, p.x, p.y, COLOR);
					}
				}
			}

			/*	SetConsoleTextAttribute(hConsole, 15);
				for (int i = -2; i < 2; i++) {
					for (int j = -2; j < 2; j++) {
						p.x = x + i;
						p.y = y + j;
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
			COLOR = RGB(255, 255, 255);
			for (int i = -10; i < 10; i++) {
				for (int j = -10; j < 10; j++) {
					p.x = x + i;
					p.y = y + j;
					SetPixel(mydc, p.x, p.y, COLOR);
				}
			}


			break;
			 

		}
	}

}


