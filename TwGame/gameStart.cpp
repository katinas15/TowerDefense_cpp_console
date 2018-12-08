#include "pch.h"
#include "Library.h"
#include <thread>
#include <iostream>
#include <conio.h>

const int fieldColor = 6 * 16;
const int backgroundColor = 15 * 16;
int laukas[50][33];
int spawnX = 2, spawnY = 2;
int baseX = 2, baseY = 2;
using namespace std;

void printLaukas() {
	lib::setColor(fieldColor);
	for (int i = 0; i < 33; i++) {
		for (int j = 0; j < 50; j++) {
			if (laukas[j][i] == 1) {
				lib::setCursorPosition(j, i);
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
	int saveColor = 15 + 2 * 16;
	int messageColor = 3 + 15 * 16;
	bool editMode;
	bool saveMode;

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
	void selectedBlock(int a) {//funkcija isveda koks yra siuo menu pasirinktas blokas
		block = a;
		langas lang;
		if (a == 3) {		
			text.set(53, 28, "Spawn point", backgroundColor, 0, true, false);
			lang.set(56, 30, 1, 1, spawnColor, 0);	//taip pat nurodoma blokelio spalva desineje apacioje
		}
		else if (a == 2) {
			text.set(53, 28, "Base", backgroundColor, 0, true, false);
			lang.set(56, 30, 1, 1, baseColor, 0);
		}
		else if (a == 1) {
			text.set(53, 28, "Path", backgroundColor, 0, true, false);
			lang.set(56, 30, 1, 1, pathColor, 0);
		}
	}
	void drawCurrentSession() {	//nupaiso tai kas jau yra editoriuje, skirtas tam kad panaikinti nebereikalingus issokusius meniu
		lib::setColor(fieldColor);
		//reikia nutrinti kas pries tai buvo ant ekrano
		for (int i = 1; i < 33; i++) {
			lib::setCursorPosition(1, i + 1);
			for (int j = 1; j < 50; j++) {
				if (laukas[j][i] == 1) {
					print(j, i, pathColor);
				}
				else if (laukas[j][i] == 2) {
					print(j, i, baseColor);
				}
				else if (laukas[j][i] == 3) {
					print(j, i, spawnColor);
				}
				else print(j, i, backgroundColor);
			}
		}
	}
	void exitEditor() {
		editMode = false;
		lib::printText(20, 20, "Loading... ", messageColor);	//koks yra pasirinktas blokas pasako
	}
	void drawBorder() {
		langas lang;	//level editor remeliai
		lang.set(0, 0, 50, 33, backgroundColor, 3);
	}
	void exitSaveMode() {
		saveMode = false;
		lib::printText(20, 20, "Exiting... ", messageColor);	//message
		Sleep(20);
		drawCurrentSession();
	}
	void saveMapToFile(string file) {
		saveMode = false;
		if (file.length() > 0) {
			lib::printText(20, 20, "Saving... ", messageColor);	//message
			Sleep(20);
			ofstream fr(file + ".map");
			for (int i = 0; i < 33; i++) {
				for (int j = 0; j < 50; j++) {
					fr << laukas[j][i];
				}
				fr << endl;
			}
		}
	}
	void saveToFile() {
		saveMode = true;//save pradeda veikti
		string filename;//failo pavadinimas
		langas save;
		save.set(14, 15, 30, 8, saveColor, 1);//nupaisomas naujas langas kur parasytas tekstas ir ivedamas failo pavadinimas
		lib::printText(15, 16, "Enter file name: ", saveColor);	//message
		lib::printText(15, 17, "ESC - exit ", saveColor);	//message
		lib::printText(15, 18, "ENTER - save ", saveColor);	//message

		langas textbox;
		textbox.set(17,22,24,0,backgroundColor,0);	//vieta kur bus rasomas tekstas

		lib::printText(0, 0, " ", 0); //glitch kaireje virsuj, pokolkas vienintelis budas kaip tai ispresti
		
		lib::setCursorPosition(17 + filename.length(), 22);
		while (saveMode) {
			lib::setCursorVisibility(true);//tada isjungti
			if (kbhit()) {	//tikrina kokia buvo paspausta raide
				lib::setColor(backgroundColor);
				int ch = getch();
				if (ch == '\b' && filename.length() > 0) {	//jei buvo paspausta backspace ta raide istrinama ir string ir lange
					filename.pop_back();
					lib::setCursorPosition(17 + filename.length(), 22);
					cout << " ";
				}
				else if (ch == 13) {//jei enter
					saveMapToFile(filename);
				}
				else if (ch == 27 && filename.length() > 0) { //jei escape{
					exitSaveMode();
				}
				else if (filename.length() < 25 && ch != 0) {
					lib::setCursorPosition(17 + filename.length(), 22);
					filename += char(ch); //paspaustas mygtukas idedamas i string
					cout << char(ch);	//ir isvedamas konsoleje
				}
			}
		}
		lib::setCursorVisibility(false);//tada isjungti
		drawCurrentSession();
	}
	void loadMap() {
		auto path = std::experimental::filesystem::current_path();//nustatoma dabartine direktorija .map failu radimui
		vector<string> files = lib::fileTypeInFolder(path.string(), "map");	//randami visi map failai
		menu mapFiles;	//sukuriamas meniu kad butu galima pasirinkti mapa
		mapFiles.setXY(15, 10);
		mapFiles.setWidth(30);
		mapFiles.setHeight(2);
		mapFiles.setColor(1 + 16 * 15);
		mapFiles.setRows(files.size() + 1);
		mapFiles.setBorder(1);
		mapFiles.setText(0, "(CLICK ON MAP BELOW TO LOAD)");
		for (int i = 0; i < files.size(); i++) {

			mapFiles.setText(i + 1, "..." + files[i].substr(files[i].length() - 20, 20)); //isvesti tik 20 char
		}

		mapFiles.create();
		mapFiles.setFunction(0, bind(&lib::nothing));
		//priskiriama print funkcija
		for (int i = 0; i < files.size(); i++) {
			mapFiles.setFunction(i + 1, bind(&lib::printBMP, files[i], 800, 200));
		}
	}
	void start() {
		lib::setFontSize(20, 20);
		lib::setConsoleResolution(1280, 720);
		lib::clearscreen(15 * 16);
		lib::remove_scrollbar();
		lib::setCursorVisibility(false);
		lib::printText(52, 26, "Selected: ", backgroundColor);	//koks yra pasirinktas blokas pasako
		textField rightclick;
		rightclick.set(50, 15, 15, 4, 1 + 15 * 16, 0, "Right-click to delete block!");
		drawBorder();

		menu edit;	//level editor pasirinkimu meniu
		edit.setText(0, "Spawn point");
		edit.setText(1, "Base");
		edit.setText(2, "Path");
		edit.setText(3, "Save map");
		edit.setText(4, "Load map");
		edit.setText(5, "Start game");
		edit.setText(6, "Main menu");
		edit.set(50, 0, 14, 2, backgroundColor,7, 3);
		edit.setFunction(0, bind(&levelEditor::selectedBlock, this, 3));	//paspaudus nusistato norimas blokas
		edit.setFunction(1, bind(&levelEditor::selectedBlock, this, 2));
		edit.setFunction(2, bind(&levelEditor::selectedBlock, this, 1));
		edit.setFunction(3, bind(&levelEditor::saveToFile, this));//atidaroma save funkcija
		edit.setFunction(4, bind(&levelEditor::loadMap, this));	//parodomi kokie map yra papkeje
		edit.setFunction(5, bind(&levelEditor::exitEditor, this));	//iseinama is editoriaus

		editMode = true;
		while (editMode) { //enemy baze
			while (lib::mouseEvent()) {	//jeigu nera jokio mouse event nera reikalo atlkineti veiksmu
				edit.check();	//tikrina meniu pasirinkimus, ar buvo paspausta ant meniu
				lib::printText(0, 0, " ", 0); //glitch kaireje virsuj, pokolkas vienintelis budas kaip tai ispresti
				if (lib::mouseLeftClick()) {
					COORD pos = lib::getMousePosition();
					if (check(pos.X, pos.Y)) {	//tikrinama ar neuzeina uz ribu
						if (block == 3) {
							print(spawnX, spawnY, backgroundColor);	//istrinamas praeitas spawn pointas, nes gali buti tik vienas spawn
							laukas[spawnX][spawnY] = 0;	//nutrinama reiksme faile
							laukas[pos.X][pos.Y] = 3;	//nustatoma 3 del failo saugojimo
							spawnX = pos.X;	//nustatomos naujos spawn koordinates
							spawnY = pos.Y;
							print(spawnX, spawnY, spawnColor);	//isvedamas
						}
						if (block == 1) {
							COORD pos = lib::getMousePosition();
							if (check(pos.X, pos.Y)) {
								laukas[pos.X][pos.Y] = 1;
								print(pos.X, pos.Y, pathColor);
							}
						}
						if (block == 2) {
							print(baseX, baseY, backgroundColor);	//gali buti tik viena baze
							laukas[baseX][baseY] = 0;
							laukas[pos.X][pos.Y] = 2;
							baseX = pos.X;
							baseY = pos.Y;
							print(baseX, baseY, baseColor);
						}
					}
				}
				if (lib::mouseRightClick()) {	//rightclick istrina bloka
					COORD pos = lib::getMousePosition();
					if (check(pos.X, pos.Y)) {
						laukas[pos.X][pos.Y] = 0;
						print(pos.X, pos.Y, backgroundColor);
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
	ios_base::sync_with_stdio(false);//pagreitina isvedima
	cin.tie(NULL);
	
	levelEditor f;
	f.start();
	
	lib::setFontSize(14, 14);
	lib::setConsoleResolution(1280, 720);
	lib::clearscreen(backgroundColor);
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
path editor  --- complete
padaryti kad leistu faila issaugoti su pasirinktu pavadinimu --COMPLETE
padaryti kad leistu uzloadint pasirinka faila is duotu pasirinkimu(tikrintu ar game papkeje kokia yra map failai)


padaryti kad judetu keli enemy vienu metu

sukurti tower
tower saudymas
tower placinimas

*/

