#include "pch.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <functional>
#include <vector>
#pragma warning(disable:4996)
using namespace std;



void cursor() {
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
	//clearscreen();
	//initialize perkelti i kita faila
	//---------------------------------
	//---------------------------------
	//---------------------------------
	//---------------------------------
	//---------------------------------


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

namespace lib {

	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InputRecord;
	DWORD Events;
	HWND console = GetConsoleWindow();
	RECT r;
	CONSOLE_CURSOR_INFO cci;
	CONSOLE_FONT_INFOEX cfi;
	HDC hdc = GetDC(console);

	void clearscreen(int color) {
		COORD coordScreen = { 0, 0 };
		DWORD cCharsWritten;
		DWORD dwConSize;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		SetConsoleTextAttribute(hConsole, color);
		GetConsoleScreenBufferInfo(hConsole, &csbi);
		dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
		FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
		GetConsoleScreenBufferInfo(hConsole, &csbi);
		FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
		SetConsoleCursorPosition(hConsole, coordScreen);
	}

	void setColor(int color) {	//nustatoma teskto spalva
		SetConsoleTextAttribute(hConsole, color);
	}

	void printText(int x, int y, string text, int color) {	//isvedamas teksta i nurodyta pozicija, pasirinktos spalvos
		SetConsoleTextAttribute(hConsole, color);
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(hout, coord);
		cout << text;
	}

	void remove_scrollbar() //isjungiamas consoles scroll bar
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO info;
		GetConsoleScreenBufferInfo(handle, &info);
		COORD new_size =
		{
			info.srWindow.Right - info.srWindow.Left + 1,
			info.srWindow.Bottom - info.srWindow.Top + 1
		};
		SetConsoleScreenBufferSize(handle, new_size);
	}

	COORD getMousePosition() {	//nustatoma peles pozicija lange
		ReadConsoleInput(hin, &InputRecord, 1, &Events);
		coord.X = InputRecord.Event.MouseEvent.dwMousePosition.X;
		coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
		return coord;
	}

	void setConsoleResolution(int x, int y) {	//nustatomas konsoles dydis
		GetWindowRect(console, &r); //stores the console's current dimensions
		MoveWindow(console, r.left, r.top, x, y, TRUE);
	}

	void goFullscreen(){	//nustatomas fullscreen konsoles dydis
		SetConsoleDisplayMode(hConsole, CONSOLE_FULLSCREEN_MODE, NULL);
	}

	bool mouseLeftClick() {	//nustatoma ar buvo paspaustas pirmas mouse
		ReadConsoleInput(hin, &InputRecord, 1, &Events);
		if (GetKeyState(VK_LBUTTON) < 0)	//jei 1-mouse mygtukas laivas pastatomas
		{
			return true;
		}
		return false;
	}

	bool mouseRightClick() {//nustatoma ar buvo paspaustas antras mouse
		ReadConsoleInput(hin, &InputRecord, 1, &Events);
		if (GetKeyState(VK_LBUTTON) > 0)	//jei 2-mouse mygtukas laivas pastatomas
		{
			return true;
		}
		return false;
	}

	COORD mousePosition() {	//nustatoma mouse pozicija lange
		ReadConsoleInput(hin, &InputRecord, 1, &Events);
		coord.X = InputRecord.Event.MouseEvent.dwMousePosition.X;
		coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
		return coord;
	}

	bool mouseEvent() {	//nustatoma ar ivyko kazkoks mouse event, pvz.: sujudejo mouse arba paspaustas mouse mygtukas
		ReadConsoleInput(hin, &InputRecord, 1, &Events);
		if (InputRecord.EventType == MOUSE_EVENT)  return true;
		else return false;
	}

	void setCursorPosition(int x, int y) {//nustatoma kursoriaus pozicija
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(hout, coord);
	}

	void setCursorVisibility(bool visibility) {	//nustatoma ar kursoriu matomas ar nematomas
		cci.dwSize = 25;
		cci.bVisible = visibility;
		SetConsoleCursorInfo(hout, &cci);
		SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT); 
	}

	void setFontSize(int x, int y) {	//teksto fonto dydis
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = x;                   // Width of each character in the font
		cfi.dwFontSize.Y = y;                  // Height
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	}

	void printPixel(int x, int y, COLORREF color) {
		SetPixel(hdc, x, y, color);
	}

	void printBMP(const char* filename, int x, int y) {
		//https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file
		// read BMP file
		FILE* f = fopen(filename, "rb");

		if (f == NULL)
			throw "Argument Exception";

		unsigned char info[54];
		fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

		// extract image height and width from header
		int width = *(int*)&info[18];
		int height = *(int*)&info[22];

		int row_padded = (width * 3 + 3) & (~3);
		unsigned char* data = new unsigned char[row_padded];
		unsigned char tmp;

		int tempx = x, tempy = y;
		for (int i = 0; i < height; i++)
		{
			tempx = x;
			fread(data, sizeof(unsigned char), row_padded, f);
			for (int j = 0; j < width * 3; j += 3)
			{
				// Convert (B, G, R) to (R, G, B)
				tmp = data[j];
				data[j] = data[j + 2];
				data[j + 2] = tmp;
				printPixel(tempx, tempy, RGB((int)data[j], (int)data[j + 1], (int)data[j + 2]));
				tempx++;
			}
			tempy++;
		}
		fclose(f);

	/*	int tempx = x, tempy = y;
		for (int i = 0; i < height; i++)
		{
			tempx = x;
			for (int j = 0; j < width*3; j+=3)
			{
				// Now data should contain the (R, G, B) values of the pixels.
				//The color of pixel (i, j) is stored at data[3 * (i * width + j)],
				//data[3 * (i * width + j) + 1] and data[3 * (i * width + j) + 2].
				//cout << "R: " << (int)data[j] << " G: " << (int)data[j + 1] << " B: " << (int)data[j + 2] << endl;
				printPixel(tempx, tempy, RGB((int)data[j], (int)data[j + 1], (int)data[j + 2]));
				tempx++;
			}
			tempy++;
		}*/

		
		
	}

};

