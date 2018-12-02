#include "pch.h"
#include "Library.h"
#include <thread>

using namespace std;

class enemy {
private:
	int x, y;
public:
	void setXY(int a, int b) {
		x = a; y = b;
	}
	void create() {

		lib::setColor(4);
		lib::setCursorPosition(x, y - 1);
		cout << " " << char(203) << " ";
		lib::setCursorPosition(x, y);
		cout << char(204) << char(206) << char(185);
		lib::setCursorPosition(x, y + 1);
		cout << " " << char(202) << " ";
	}
};

int main()
{
	lib::setFontSize(2, 2);
	lib::setConsoleResolution(1280, 720);
	lib::remove_scrollbar();
	enemy a;
	a.setXY(5, 5);
	a.create();
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

