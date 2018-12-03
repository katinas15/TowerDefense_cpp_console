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
		del();
		char ch = char(178);
		lib::setColor(4);
		lib::setCursorPosition(x + 1, y - 1);
		cout << ch;
		lib::setCursorPosition(x, y);
		cout << ch << ch << ch;
		lib::setCursorPosition(x + 1, y + 1);
		cout << ch;
	}

	void move(int a,int b) {
		if (x > a) { //kaire

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

	field gameField;

	enemy a;
	a.setXY(5, 0);
	a.create(); 
	for (int i = 0; i < 30; i++) {
		a.move(i, 5);
		Sleep(100);
	}
	//cout<< "asdasda";
	getchar();
	return 0;
}
/*
enemy judejimas
sukurti kad enemy vaiksciotu pagal path
path editor
padaryti kad judetu keli enemy vienu metu

sukurti tower
tower saudymas
tower placinimas

*/