class table {	//lenteles klase
private:
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
public:

	int x;
	int y;
	int width;
	int height;
	int rows;
	int color;
	string text[10000];
	int borderType;

	void create() {	//sukuriama lentele
		remove();
		lib::setColor(color);
		int tempy = y;

		//uzspalvinama table vieta/ uzprintinami tusti langeliai kur bus table
		for (int r = 0; r < rows; r++) {
			lib::setCursorPosition(x, tempy);
			for (int i = 0; i <= height; i++) {
				lib::setCursorPosition(x, tempy + i);
				for (int j = 0; j <= width; j++) {
					cout << " ";
				}
			}
			tempy += height;
		}

		char v, b, vk, vd, ak, ad;
		if (borderType == 0) {	//jei krastu nera tiesiog isvedamas tesktas
			for (int r = 0; r < rows; r++) {
				lib::setCursorPosition(x + width / 2 - text[r].length() / 2, y + height / 2 + height * r);
				cout << text[r];
			}
		}
		else {
			if (borderType == 1) {	//viengubas krastas
				v = char(196);
				b = char(179);
				vk = char(218);
				vd = char(191);
				ak = char(192);
				ad = char(217);
			}
			else if (borderType == 2) {	//dvigubas krastas
				v = char(205);
				b = char(186);
				vk = char(201);
				vd = char(187);
				ak = char(200);
				ad = char(188);
			}


			lib::setCursorPosition(x, y);
			for (int i = 0; i <= width; i++) {	//virsus
				cout << v;
			}

			tempy = y;
			for (int r = 0; r < rows; r++) {
				lib::setCursorPosition(x, tempy + height);
				for (int i = 0; i <= width; i++) {	//apacia
					cout << v;
				}

				for (int i = tempy; i <= tempy + height; i++) {	//kaire
					lib::setCursorPosition(x, i);
					cout << b;
				}

				for (int i = tempy; i <= tempy + height; i++) {	//desine
					lib::setCursorPosition(x + width, i);
					cout << b;
				}

				tempy += height;
			}

			//kampai
			lib::setCursorPosition(x, y);
			cout << vk;
			lib::setCursorPosition(x + width, y);
			cout << vd;
			lib::setCursorPosition(x, y + height * rows);
			cout << ak;
			lib::setCursorPosition(x + width, y + height * rows);
			cout << ad;

			//tekstas
			for (int r = 0; r < rows; r++) {
				lib::setCursorPosition(x + width / 2 - text[r].length() / 2, y + height / 2 + height * r);
				cout << text[r];
			}
		}
	}

