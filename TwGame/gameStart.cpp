#include "pch.h"
#include "Library.h"
#include <thread>
#include <iostream>

const int fieldColor = 15 * 16;
int laukas[50][33];
int spawnX = 2, spawnY = 2;
int baseX = 2, baseY = 2;
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

class levelEditor{
private:
	int block;	
	variableText text;
	int pathColor = 6 * 16;
	int baseColor = 5 * 16;
	int spawnColor = 0;

public:
	void print(int x,int y, int color) {	//paiso blokus nurodytoje pozicijoje
		lib::setColor(color);
		lib::setCursorPosition(x, y);
		cout << " ";
	}
	bool check(int x, int y) {
		if (x < 1 || y < 1 || x > 49 || y > 32) return false;	//tikrina lauko ribas ir jei tinka isveda true
		else return true;
	}

	void setBlock(int a) {//funkcija isveda koks yra siuo menu pasirinktas blokas
		block = a;
		langas lang;
		
		if (a == 3) {		
			text.set(53, 28, "Spawn point", fieldColor, 0, true, false);
			lang.set(56, 30, 2, 2, spawnColor, 0);	//taip pat nurodoma blokelio spalva desineje apacioje
		}
		else if (a == 2) {
			text.set(53, 28, "Base", fieldColor, 0, true, false);
			lang.set(56, 30, 2, 2, baseColor, 0);
		}
		else if (a == 1) {
			text.set(53, 28, "Path", fieldColor, 0, true, false);
			lang.set(56, 30, 2, 2, pathColor, 0);
		}
	}

	void saveToFile() {
		ofstream fr("map.txt");
		for (int i = 0; i < 33; i++) {
			for (int j = 0; j < 50; j++) {
				fr << laukas[j][i];
			}
			fr << endl;
		}
	}

	void start() {
		lib::setFontSize(20, 20);
		lib::setConsoleResolution(1280, 720);
		lib::clearscreen(15 * 16);
		lib::remove_scrollbar();
		lib::setCursorVisibility(false);
		lib::printText(52, 26, "Selected: ", fieldColor);	//koks yra pasirinktas blokas pasako

		langas lang;	//level editor remeliai
		lang.set(0, 0, 50, 33, fieldColor, 3);

		menu edit;	//level editor pasirinkimu meniu
		edit.setText(0, "Spawn point");
		edit.setText(1, "Base");
		edit.setText(2, "Path");
		edit.setText(3, "Save to file");
		edit.setText(4, "Start game");
		edit.setText(5, "Main menu");
		edit.set(50, 0, 14, 4, fieldColor,6, 3);
		edit.setFunction(0, bind(&levelEditor::setBlock, this, 3));	//paspaudus nusistato norimas blokas
		edit.setFunction(1, bind(&levelEditor::setBlock, this, 2));
		edit.setFunction(2, bind(&levelEditor::setBlock, this, 1));
		edit.setFunction(3, bind(&levelEditor::saveToFile, this));
		while (1) { //enemy baze
			edit.check();	//tikrina meniu pasirinkimus, ar buvo paspausta ant meniu
			if (lib::mouseLeftClick()) {	
				COORD pos = lib::getMousePosition();
				if (check(pos.X, pos.Y)) {	//tikrinama ar neuzeina uz ribu
					if (block == 3) {
						print(spawnX, spawnY, fieldColor);	//istrinamas praeitas spawn pointas, nes gali buti tik vienas spawn
						laukas[spawnX][spawnY] = 0;	//nutrinama reiksme faile
						laukas[pos.X][pos.Y] = 3;	//nustatoma 3 del failo saugojimo
						spawnX = pos.X;	//nustatomos naujos spawn koordinates
						spawnY = pos.Y;
						print(spawnX, spawnY, 0);	//isvedamas
					}
					if (block == 1) {
						COORD pos = lib::getMousePosition();
						if (check(pos.X, pos.Y)) {
							laukas[pos.X][pos.Y] = 1;
							print(pos.X, pos.Y, pathColor);
						}
					}
					if (block == 2) {
						print(baseX, baseY, fieldColor);	//gali buti tik viena baze
						laukas[baseX][baseY] = 0;
						laukas[pos.X][pos.Y] = 2;
						baseX = pos.X;
						baseY = pos.Y;
						print(baseX, baseY, baseColor);
					}
				}
			}
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
	lib::setFontSize(15, 15);
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
	lib::setFontSize(14, 14);
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
enemy judejimas  --- COMPLETE
sukurti kad enemy vaiksciotu pagal path --- COMPLETE
path editor
padaryti kad judetu keli enemy vienu metu

sukurti tower
tower saudymas
tower placinimas

*/

