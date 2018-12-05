#include "pch.h"
#include "Library.h"
#include <thread>
#include <iostream>

const int fieldColor = 6 * 16;
int laukas[50][50];
int spawnX, spawnY;
using namespace std;

void printLaukas() {
	lib::setColor(fieldColor);
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			if (laukas[i][j] == 1) {
				lib::setCursorPosition(i, j);
				cout << " ";
				/*for (int g = -1; g <= 1; g++) {
					lib::setCursorPosition(i-1,j+g);
					cout << "   ";
				}*/
			}
		}
		cout << endl;
	}
}

//pirmas create
//antras delete

/*
00000000*/
class levelEditor{
public:
	void print(int x,int y, int color) {
		lib::setColor(color);
		/*for (int i = -1; i <= 1; i++) {
			lib::setCursorPosition(x - 1, y + i);
			cout << "   ";
		}*/
		lib::setCursorPosition(x, y);
		cout << " ";
	}
	void start() {
		lib::setFontSize(30, 30);
		lib::setConsoleResolution(1280, 720);
		lib::clearscreen(15 * 16);
		lib::remove_scrollbar();
		lib::setCursorVisibility(false);
		//lib::cursorDraw(15 * 16, 0);
		while (1) { //enemy baze
			if (lib::mouseLeftClick()) {
				COORD pos = lib::getMousePosition();
				laukas[pos.X][pos.Y] = 3;
				spawnX = pos.X;
				spawnY = pos.Y;
				print(spawnX, spawnY, 0);
				break;
			}
		}

		while (1) { //path set
			if (lib::mouseLeftClick()) {
				COORD pos = lib::getMousePosition();
				laukas[pos.X][pos.Y] = 1;
				print(pos.X, pos.Y, 6*16);
			}
			else if (lib::mouseRightClick()) {
				break;
			}
		}

		ofstream fr("map.txt");
		for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 50; j++) {
				fr << laukas[i][j];
			}
			fr << endl;
		}

	}	
};


class enemy {
private:
	int x, y;
	bool virsu = false;
	bool apacia = false;
	bool kaire = false;
	bool desine = false;
	bool vertikalu = false;
	bool horizontalu = false;
public:
	friend class field;
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
	void create() {	//pirmineje stadijoje nezinome i kuria puse eiti todel tai reiia nustatyti
		if (laukas[x + 1][y] == 1) {	//desine
			desine = true;
			horizontalu = true;
		}
		else if (laukas[x - 1][y] == 1) {	//kaire
			kaire = true;
			horizontalu = true;
		}
		else if (laukas[x][y + 1] == 1) {	//virsu
			virsu = true;
			vertikalu = true;
		}
		else if (laukas[x][y - 1] == 1) {	//zemyn
			apacia = true;
			vertikalu = true;
		}
		print();
	}

	void print() {
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
		lib::setColor(fieldColor);
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
		print();
	}

	void checkVertical() {
		desine = false;
		kaire = false;
		horizontalu = false;
		vertikalu = true;
		if (laukas[x][y - 1] == 1) {
			virsu = true;
			move(x, y - 1);
		} else if(laukas[x][y + 1] == 1){
			apacia = true;
			move(x, y + 1);
		}
	}

	void checkHorizontal() {
		apacia = false;
		virsu = false;
		horizontalu = true;
		vertikalu = false;
		if (laukas[x + 1][y] == 1) {
			desine = true;
			move(x + 1, y);
		} else if (laukas[x - 1][y] == 1) {
			kaire = true;
			move(x - 1, y);
		}
	}

	void followPath() {
		if (horizontalu) {
			if (laukas[x + 1][y] == 1 && desine) {	//desine
				move(x + 1, y);	
			}
			else if (laukas[x - 1][y] == 1 && kaire) {	//kaire
				move(x - 1, y);
			}
			else if (laukas[x + 1][y] == 0 || laukas[x - 1][y] == 0) {
				checkVertical();//jeigu nei i kaire, nei i desine negali paeiti einama vertikaliai
			}
		}
		else if (vertikalu) {
			if (laukas[x][y - 1] == 1 && virsu) {	//virsu
				move(x, y - 1);
			}
			else if (laukas[x][y + 1] == 1 && apacia) {	//zemyn
				move(x, y + 1);
			}
			else if (laukas[x][y - 1] == 0 || laukas[x][y + 1] == 0) {
				checkHorizontal();	//jeigu nei i virsu, nei i apacia negali eiti einama horizontaliai
			}
		}
		//jei bus nenurodyta pabaiga, vaikscios pirmyn atgal, nes checkHorizontal padarys vertical = false ir kitu ciklu checkVertical ras praeita reiksme ir eis atgal
	}
};

int main()
{

	
	lib::setFontSize(10, 10);
	lib::setConsoleResolution(1280, 720);
	lib::clearscreen(15 * 16);
	lib::remove_scrollbar();
	lib::setCursorVisibility(false);

	ios_base::sync_with_stdio(false);//pagreitina isvedima
	cin.tie(NULL);



	/*ofstream fr("map.txt");
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			fr << "0 ";
		}
		fr << endl;
	}*/

	

	levelEditor f;
	f.start();
	ifstream fd("map.txt");
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			fd >> laukas[j][i];
		}
	}
	lib::setFontSize(10, 10);
	lib::setConsoleResolution(1280, 720);
	lib::clearscreen(15 * 16);
	lib::remove_scrollbar();
	lib::setCursorVisibility(false);
	printLaukas();

	enemy a;
	a.setXY(spawnX, spawnY);
	a.create();
	a.followPath();
	a.followPath();
	a.followPath();
	a.followPath();
	a.followPath();
	a.followPath();
	a.followPath();
	a.followPath();
	a.followPath();
	a.followPath();
	a.followPath();
	a.followPath();
	enemy b;
	b.setXY(spawnX, spawnY);
	b.create();
	b.followPath();
	b.followPath();
	b.followPath();
	b.followPath();
	b.followPath();

	enemy c;
	c.setXY(spawnX, spawnY);
	c.create();

	

	while (1) {
		a.followPath();
		b.followPath();
		c.followPath();
		Sleep(20);
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
enemy judejimas  --- COMPLETE
sukurti kad enemy vaiksciotu pagal path --- COMPLETE
path editor
padaryti kad judetu keli enemy vienu metu

sukurti tower
tower saudymas
tower placinimas

*/