	void remove() {	//lentele istrinama !!!!!!!!!!!!!!!!!!!!reikia fix del spalvos
		//lentele istrinama !!!!!!!!!!!!!!!!!!!!reikia fix del spalvos
		//lentele istrinama !!!!!!!!!!!!!!!!!!!!reikia fix del spalvos
		//lentele istrinama !!!!!!!!!!!!!!!!!!!!reikia fix del spalvos
		//lentele istrinama !!!!!!!!!!!!!!!!!!!!reikia fix del spalvos
		//lentele istrinama !!!!!!!!!!!!!!!!!!!!reikia fix del spalvos
		//lentele istrinama !!!!!!!!!!!!!!!!!!!!reikia fix del spalvos
		//lentele istrinama !!!!!!!!!!!!!!!!!!!!reikia fix del spalvos
		lib::setColor(15);
		int tempy = y;

		//uzspalvinama table vieta
		for (int r = 0; r < rows; r++) {
			lib::setCursorPosition(x, tempy);
			for (int i = 0; i <= height; i++) {
				lib::setCursorPosition(x, tempy + i);
				for (int j = 0; j <= width; j++) {
					cout << " ";
				}
			}
			tempy += height;
		}
	}

	void set(int sx, int sy, int swidth, int sheight, int scolor,int srows, int sborderType) {
		/*istrinama sena lentele ir sukuriama nauja lentele su naujais parametrais*/
		remove();
		x = sx;
		y = sy;
		width = swidth;
		height = sheight;
		color = scolor;
		rows = srows;
		borderType = sborderType;
		create();
	}
};

class langas {	//uzspalvinama pasirinkta vieta
private:
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

public:
	int x;
	int y;
	int width;
	int height;
	int color;
	int borderType;

	void create() {
		remove();
		lib::setColor(color);
		int tempy = y;

		//uzspalvinama table vieta
		lib::setCursorPosition(x, tempy);
		for (int i = 0; i <= height; i++) {
			lib::setCursorPosition(x, tempy + i);
			for (int j = 0; j <= width; j++) {
				cout << " ";
			}
		}
		
		char v, b, vk, vd, ak, ad;
		if (borderType == 1) {//viengubas krastas
			v = char(196);
			b = char(179);
			vk = char(218);
			vd = char(191);
			ak = char(192);
			ad = char(217);
		}
		else if (borderType == 2) {//dvigubas krastas
			v = char(205);
			b = char(186);
			vk = char(201);
			vd = char(187);
			ak = char(200);
			ad = char(188);
		}

		if (borderType != 0) { // jei krastu nera toliau vnieko nereikia daryti
			lib::setCursorPosition(x, y);
			for (int i = 0; i <= width; i++) {	//virsus
				cout << v;
			}

			tempy = y;
			lib::setCursorPosition(x, tempy + height);
			for (int i = 0; i < width; i++) {	//apacia
				cout << v;
			}

			for (int i = tempy; i < tempy + height; i++) {	//kaire
				lib::setCursorPosition(x, i);
				cout << b;
			}

			for (int i = tempy; i < tempy + height; i++) {	//desine
				lib::setCursorPosition(x + width, i);
				cout << b;
			}

			tempy += height;

			//kampai
			lib::setCursorPosition(x, y);
			cout << vk;
			lib::setCursorPosition(x + width, y);
			cout << vd;
			lib::setCursorPosition(x, y + height);
			cout << ak;
			lib::setCursorPosition(x + width, y + height);
			cout << ad;
		}

	}

	void remove() {
		lib::setColor(15);
		int tempy = y;

		//uzspalvinama table vieta
		lib::setCursorPosition(x, tempy);
		for (int i = 0; i <= height; i++) {
			lib::setCursorPosition(x, tempy + i);
			for (int j = 0; j <= width; j++) {
				cout << " ";
			}
		}
	}

	void set(int sx, int sy, int swidth, int sheight, int scolor, int sborderType) {
		/*istrinama sena lentele ir sukuriama nauja lentele su naujais parametrais*/
		remove();
		x = sx;
		y = sy;
		width = swidth;
		height = sheight;
		color = scolor;
		borderType = sborderType;
		create();
	}
};

