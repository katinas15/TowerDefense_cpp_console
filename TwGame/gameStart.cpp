#include "pch.h"
#include "Library.h"
#include <thread>
#include <iostream>
#include <conio.h>
#include <chrono>

const int fieldColor = 6 * 16;
const int backgroundColor = 15 * 16;
int laukas[50][33];
int spawnX = 2, spawnY = 2;
int baseX = 2, baseY = 2;
int pathColor = 6 * 16;
int baseColor = 5 * 16;
int spawnColor = 0;
int playerHealth = 1;
int playerMoney = 1000;
chrono::steady_clock::time_point laikas;

using namespace std;

void startLevelEditor();
void startGame();

void printLaukas() {

	for (int i = 0; i < 33; i++) {
		for (int j = 0; j < 50; j++) {
			if (laukas[j][i] == 1) {
				lib::setColor(fieldColor);
				for (int g = -1; g <= 1; g++) {
					lib::setCursorPosition(j - 1, i + g);
					cout << "   ";
					if (laukas[j - 1][i + g] == 0) laukas[j - 1][i + g] = 4;
					if (laukas[j][i + g]	 == 0) laukas[j][i + g] = 4;
					if (laukas[j + 1][i + g] == 0) laukas[j + 1][i + g] = 4;
				}
			}
			else if (laukas[j][i] == 2) {
				lib::setColor(baseColor);
				for (int g = -1; g <= 1; g++) {
					lib::setCursorPosition(j - 1, i + g);
					cout << "   ";
					if (laukas[j - 1][i + g] == 0) laukas[j - 1][i + g] = 4;
					if (laukas[j][i + g] == 0) laukas[j][i + g] = 4;
					if (laukas[j + 1][i + g] == 0) laukas[j + 1][i + g] = 4;
				}
			}
			else if (laukas[j][i] == 3) {
				lib::setColor(spawnColor);
				for (int g = -1; g <= 1; g++) {
					lib::setCursorPosition(j - 1, i + g);
					cout << "   ";
					if (laukas[j - 1][i + g] == 0) laukas[j - 1][i + g] = 4;
					if (laukas[j][i + g] == 0) laukas[j][i + g] = 4;
					if (laukas[j + 1][i + g] == 0) laukas[j + 1][i + g] = 4;
				}
			}
		}
		cout << endl;
	}
}

