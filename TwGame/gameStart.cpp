#include "pch.h"
#include "Library.h"
#include <thread>

const int fieldColor = 2 * 16;

using namespace std;

class field {
private:
	int pos[50][50];
public:
	void set(int x,int y) {
		pos[x][y] = 1;
	}

	void print() {
		lib::setColor(fieldColor);
		for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 50; j++) {
				cout << " ";
			}
			cout << endl;
		}
	}
};

class enemy {
private:
	int x, y;
public:
	void setXY(int a, int b) {
		x = a; y = b;
	}
	void del() {
		lib::setColor(fieldColor);
		for (int i = -1; i <= 1; i++) {
			lib::setCursorPosition(x-1, y+i);
			for (int j = -1; j <= 1; j++) {
				cout << " ";
			}
		}
	}
	void create() {
		//del();
		char ch = char(178);
		lib::setColor(4);
		lib::setCursorPosition(x, y - 1);
		cout << ch;
		lib::setCursorPosition(x - 1, y);
		cout << ch << ch << ch;
		lib::setCursorPosition(x, y + 1);
		cout << ch;
	}

	void move(int a,int b) {
		lib::setColor(4);
		if (a > x) {//desine
			lib::setCursorPosition(x, y - 1);
			cout << " ";
			lib::setCursorPosition(x-1, y);
			cout << " ";
			lib::setCursorPosition(x, y + 1);
			cout << " ";
		}
		else if (a < x) {//kaire
			lib::setCursorPosition(x, y - 1);
			cout << " ";
			lib::setCursorPosition(x + 1, y);
			cout << " ";
			lib::setCursorPosition(x, y + 1);
			cout << " ";
		}
		else if (b > y) {//zemyn
			lib::setCursorPosition(x-1, y);
			cout << " ";
			lib::setCursorPosition(x, y-1);
			cout << " ";
			lib::setCursorPosition(x+1, y);
			cout << " ";
		}
		else if (b < y) {//aukstyn
			lib::setCursorPosition(x - 1, y);
			cout << " ";
			lib::setCursorPosition(x, y + 1);
			cout << " ";
			lib::setCursorPosition(x + 1, y);
			cout << " ";
		}
		setXY(a, b);
		create();
	}
};

int main()
{
	lib::setFontSize(10, 10);
	lib::setConsoleResolution(1280, 720);
	lib::remove_scrollbar();
	ios_base::sync_with_stdio(false);
	field gameField;

	enemy a;
	enemy b;
	b.setXY(10, 10);
	a.setXY(5, 5);
	b.create();
	a.create(); 
	while (1) {
		for (int i = 0; i < 30; i++) {
			b.move(20, i);
			a.move(5, i);
			Sleep(20);
		}
		for (int i = 0; i < 30; i++) {
			a.move(i, 35);
			b.move(i, 23);
			Sleep(20);
		}
		for (int i = 30; i > 0; i--) {
			a.move(30, i);
			b.move(23, i);
			Sleep(20);
		}
		for (int i = 30; i > 0; i--) {
			a.move(i, 40);
			b.move(i, 60);
			Sleep(20);
		}
	}
	//cout<< "asdasda";
	getchar();
	return 0;
}
/*
10000000000000000000
11111000000000000000
00001000010000000000
00001111110111111000
*/
/*
enemy judejimas
sukurti kad enemy vaiksciotu pagal path
path editor
padaryti kad judetu keli enemy vienu metu

sukurti tower
tower saudymas
tower placinimas

*/