struct clickableObject {	//nurodoma kokioje pozicijoje galima paspasti objekta ir kokia funkcija jis atliks
	int topLeftX, topLeftY;
	int bottomRightX, bottomRightY;
	string text = "";
	function<void()> funkcija;
};

class menu {
private:
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
public:
	int x;
	int y;
	int width;
	int height;
	int rows;
	int color;
	string text[10000];
	int borderType;
	clickableObject * object;

	void create() {
		remove();

		object = new clickableObject[rows];	//nustatomos meniu lenteliu pasirinkimai, kad juos butu galima paspasti su mouse ir ivyktu funkcija
		for (int i = 0; i < rows; i++) {
			object[i].topLeftX = x;
			object[i].topLeftY = y + height*i ;
			object[i].bottomRightX = x + width;
			object[i].bottomRightY = y + height*(i+1);
		}

		lib::setColor(color);
		int tempy = y;

		//uzspalvinama table vieta
		for (int r = 0; r < rows; r++) {
			lib::setCursorPosition(x, tempy);
			for (int i = 0; i <= height; i++) {
				lib::setCursorPosition(x, tempy + i);
				for (int j = 0; j <= width; j++) {
					cout << " ";
				}
			}
			tempy += height;
		}

		char v, b, vk, vd, ak, ad;
		if (borderType == 0) {//jei krastu nera
			for (int r = 0; r < rows; r++) {
				lib::setCursorPosition(x + width / 2 - text[r].length() / 2, y + height / 2 + height * r);
				cout << text[r];
			}
		}
		else {
			if (borderType == 1) {//viengubas krastas
				v = char(196);
				b = char(179);
				vk = char(218);
				vd = char(191);
				ak = char(192);
				ad = char(217);
			}
			else if (borderType == 2) {//dvigubas krastas
				v = char(205);
				b = char(186);
				vk = char(201);
				vd = char(187);
				ak = char(200);
				ad = char(188);
			}


			lib::setCursorPosition(x, y);
			for (int i = 0; i <= width; i++) {	//virsus
				cout << v;
			}

			tempy = y;
			for (int r = 0; r < rows; r++) {
				lib::setCursorPosition(x, tempy + height);
				for (int i = 0; i <= width; i++) {	//apacia
					cout << v;
				}

				for (int i = tempy; i <= tempy + height; i++) {	//kaire
					lib::setCursorPosition(x, i);
					cout << b;
				}

				for (int i = tempy; i <= tempy + height; i++) {	//desine
					lib::setCursorPosition(x + width, i);
					cout << b;
				}

				tempy += height;
			}

			//kampai
			lib::setCursorPosition(x, y);
			cout << vk;
			lib::setCursorPosition(x + width, y);
			cout << vd;
			lib::setCursorPosition(x, y + height * rows);
			cout << ak;
			lib::setCursorPosition(x + width, y + height * rows);
			cout << ad;

			//tekstas
			for (int r = 0; r < rows; r++) {
				lib::setCursorPosition(x + width / 2 - text[r].length() / 2, y + height / 2 + height * r);
				cout << text[r];
			}
		}
	}

	void check() {//tikrinama ar buvo paspausta ar pele uzvesta ant kazkurio is meniu lango
		for (int r = 0; r < rows; r++) {	//tikrinama ant kurio lango uzvesta
			COORD temp = lib::getMousePosition();	//kai hover pasikeicia spalva
			if (temp.X > object[r].topLeftX && temp.Y > object[r].topLeftY && object[r].bottomRightX > temp.X && object[r].bottomRightY > temp.Y) {
				lib::setColor(16*15);	//invert !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				//invert !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				//invert !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				
				for (int i = 1; i < height; i++) {//pakeiciama spalva/ hover state
					lib::setCursorPosition(object[r].topLeftX + 1, object[r].topLeftY + i);
					for (int j = 1; j < width; j++) {
						cout << " ";
					}
				}
				//kitos spalvos tekstas
				lib::setCursorPosition(object[r].topLeftX + width / 2 - text[r].length() / 2, object[r].topLeftY + height / 2);
				cout << text[r];

				//kol mouse hoverina palikti
				while (temp.X > object[r].topLeftX && temp.Y > object[r].topLeftY && object[r].bottomRightX > temp.X && object[r].bottomRightY > temp.Y) {
					temp = lib::getMousePosition();
					if (lib::mouseLeftClick()) object[r].funkcija(); //jei buvo paspausta ivykdyti funkcija
				}
				//kai hover nebera istrinamas hover state ir paliekama kaip buvo
				lib::setColor(color);
				for (int i = 1; i < height; i++) {
					lib::setCursorPosition(object[r].topLeftX + 1, object[r].topLeftY + i);
					for (int j = 1; j < width; j++) {
						cout << " ";
					}
				}

				lib::setCursorPosition(object[r].topLeftX + width / 2 - text[r].length() / 2, object[r].topLeftY + height / 2);
				cout << text[r];

			}
		}
	}