class enemy {
private:
	int x, y;
	bool virsu = false;
	bool apacia = false;
	bool kaire = false;
	bool desine = false;
	bool vertikalu = false;
	bool horizontalu = false;
	int baseHealth = 10;
	int health = baseHealth;
	int color = 4;
	int money = 1;
	int damage = 1;
public:
	friend class field;
	void setXY(int a, int b) {
		x = a; y = b;
	}
	void del() {	//nutrinamas enemy
		lib::setColor(fieldColor);
		for (int i = -1; i <= 1; i++) { //nutrina 3x3 bloka
			lib::setCursorPosition(x - 1, y + i);
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
	void print() {	//isvedamas priesas
		char ch = char(178);
		lib::setColor(color);
		lib::setCursorPosition(x, y - 1);
		cout << ch;
		lib::setCursorPosition(x - 1, y);
		cout << ch << ch << ch;
		lib::setCursorPosition(x, y + 1);
		cout << ch;
	}
	void move(int a, int b) {	//nutrinamas trail ir isprintinamas
		lib::setColor(fieldColor);
		if (a > x) {//desine
			lib::setCursorPosition(x, y - 1);
			cout << " ";
			lib::setCursorPosition(x - 1, y);
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
			lib::setCursorPosition(x - 1, y);
			cout << " ";
			lib::setCursorPosition(x, y - 1);
			cout << " ";
			lib::setCursorPosition(x + 1, y);
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
	void checkVertical() {	//tikrana ar eiti i virsu ar apacia
		desine = false;
		kaire = false;
		horizontalu = false;
		vertikalu = true;
		if (laukas[x][y - 1] == 1 || laukas[x][y - 1] == 2) {
			virsu = true;
			move(x, y - 1);
		}
		else if (laukas[x][y + 1] == 1 || laukas[x][y + 1] == 2) {
			apacia = true;
			move(x, y + 1);
		}
	}
	void checkHorizontal() {	//tikrina ar eiti i kaire ar i desine
		apacia = false;
		virsu = false;
		horizontalu = true;
		vertikalu = false;
		if (laukas[x + 1][y] == 1 || laukas[x + 1][y] == 2) {
			desine = true;
			move(x + 1, y);
		}
		else if (laukas[x - 1][y] == 1 || laukas[x - 1][y] == 2) {
			kaire = true;
			move(x - 1, y);
		}
	}
	void followPath() {	//path sekimo algroitmas
		if (horizontalu) {
			if ((laukas[x + 1][y] == 1 || laukas[x + 1][y] == 2) && desine) {	//desine
				move(x + 1, y);
			}
			else if ((laukas[x - 1][y] == 1 || laukas[x - 1][y] == 2) && kaire) {	//kaire
				move(x - 1, y);
			}
			else if (laukas[x + 1][y] == 4 || laukas[x - 1][y] == 4) {
				checkVertical();//jeigu nei i kaire, nei i desine negali paeiti einama vertikaliai
			}
		}
		else if (vertikalu) {
			if ((laukas[x][y - 1] == 1 || laukas[x][y - 1] == 2 ) && virsu) {	//virsu
				move(x, y - 1);
			}
			else if ((laukas[x][y + 1] == 1 || laukas[x][y + 1] == 2 ) && apacia) {	//zemyn
				move(x, y + 1);
			}
			else if (laukas[x][y - 1] == 4 || laukas[x][y + 1] == 4) {
				checkHorizontal();	//jeigu nei i virsu, nei i apacia negali eiti einama horizontaliai
			}
		}
		//jei bus nenurodyta pabaiga, vaikscios pirmyn atgal, nes checkHorizontal padarys vertical = false ir kitu ciklu checkVertical ras praeita reiksme ir eis atgal
	}
	COORD position() {	//siunciama dabartine enemy pozicija
		COORD pos;
		pos.X = x; pos.Y = y;
		return pos;
	}
	int damageTaken(int damage) {	//jie tower pataike nuimamos enemy givybes ir pakeiciama spalva jei atitinka salyga
		health -= damage;
		changeColor();
		print();
		return health;
	}
	void changeColor() { //pakeicia prieso spalva kai hp maziau puses
		if (health < baseHealth/2) color = 12;
	}
	int getGold() { // pareturnina kiek aukso zaidejas gauna kai toweriai nuzudo priesa
		return money;
	}
	int getDamageDealt(){ //pareturnina kiek damage gauna zaidejo baze kai priesas ja pasiekia
		return damage;
	}
};

class tower {
private:
	int x, y;
	int range = 7;
	bool shoot = true;
	int rechargeTime = 300;
	chrono::steady_clock::time_point lastShot;
	int damage = 2;
	int towerCost = 10;
public:
	void setXY(int a, int b) {
		x = a; y = b;
	}
	void print() {	//isprintinamas tower ir nustatoma lauko reiksme i 4 tam kad poto butu galima tikrinti ar galima toje vietoje statyti kita tower
		lib::setColor(0);
		for (int i = -1; i <= 1; i++) { 
			lib::setCursorPosition(x - 1, y + i);
			cout << "   ";
			laukas[x - 1][y + i] = 4;
			laukas[x][y + i]	 = 4;
			laukas[x + 1][y + i] = 4;
		}
		lib::setColor(1*16);
		lib::setCursorPosition(x, y);
		cout << " ";
	}
	int returnRange(){//grazinamas tower range
		return range;
	}
	COORD returnPosition(){//grazinama tower pozicija
		COORD pos;
		pos.X = x; pos.Y = y;
		return pos;
	}
	bool returnShoot() {//grazinama ar tower gali sauti, jei pirma issove turi laukti recharge time
		return shoot;
	}
	void rechargeShoot() {	//tikrinama ar vel isnaujo gali tower sauti
		auto intervalas = std::chrono::duration_cast<std::chrono::milliseconds>(laikas - lastShot);
		if(intervalas.count() >= rechargeTime) shoot = true;
	}
	void changeShoot() { //tikrinama ar gali toweris sauti (saudo kas intervala)
		lastShot = chrono::steady_clock::now();
		shoot = false;
	}
	int returnDamage() {//grazinama kiek damage daro tower
		return damage;
	}
	void placeTower() { // padeda toweri su 1 (!) mygtuku
		if (playerMoney >= towerCost) {
			COORD pos = lib::getMousePosition();
			if (pos.X != 0 && pos.Y != 0) {//tikrina ar pos nelygu 0 nes yra glitch kad jeigu nejudini mouse ir prasai pozicijos grazina 0,0
				setXY(pos.X, pos.Y);
				print();
				playerMoney -= towerCost;
			}
		} else {
			lib::printText(1, 36, "Gold stocks are too low sire", 4);	//jei truksta pinigu isvedamas message
		}
	}
	void del() {
		lib::setColor(backgroundColor);
		for (int i = -1; i <= 1; i++) {
			lib::setCursorPosition(x - 1, y + i);
			cout << "   ";
			laukas[x - 1][y + i] = 0;
			laukas[x][y + i] = 0;
			laukas[x + 1][y + i] = 0;
		}
	}
};

class rounds {
private:
	int numberOfEnemies = 5;
	int roundNumber = 1;
	int spawnRate = 35;
	int sleepTime = 40;
	int roundFreezeTime = 1;
public:
	void increaseNumberOfEnemies() {
		numberOfEnemies *= 2;
	}
	void incRounds() {
		roundNumber++;
	}
	void incSpawnRate() {
		if(spawnRate > 3) spawnRate -= 7;
		else spawnRate = 1;
	}
	void incSleepTime() {
		if (sleepTime > 1) sleepTime -= 9;
		else sleepTime = 1;
	}
	int returnNumberOfEnemies(){
		return numberOfEnemies;
	}
	int returnRounds() {
		return roundNumber;
	}
	int returnSpawnRate(){
		return spawnRate;
	}
	int returnSleepTime() {
		return sleepTime;
	}
	int returnFreezeTime() {
		return roundFreezeTime;
	}
	void increase() {
		increaseNumberOfEnemies();
		incRounds();
		incSpawnRate();
		incSleepTime();
	}
};
class levelEditor {
private:
	int block;
	variableText text;
	int saveColor = 15 + 2 * 16;
	int messageColor = 3 + 15 * 16;
	bool editMode;
	bool saveMode;
	bool loadMode;

public:
	void print(int x, int y, int color) {	//paiso teksta nurodytoje pozicijoje
		lib::setColor(color);
		lib::setCursorPosition(x, y);
		cout << " ";
	}
	bool check(int x, int y) {
		if (x < 1 || y < 1 || x > 49 || y > 32) return false;	//tikrina lauko ribas ir jei tinka isveda true/ tikrina ar buvo paspausta pasymo ribose
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
					print(j, i, pathColor);//path block
				}
				else if (laukas[j][i] == 2) {//base block
					print(j, i, baseColor);
				}
				else if (laukas[j][i] == 3) {//spawn block
					print(j, i, spawnColor);
				}
				else print(j, i, backgroundColor);
			}
		}
	}
	void exitEditor() {//iseinama is level editor
		editMode = false;
		lib::printText(20, 20, "Loading... ", messageColor);	//koks yra pasirinktas blokas pasako
		startGame();
	}
	void drawBorder() {
		langas lang;	//level editor remeliai
		lang.set(0, 0, 50, 33, backgroundColor, 3);
	}
	void exitSaveMode() {//iseinama is save lango
		saveMode = false;
		lib::printText(20, 20, "Exiting... ", messageColor);	//message
		Sleep(20);
		drawCurrentSession();
	}
	void saveMapToFile(string file) {//map issaugomas nurodytame faile
		saveMode = false;
		if (file.length() > 0) {
			lib::printText(20, 20, "Saving... ", messageColor);	//message
			Sleep(20);
			ofstream fr(file + ".map");
			for (int i = 0; i < 33; i++) {
				for (int j = 0; j < 50; j++) {
					fr << laukas[j][i] << " ";
				}
				fr << endl;
			}
		}
	}
	void saveToFile() {//save langas
		saveMode = true;//save pradeda veikti
		string filename;//failo pavadinimas
		langas save;
		save.set(14, 15, 30, 8, saveColor, 1);//nupaisomas naujas langas kur parasytas tekstas ir ivedamas failo pavadinimas
		lib::printText(15, 16, "Enter file name: ", saveColor);	//message
		lib::printText(15, 17, "ESC - exit ", saveColor);	//message
		lib::printText(15, 18, "ENTER - save ", saveColor);	//message

		langas textbox;
		textbox.set(17, 22, 24, 0, backgroundColor, 0);	//vieta kur bus rasomas tekstas

		lib::printText(0, 0, " ", 0); //glitch kaireje virsuj, pokolkas vienintelis budas kaip tai ispresti

		lib::setCursorPosition(17 + filename.length(), 22);
		while (saveMode) {
			lib::setCursorVisibility(true);//ijungiamas cursor kad matytusi kur rasoma
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
				else if (ch == 27 && filename.length() > 0) { //jei escape
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
	void exitLoadMode() {
		loadMode = false;
		lib::printText(20, 20, "Exiting... ", messageColor);	//message
		Sleep(20);
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
		mapFiles.setText(0, "EXIT");

		for (int i = 0; i < files.size(); i++) {
			mapFiles.setText(i + 1, files[i].substr(path.string().length() + 1, files[i].length() - path.string().length())); //is viso path string atimama nereikalinga dalis ir paliekamas map pavadinimas su .map liekana
		}
		mapFiles.create();
		mapFiles.setFunction(0, bind(&levelEditor::exitLoadMode, this));
		//priskiriama print funkcija
		for (int i = 0; i < files.size(); i++) {
			mapFiles.setFunction(i + 1, bind(&levelEditor::fromFileToMap, this, files[i]));	//nustatoma kad paspaudus ant meniu butu uzkraunamas failas
		}

		loadMode = true;
		while (loadMode) {
			mapFiles.check();
		}
		drawCurrentSession();
	}
	void fromFileToMap(string filename) {
		loadMode = false;
		lib::printText(20, 20, "Loading... ", messageColor);	//message
		Sleep(20);
		ifstream df(filename);
		for (int i = 0; i < 33; i++) {
			for (int j = 0; j < 50; j++) {
				df >> laukas[j][i];
				if (laukas[j][i] == 2) {
					baseX = j;
					baseY = i;
				}
				else if (laukas[j][i] == 3) {
					spawnX = j;
					spawnY = i;
				}
			}
		}
	}
	void clearAll() {
		for (int i = 0; i < 33; i++) {
			for (int j = 0; j < 50; j++) {
				laukas[j][i] = 0;
			}
		}
		drawCurrentSession();
	}
	void start() {
		lib::setFontSize(20, 20);
		lib::setConsoleResolution(1280, 720);
		lib::clearscreen(15 * 16);
		lib::remove_scrollbar();
		lib::setCursorVisibility(false);
		lib::printText(52, 26, "Selected: ", backgroundColor);	//koks yra pasirinktas blokas pasako
		textField rightclick;
		rightclick.set(50, 20, 15, 4, 1 + 15 * 16, 0, "Right-click to delete block!");
		drawBorder();

		menu edit;	//level editor pasirinkimu meniu
		edit.setText(0, "Spawn point");
		edit.setText(1, "Base");
		edit.setText(2, "Path");
		edit.setText(3, "Save map");
		edit.setText(4, "Load map");
		edit.setText(5, "Start game");
		edit.setText(6, "Main menu");
		edit.setText(7, "Clear all");
		edit.set(50, 0, 14, 2, backgroundColor, 8, 3);
		edit.setFunction(0, bind(&levelEditor::selectedBlock, this, 3));	//paspaudus nusistato norimas blokas
		edit.setFunction(1, bind(&levelEditor::selectedBlock, this, 2));
		edit.setFunction(2, bind(&levelEditor::selectedBlock, this, 1));
		edit.setFunction(3, bind(&levelEditor::saveToFile, this));//atidaroma save funkcija
		edit.setFunction(4, bind(&levelEditor::loadMap, this));	//parodomi kokie map yra papkeje
		edit.setFunction(5, bind(&levelEditor::exitEditor, this));	//iseinama is editoriaus
		edit.setFunction(6, bind(&levelEditor::exitEditor, this));	//iseinama is editoriaus
		edit.setFunction(7, bind(&levelEditor::clearAll, this));	//iseinama is editoriaus

		editMode = true;
		while (editMode) { //enemy baze
			while (lib::mouseEvent() && editMode) {	//jeigu nera jokio mouse event nera reikalo atlkineti veiksmu
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

class mainGame {
private:
	vector<enemy> enemyVector;
	vector<tower> towerVector;
	rounds round;
	variableText money;
	variableText health;
	variableText enemies;
	variableText currentRound;
	int	enemiesCreated = 0;
	int totalEnemiesDefeated = 0;
public:
	void enemyHit() {
		for (int i = 0; i < towerVector.size(); i++) {// tikrinami visi tower
			if (towerVector[i].returnShoot()) {
				COORD towerPos = towerVector[i].returnPosition();	//paimama tower pozicija ir range
				int range = towerVector[i].returnRange();	// nustatomas tower'io saudymo nuotolis
				for (int j = 0; j < enemyVector.size(); j++) {	//tikrinami ar koksnors enemy yra tower range
					COORD enemyPos = enemyVector[j].position();
					if ((enemyPos.X <= towerPos.X + range && enemyPos.X >= towerPos.X - range) && (enemyPos.Y <= towerPos.Y + range && enemyPos.Y >= towerPos.Y - range)) {
							//jei yra tower lauke nuimamos gyvybes arba panaikinamas
						if (enemyVector[j].damageTaken(towerVector[i].returnDamage()) <= 0) { //tikrinama ar prieso gyvybes 0 ar maziau
							playerMoney += enemyVector[j].getGold();  //prideda zaidejui golda uz sunaikinta priesa
							deleteEnemy(j);	//istrina priesa
							totalEnemiesDefeated++;
						}
						towerVector[i].changeShoot();//nustatoma kad kazkuri laika negaletu tower sauti
						break;
					}
				}
			} else towerVector[i].rechargeShoot();//tikrinama ar jau gali tower sauti, jei gali shoot == true
		}
	}
	void printBase(){ //isprintinama baze
		lib::setColor(baseColor);
		for (int g = -1; g <= 1; g++) {
			lib::setCursorPosition(baseX - 1, baseY + g);
			cout << "   ";
		}
	}
	void printSpawn() { //isprintina spawn
		lib::setColor(spawnColor);
		for (int g = -1; g <= 1; g++) {
			lib::setCursorPosition(spawnX - 1, spawnY + g);
			cout << "   ";
		}
	}
	void baseReached(int i) { // tikrina ar baze pasiekta, jei taip - sunaikina priesa, perpiesia baze, nuima zaidejo hp
		if (enemyVector.size() > 0) {
			COORD pos = enemyVector[i].position();
			if (pos.X == baseX && pos.Y == baseY) {
				playerHealth -= enemyVector[i].getDamageDealt();
				deleteEnemy(i);
				printBase();
			}
		}
		if (playerHealth <= 0) {
			gameEnd();
		}
	}
	void gameEnd() {
		textField gg;
		gg.set(15, 15, 23, 4, baseColor, 2, "GAME OVER, PRESS ESC TO CONTINUE...");
		while (1) {
			if (_kbhit()) {
				int ch = getch();
				if (ch == 27) { //jei escape
					startLevelEditor();
					break;
				}
			}
		}
	}
	void printField(int x,int y) {//kai enemy mirsta ikvieciama sita funkcija, enemy istrinamas nuo consoles lango
		lib::setCursorPosition(x, y);
		lib::setColor(pathColor);
		for (int i = -1; i <= 1; i++) {
			lib::setCursorPosition(x - 1, y + i);
			for (int j = -1; j <= 1; j++) {
				cout << " ";
			}
		}
	}
	void deleteEnemy(int i) { //istrina priesa – konsolej, vektoriuje
		if (enemyVector.size() > 0) {
			COORD pos = enemyVector[i].position();
			enemyVector[i].del();
			enemyVector.erase(enemyVector.begin() + i);
			printField(pos.X, pos.Y);
		}
	}
	void buttonPressed() {// tikrina koks buvo paspaustas mygtukas
		char ch = getch();
		if (ch == '1') {	//jei paspaustas 1, tai nuperkamas tower
			if (checkPlacement()) {//tikrina ar toje vietoje gali buti padetas tower
				tower a;
				towerVector.push_back(a);
				towerVector[towerVector.size() - 1].placeTower();
			}
		}
		else if (ch == '0') {	//jei paspaustas 1, tai nuperkamas tower
				destroyTower();
		}
	}
	void destroyTower() {
		COORD mousePos = lib::getMousePosition();
		for (int i = 0; i < towerVector.size(); i++) {
			COORD towerPos = towerVector[i].returnPosition();
			if (mousePos.X == towerPos.X && mousePos.Y == towerPos.Y) {
				towerVector[i].del();
				towerVector.erase(towerVector.begin() + i);
			}
		}
	}
	bool checkPlacement() { //tikrinama ar galima pa'place'int tower'i
		COORD pos = lib::getMousePosition();
		if (pos.X == 0 && pos.Y == 0) return false;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (laukas[pos.X + j][pos.Y + i] != 0) return false;
			}
		}
		return true;
	}
	void freezeTime() {
		variableText freezeTime;
		freezeTime.setXY(5, 38);
		laikas = std::chrono::steady_clock::now();	//nustatomas dabartinis laikas
		auto freezeLaikas = std::chrono::steady_clock::now();	//nustatomas dabartinis laikas
		auto intervalas = std::chrono::duration_cast<std::chrono::seconds>(laikas - freezeLaikas);
		int t = 0;
		while (intervalas.count() <= round.returnFreezeTime()) {
			
			intervalas = std::chrono::duration_cast<std::chrono::seconds>(laikas - freezeLaikas);
			laikas = std::chrono::steady_clock::now();	//nustatomas dabartinis laikas

			if (intervalas.count() > t) {
				freezeTime.set(0, 38, "Time Until Next Round: ", 1 + 15 * 16, round.returnFreezeTime() - intervalas.count(), true, true);
				t++;
			}

			if (_kbhit()) {
				buttonPressed();	//tikrinam ar buvo paspaustas koksnors mygtukas
				printStatus();
			}
		}
		freezeTime.remove();
	}
	void printStatus() {
		money.setVariable(playerMoney);
		money.create();
		health.setVariable(playerHealth);
		health.create();
		enemies.setVariable(totalEnemiesDefeated);
		enemies.create();
		currentRound.setVariable(round.returnRounds());
		currentRound.create();
	}
	void start() {
		lib::setFontSize(16, 16);
		lib::setConsoleResolution(1300, 800);
		lib::clearscreen(backgroundColor);
		lib::remove_scrollbar();
		lib::setCursorVisibility(false);

		menu gameMenu;	//zaidimo menu esantis meniu
		gameMenu.setText(0, "Place Tower");
		gameMenu.setText(1, "Save Game");
		gameMenu.setText(2, "Load Game");
		gameMenu.setText(3, "Main Menu");
		gameMenu.setText(4, "Pause Game");
		gameMenu.set(51, 0, 14, 4, backgroundColor, 5, 3);
		gameMenu.setFunction(0, bind(&lib::nothing));	
		gameMenu.setFunction(1, bind(&lib::nothing));
		gameMenu.setFunction(2, bind(&lib::nothing));
		gameMenu.setFunction(3, bind(&lib::nothing));
		gameMenu.setFunction(4, bind(&lib::nothing));

		drawBorder();
		printLaukas();
		printSpawn();
		
		money.setXY(53, 30);
		money.set(53, 30, "Money: ", 1 + 15 * 16, playerMoney, true, true);
		health.setXY(53, 33);
		health.set(53, 33, "Health: ", 1 + 15 * 16, playerHealth, true, true);
		enemies.setXY(53, 36);
		enemies.set(53, 36, "Enemies Defeated: ", 1 + 15 * 16, totalEnemiesDefeated, true, true);
		currentRound.setXY(53, 39);
		currentRound.set(53, 39, "Round: ", 1 + 15 * 16, round.returnRounds(), true, true);
		
	while (1) {
		freezeTime();
		enemiesCreated = 0;
		int k = 0;
		createNewEnemy();
		while (enemiesCreated < round.returnNumberOfEnemies() || enemyVector.size() > 0) {
			printStatus();
			printSpawn();

			laikas = std::chrono::steady_clock::now();	//nustatomas dabartinis laikas
			for (int i = 0; i < enemyVector.size(); i++) {
				enemyVector[i].followPath();//kiekvienam enemy liepama paeiti viena pozicija
				baseReached(i);//tikrinama ar pasieke baze
			}
			enemyHit();	//tikrina ar tower pataike i enemy
			Sleep(round.returnSleepTime());
			k++;
			if (k == round.returnSpawnRate() && enemiesCreated < round.returnNumberOfEnemies()) {	//kas 10 paejimu sukuriamas naujas priesas
				createNewEnemy();
				k = 0;
			}
			if (_kbhit()) buttonPressed();	//tikrinam ar buvo paspaustas koksnors mygtukas
		}
		round.increase();
	}
}
	void createNewEnemy() { //sukuria nauja priesa – sukuriamas naujas priesas vektoriuje ir nupaisomas konsoleje
		enemy a;
		enemyVector.push_back(a);
		enemyVector[enemyVector.size() - 1].setXY(spawnX, spawnY);
		enemyVector[enemyVector.size() - 1].create();
		enemiesCreated++;
	}
	void drawBorder() {
		langas lang;	//game langas remeliai
		lang.set(0, 0, 51, 34, backgroundColor, 3);
	}
};

levelEditor editor;
mainGame game;

int main()
{
	ios_base::sync_with_stdio(false);//pagreitina isvedima
	cin.tie(NULL);

	levelEditor t;
	t.start();
	startLevelEditor();

	getchar();

	return 0;
}

void startLevelEditor() {
	editor.start();
}

void startGame() {
	game.start();
}
/*
enemy judejimas  --- COMPLETE
sukurti kad enemy vaiksciotu pagal path --- COMPLETE
path editor  --- complete
padaryti kad leistu faila issaugoti su pasirinktu pavadinimu --COMPLETE
padaryti kad leistu uzloadint pasirinka faila is duotu pasirinkimu(tikrintu ar game papkeje kokia yra map failai) -- COMPLETE
padaryti kad judetu keli enemy vienu metu -- COMPLETE

sukurti tower	--COMPLETE
tower saudymas--CPMPLETE
tower placinimas--COMPLETE
kai nuzudomas priesas, pries mirti pakeisti spalva --COMPLETE
implementuoti priesams hp	--COMPLETE
padaryt kad negalima butu placint toweri ant towerio (stackint) ir ant tako -- COMPLETE

money -- COMPLETE
health	-- COMPLETE
delete tower -- COMPLEtE

rounds -- COMPLETE
game end -- COMPLETE

GAME OVER ERROR

save game
load game

upgrade
different enemies
more towers

bruksnys neaisku del ko	--COMPLETE
blokas prie money del text variable -- COMPLETE

padaryti kad load map meniu galetu uzkrauti daugiau failu, yra scrollbar kazkoks


*/