	void remove() {
		lib::setColor(15);
		int tempy = y;

		//uzspalvinama table vieta
		for (int r = 0; r < rows; r++) {
			lib::setCursorPosition(x, tempy);
			for (int i = 0; i <= height; i++) {
				lib::setCursorPosition(x, tempy + i);
				for (int j = 0; j <= width; j++) {
					cout << " ";
				}
			}
			tempy += height;
		}
	}

	void set(int sx, int sy, int swidth, int sheight, int scolor, int srows, int sborderType) {
		/*istrinama sena lentele ir sukuriama nauja lentele su naujais parametrais*/
		remove();
		x = sx;
		y = sy;
		width = swidth;
		height = sheight;
		color = scolor;
		rows = srows;
		borderType = sborderType;
		create();
	}
};




int main()
{
	//lib::clearscreen(9 + 10 * 16);	//istrina spalva ir nuspalvina ekrana i nurodyta spalva
	lib::remove_scrollbar(); //nuima is console scroll bar	
	lib::setFontSize(10, 20);	//fonto dydis
	//lib::goFullscreen();	//fullscreen
	lib::printText(30, 20, "nibba", 2+16*4); //x,y,string,color
	//COORD a = lib::getMousePosition();	//get mouse position
	//lib::setConsoleResolution(1280, 720); // set resolution
//	lib::setCursorPosition(a.X,a.Y);
	lib::setCursorVisibility(false);
	

	/*----------------------------------
	-------------------------------------
	-----SKIRTINGI BUDAI KAIP GALIMA SUKURTI LANGA, TABLE AR MENIU*/

	langas a;	//paprastas langas,   x,y,width,height,color,bordertype
	a.x = 20;
	a.y = 20;
	a.width = 10;
	a.height = 10;
	a.color = 15;
	a.borderType = 2;
	a.create();
	//a.remove();
	a.set(25, 20, 10, 10, 18, 2);	//arba galima sukurti su set

	menu b;	//menu,   x,y,width,height,color,rows,text,bordertype, object
	b.x = 0;
	b.y = 0;
	b.width = 20;
	b.height = 2;
	b.color = 15;
	b.rows = 5;
	b.text[0] = "start";
	b.text[1] = "quit game";
	b.text[2] = "options";
	b.text[3] = "credits";
	b.text[4] = "extra";
	b.borderType = 2;
	b.create();
	//b.remove();
	b.set(10, 0, 20, 2, 15, 5, 2);	//arba su set


	table c;	//table,   x,y,width,height,color,rows,text,bordertype
	c.text[0] = "start";
	c.text[1] = "quit game";
	c.text[2] = "options";
	c.set(60, 10, 20, 2, 1 + 16 * 15, 3, 1);//x,y,width,height,color,rows,bordertype


	//lib::goFullscreen();	//fullscreen
	
	
	/*clickableObject A[100];
	A[0].funkcija = bind(&lib::printText,50,10,"cat",15);	//kad ivesti parametra reikia bind
	A[0].funkcija();
	cin.get();*/
	lib::printBMP("nib.bmp", 300, 300);
	b.object[0].funkcija = bind(&lib::printText, 50, 10, "cat", 15);	//objecktui priskiriama funkcija print text
	b.object[1].funkcija = bind(&lib::printText, 60, 20, "gameover u ded", 15);
	while (1) {
		if(lib::mouseEvent()) b.check();
	}
	
}



/*
set text to position -- COMPLETE
set console size	-- T

table - COMPLETE
clickable objects - COMPLETE
clickable table - COMPLETE

kartais reikie keliu click kad veiktu object
color pallet
import bmp image
right click - open/create menu
menu

cursor/no flicker
threads
*/
