#include "pch.h"
#include "Library.h"
#include <thread>
#include <iostream>
#include <conio.h>
#include <chrono>

const int fieldColor = 6 * 16;		//tako per kuri eina priesai spalva
const int backgroundColor = 15 * 16;	
int laukas[50][33];//map koordinaciu masyvas
int spawnX = 2, spawnY = 2;	//enemy spawn koordinates
int baseX = 2, baseY = 2;	//base koordinates	
int baseColor = 5 * 16;	//bazes spalva
int spawnColor = 0; //spawn'o spalva
int basePlayerHealth = 10;		//pradinis health
int playerHealth = basePlayerHealth; // dabartinis health
int basePlayerMoney = 100;//pradinis money
int playerMoney = basePlayerMoney; //dabartinis money (aukso kiekis)
chrono::steady_clock::time_point laikas;//dabartinis laikas

using namespace std;

void startLevelEditor();

void printLaukas() {//isprintinamas zadimo laukas, takas, baze, spawn

	for (int i = 0; i < 33; i++) {
		for (int j = 0; j < 50; j++) {
			if (laukas[j][i] == 1) {//tikrina ar tai yra path
				lib::setColor(fieldColor);
				for (int g = -1; g <= 1; g++) {
					lib::setCursorPosition(j - 1, i + g);
					cout << "   ";//tas blokas yra uzspalvinamas
					if (laukas[j - 1][i + g] == 0) laukas[j - 1][i + g] = 4; //uzdedamas flag laukas masyve, panaudojamas tikrinime ar galima deti tower, nes blokai 3x3, o interactive langeliai 1x1
					if (laukas[j][i + g] == 0) laukas[j][i + g] = 4;
					if (laukas[j + 1][i + g] == 0) laukas[j + 1][i + g] = 4;
				}
			}
			else if (laukas[j][i] == 2) {	// tikrinama ar tai yra zaidejo base
				lib::setColor(baseColor);
				for (int g = -1; g <= 1; g++) {
					lib::setCursorPosition(j - 1, i + g);
					cout << "   ";
					if (laukas[j - 1][i + g] == 0) laukas[j - 1][i + g] = 4;
					if (laukas[j][i + g] == 0) laukas[j][i + g] = 4;
					if (laukas[j + 1][i + g] == 0) laukas[j + 1][i + g] = 4;
				}
			}
			else if (laukas[j][i] == 3) {  // tikrinama ar tai yra prieso spawn
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

class enemy {	//paprastas priesas
private:
	int x, y;//enemy koordinates
	bool virsu = false;	//bool reikalingi enemy judejimui
	bool apacia = false;
	bool kaire = false;
	bool desine = false;
	bool vertikalu = false;
	bool horizontalu = false;
	int baseHealth = 10;//enemy pradines givybes
	int health = baseHealth;	//enemy dabartines gyvybes
	int color = 4;	//print spalva
	int money = 1;//pinigai duodami uz enemy sunaikinima
	int damage = 1;	// damage kuri daro zaidejo bazei
public:
	virtual void setXY(int a, int b) {	//nustato x ir y koordinates
		x = a; y = b;
	}
	virtual void del() {	//nutrinamas enemy
		lib::setColor(fieldColor);
		for (int i = -1; i <= 1; i++) { //nutrina 3x3 bloka
			lib::setCursorPosition(x - 1, y + i);
			for (int j = -1; j <= 1; j++) {
				cout << " ";
			}
		}
	}
	virtual void create() {	//pirmineje stadijoje nezinome i kuria puse eiti todel tai reiia nustatyti
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
	virtual void print() {	//isvedamas priesas
		char ch = char(178);
		lib::setColor(color);
		lib::setCursorPosition(x, y - 1);
		cout << ch;
		lib::setCursorPosition(x - 1, y);
		cout << ch << ch << ch;
		lib::setCursorPosition(x, y + 1);
		cout << ch;
	}
	virtual void move(int a, int b) {	//nutrinamas trail ir isprintinamas
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
	virtual void checkVertical() {	//tikrana ar eiti i virsu ar apacia
		desine = false;//praeiti kintamieji yra resetinami
		kaire = false;
		horizontalu = false;
		vertikalu = true;
		if (laukas[x][y - 1] == 1 || laukas[x][y - 1] == 2) {//tikrina ar eiti i virsu
			virsu = true;
			move(x, y - 1);
		}
		else if (laukas[x][y + 1] == 1 || laukas[x][y + 1] == 2) {//tikrina ar eiti i apacia
			apacia = true;
			move(x, y + 1);
		}
	}
	virtual void checkHorizontal() {	//tikrina ar eiti i kaire ar i desine
		apacia = false;//praeiti kintamieji nuresetinami
		virsu = false;
		horizontalu = true;
		vertikalu = false;
		if (laukas[x + 1][y] == 1 || laukas[x + 1][y] == 2) {//tikrina ar eiti i desnie
			desine = true;
			move(x + 1, y);
		}
		else if (laukas[x - 1][y] == 1 || laukas[x - 1][y] == 2) {//tikrina ar eiti i kaire
			kaire = true;
			move(x - 1, y);
		}
	}
	virtual void followPath() {	//path sekimo algroitmas
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
			if ((laukas[x][y - 1] == 1 || laukas[x][y - 1] == 2) && virsu) {	//virsu
				move(x, y - 1);
			}
			else if ((laukas[x][y + 1] == 1 || laukas[x][y + 1] == 2) && apacia) {	//zemyn
				move(x, y + 1);
			}
			else if (laukas[x][y - 1] == 4 || laukas[x][y + 1] == 4) {
				checkHorizontal();	//jeigu nei i virsu, nei i apacia negali eiti einama horizontaliai
			}
		}
		//jei bus nenurodyta pabaiga, vaikscios pirmyn atgal, nes checkHorizontal padarys vertical = false ir kitu ciklu checkVertical ras praeita reiksme ir eis atgal
	}
	virtual COORD position() {	//siunciama dabartine enemy pozicija
		COORD pos;
		pos.X = x; pos.Y = y;
		return pos;
	}
	virtual int damageTaken(int damage) {	//jie tower pataike nuimamos enemy givybes ir pakeiciama spalva jei atitinka salyga
		health -= damage;
		changeColor();
		print();
		return health;
	}
	virtual void changeColor() { //pakeicia prieso spalva kai hp maziau puses
		if (health < baseHealth / 2) color = 12;
	}
	virtual int getGold() { // pareturnina kiek aukso zaidejas gauna kai toweriai nuzudo priesa
		return money;
	}
	virtual int getDamageDealt() { //pareturnina kiek damage gauna zaidejo baze kai priesas ja pasiekia
		return damage;
	}
};
class boss : public enemy {	// boss tipo priesas
private:
	int x, y;
	bool virsu = false;	//reikalingi prieso judejimui nustatyti
	bool apacia = false;
	bool kaire = false;
	bool desine = false;
	bool vertikalu = false;
	bool horizontalu = false;
	int baseHealth = 100;
	int health = baseHealth;
	int color = 5;
	int money = 100;
	int damage = 25;
public:
	virtual void setXY(int a, int b) { //nustatyti x ir y koordinates
		x = a; y = b;
	}
	virtual void del() {	//nutrinamas enemy
		lib::setColor(fieldColor);
		for (int i = -1; i <= 1; i++) { //nutrina 3x3 bloka
			lib::setCursorPosition(x - 1, y + i);
			for (int j = -1; j <= 1; j++) {
				cout << " ";
			}
		}
	}
	virtual void create() {	//pirmineje stadijoje nezinome i kuria puse eiti todel tai reiia nustatyti
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
	virtual void print() {	//isvedamas priesas
		char ch = char(178);
		lib::setColor(color);
		lib::setCursorPosition(x-1, y - 1);
		cout << ch;
		lib::setCursorPosition(x + 1, y - 1);
		cout << ch;
		lib::setCursorPosition(x, y);
		cout << ch;
		lib::setCursorPosition(x - 1, y + 1);
		cout << ch;
		lib::setCursorPosition(x + 1, y + 1);
		cout << ch;
	}
	virtual void move(int a, int b) {	//nutrinamas trail ir isprintinamas
		lib::setColor(fieldColor);
		if (a > x) {//desine
			del();
		}
		else if (a < x) {//kaire
			del();
		}
		else if (b > y) {//zemyn
			del();
		}
		else if (b < y) {//aukstyn
			del();
		}
		setXY(a, b);
		print();
	}
	virtual void checkVertical() {	//tikrana ar eiti i virsu ar apacia
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
	virtual void checkHorizontal() {	//tikrina ar eiti i kaire ar i desine
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
	virtual void followPath() {	//path sekimo algroitmas
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
			if ((laukas[x][y - 1] == 1 || laukas[x][y - 1] == 2) && virsu) {	//virsu
				move(x, y - 1);
			}
			else if ((laukas[x][y + 1] == 1 || laukas[x][y + 1] == 2) && apacia) {	//zemyn
				move(x, y + 1);
			}
			else if (laukas[x][y - 1] == 4 || laukas[x][y + 1] == 4) {
				checkHorizontal();	//jeigu nei i virsu, nei i apacia negali eiti einama horizontaliai
			}
		}
		//jei bus nenurodyta pabaiga, vaikscios pirmyn atgal, nes checkHorizontal padarys vertical = false ir kitu ciklu checkVertical ras praeita reiksme ir eis atgal
	}
	virtual COORD position() {	//siunciama dabartine enemy pozicija
		COORD pos;
		pos.X = x; pos.Y = y;
		return pos;
	}
	virtual int damageTaken(int damage) {	//jie tower pataike nuimamos enemy givybes ir pakeiciama spalva jei atitinka salyga
		health -= damage;
		changeColor();
		print();
		return health;
	}
	virtual void changeColor() { //pakeicia prieso spalva kai hp maziau puses
		if (health < baseHealth / 2) color = 12;
	}
	virtual int getGold() { // pareturnina kiek aukso zaidejas gauna kai toweriai nuzudo priesa
		return money;
	}
	virtual int getDamageDealt() { //pareturnina kiek damage gauna zaidejo baze kai priesas ja pasiekia
		return damage;
	}
};
class tower {
private:
	int type = 1;//towerio tipa, reikalingas game saugojimui, 1 - long range, 2 - close range tower
	int x, y;	//tower'io koordinates
	int range = 7;	// per kiek blokeliu saudys tower'is
	bool shoot = true;	//nusako ar siuo metu gali  tower saudyti
	int rechargeTime = 300;	// kas kiek laiko issauna
	chrono::steady_clock::time_point lastShot;	//nustatomas kada buvo issauta paskutini karta
	int damage = 2;	//daroma zala enemy
	int towerCost = 10;	//pirkimo kaina
	int borderColor = 0;//spalvos
	int centerColor = 1 * 16;	// vidurinio langelio spalva
public:
	virtual void setXY(int a, int b) {	// nustato x ir y koordinates
		x = a; y = b;
	}
	virtual void print() {	//isprintinamas tower ir nustatoma lauko reiksme i 4 tam kad poto butu galima tikrinti ar galima toje vietoje statyti kita tower
		lib::setColor(borderColor);
		for (int i = -1; i <= 1; i++) {
			lib::setCursorPosition(x - 1, y + i);
			cout << "   ";
			laukas[x - 1][y + i] = 4;
			laukas[x][y + i] = 4;
			laukas[x + 1][y + i] = 4;
		}
		lib::setColor(centerColor);
		lib::setCursorPosition(x, y);
		cout << " ";
	}
	virtual int returnRange() {//grazinamas tower range
		return range;
	}
	virtual COORD returnPosition() {//grazinama tower pozicija
		COORD pos;
		pos.X = x; pos.Y = y;
		return pos;
	}
	virtual bool returnShoot() {//grazinama ar tower gali sauti, jei pirma issove turi laukti recharge time
		return shoot;
	}
	virtual void rechargeShoot() {	//tikrinama ar vel isnaujo gali tower sauti
		auto intervalas = std::chrono::duration_cast<std::chrono::milliseconds>(laikas - lastShot);//lyginamas dabartinis laikas su laiku kada buvo paskutini karta sauta
		if (intervalas.count() >= rechargeTime) shoot = true;//jei praejo pakankamai laiko galima sauti
	}
	virtual void changeShoot() { //nustatomas paskutinis suvio laikas ir shoot pakeiciama i false kad laukti kito suvio
		lastShot = chrono::steady_clock::now();	//nustato kada paskutini karta issove tower'is
		shoot = false;	//nustato false, kad nesaudytu
	}
	virtual int returnDamage() {//grazinama kiek damage daro tower
		return damage;
	}
	virtual void placeTower() { // padeda toweri su 1 (!) mygtuku
		if (playerMoney >= towerCost) {
			COORD pos = lib::getMousePosition();
			if (pos.X != 0 && pos.Y != 0) {//tikrina ar pos nelygu 0 nes yra glitch kad jeigu nejudini mouse ir prasai pozicijos grazina 0,0
				setXY(pos.X, pos.Y);
				print();
				playerMoney -= towerCost;	// nuima pinigus uz tower'o pastatyma
			}
		}
	}
	virtual void del() {		//istrinamas tower ir lauke nusetinama 0
		lib::setColor(backgroundColor);
		for (int i = -1; i <= 1; i++) {
			lib::setCursorPosition(x - 1, y + i);
			cout << "   ";
			laukas[x - 1][y + i] = 0;
			laukas[x][y + i] = 0;
			laukas[x + 1][y + i] = 0;
		}
	}
	virtual int returnType() {	// grazina tower'io tipa
		return type;
	}
};
class closeRange : public tower {	// close range tower'is
private:
	int type = 2;// type 2 - closerange tower
	int x, y;	// tower'io pozicija
	int range = 5; //per kiek blokeliu saudo
	bool shoot = true;	// ar gali sauti
	int rechargeTime = 200;	// kas kiek laiko sauna
	chrono::steady_clock::time_point lastShot;	// kada paskutini karta buvo issauta
	int damage = 3;	// kiek damage (zalos) daro tower
	int towerCost = 20;	// kiek kainuoja tower
	int borderColor = 1 * 16; // aplink vidurini blokeli esanciu blokeliu spalva
	int centerColor = 0; // vidurinio blokvelio spalva
public:
	virtual void setXY(int a, int b) {	//nustato x ir y koordinates
		x = a; y = b;
	}
	virtual void print() {	//isprintinamas tower ir nustatoma lauko reiksme i 4 tam kad poto butu galima tikrinti ar galima toje vietoje statyti kita tower
		lib::setColor(borderColor);
		for (int i = -1; i <= 1; i++) {
			lib::setCursorPosition(x - 1, y + i);
			cout << "   ";
			laukas[x - 1][y + i] = 4;
			laukas[x][y + i] = 4;
			laukas[x + 1][y + i] = 4;
		}
		lib::setColor(centerColor);
		lib::setCursorPosition(x, y);
		cout << " ";
	}
	virtual int returnRange() {//grazinamas tower range
		return range;
	}
	virtual COORD returnPosition() {//grazinama tower pozicija
		COORD pos;
		pos.X = x; pos.Y = y;
		return pos;
	}
	virtual bool returnShoot() {//grazinama ar tower gali sauti, jei pirma issove turi laukti recharge time
		return shoot;
	}
	virtual void rechargeShoot() {	//tikrinama ar vel isnaujo gali tower sauti
		auto intervalas = std::chrono::duration_cast<std::chrono::milliseconds>(laikas - lastShot);
		if (intervalas.count() >= rechargeTime) shoot = true;
	}
	virtual void changeShoot() { //tikrinama ar gali toweris sauti (saudo kas intervala)
		lastShot = chrono::steady_clock::now();
		shoot = false;
	}
	virtual int returnDamage() {//grazinama kiek damage daro tower
		return damage;
	}
	virtual void placeTower() { // padeda toweri su 1 (!) mygtuku
		if (playerMoney >= towerCost) {
			COORD pos = lib::getMousePosition();
			if (pos.X != 0 && pos.Y != 0) {//tikrina ar pos nelygu 0 nes yra glitch kad jeigu nejudini mouse ir prasai pozicijos grazina 0,0
				setXY(pos.X, pos.Y);
				print();
				playerMoney -= towerCost;
			}
		}
	}
	virtual void del() {//istrinamas tower
		lib::setColor(backgroundColor);
		for (int i = -1; i <= 1; i++) {
			lib::setCursorPosition(x - 1, y + i);
			cout << "   ";
			laukas[x - 1][y + i] = 0;
			laukas[x][y + i] = 0;
			laukas[x + 1][y + i] = 0;
		}
	}
	virtual int returnType() {
		return type;//returninamas koks towerio type
	}
};
class rounds {
private:
	int numberOfEnemies = 1;
	int roundNumber = 1;
	int spawnRate = 35;
	int sleepTime = 40;
	int roundFreezeTime = 5;
public:
	void increaseNumberOfEnemies() {
		numberOfEnemies *= 2;//roundo pabaigoje padvigubinamas enemy skaicius
	}
	void incRounds() {	//padidina round'o skaiciu
		roundNumber++;
	}
	void incSpawnRate() {
		if (spawnRate > 5) spawnRate -= 4;//padidinamas spawn rate, kas kiek tick bus sukurtas naujas enemy,
		else spawnRate = 1;
	}
	void incSleepTime() {	// mazinamas sleep time, kad greiciau judetu (is tikruju toweriai leciau saudo - kind of, nes sleep leidzia daugiau laiko praeiti per timeri, kuriame skaiciuojama kada issauna)
		if (sleepTime > 4) sleepTime -= 3;//
		else sleepTime = 1;
	}
	int returnNumberOfEnemies() {
		return numberOfEnemies;//grazina kiek per si raunda bus priesu
	}
	int returnRounds() {
		return roundNumber;//returnina kelintas dabar roundas
	}
	int returnSpawnRate() {
		return spawnRate;//dabartinis spawn rate
	}
	int returnSleepTime() {
		return sleepTime;//dabartinis sleep time
	}
	int returnFreezeTime() {
		return roundFreezeTime;//dabartinis freeze time
	}
	void increase() {// roundo pabaigoje visi parametrai padidinami
		increaseNumberOfEnemies();
		incRounds();
		incSpawnRate();
		incSleepTime();
	}
	void reset() {	// is naujo nustato priesu skaiciu, round'o numeri, spawn rate, sleep time ir round freeze time
		numberOfEnemies = 1;
		roundNumber = 1;
		spawnRate = 35;
		sleepTime = 40;
		roundFreezeTime = 5;
	}
};
class mainGame {
private:
	vector<enemy*> enemyVector;//vektorius kuriame yra visi priesai
	vector<tower*> towerVector;//vektorius kuriame yra visi tower
	rounds round;//round paramentu gavimas ir keitimas
	variableText money;	// isveda kiek zaidejas turi pinigu
	variableText health; // isveda kiek zaidejas turi gyvybiu
	variableText enemies; // isveda kiek zaidejo tower'iai sunaikino priesu
	variableText currentRound;	// isveda kelintas yra dabar round
	int	enemiesCreated = 0;//kiek per si rounda buvo sukurta priesu
	int totalEnemiesDefeated = 0;//parodo kiek per sia zaidimo sesija buvo sunaikinta priesu
	bool playGame = true;//bool nusakantis ar dabar vyskta zaidimas
	int saveColor = 15 + 2 * 16;//paspaudus save
	bool saveMode;//ar dabar vyskta save
	bool loadMode;	//ar dabar vyksta load game
	int messageColor = 3 + 15 * 16;
public:
	void enemyHit() {
		for (int i = 0; i < towerVector.size(); i++) {// tikrinami visi tower
			if (towerVector[i]->returnShoot()) {
				COORD towerPos = towerVector[i]->returnPosition();	//paimama tower pozicija ir range
				int range = towerVector[i]->returnRange();	// nustatomas tower'io saudymo nuotolis
				for (int j = 0; j < enemyVector.size(); j++) {	//tikrinami ar koksnors enemy yra tower range
					COORD enemyPos = enemyVector[j]->position();
					if ((enemyPos.X <= towerPos.X + range && enemyPos.X >= towerPos.X - range) && (enemyPos.Y <= towerPos.Y + range && enemyPos.Y >= towerPos.Y - range)) {
						//jei yra tower lauke nuimamos gyvybes arba panaikinamas
						if (enemyVector[j]->damageTaken(towerVector[i]->returnDamage()) <= 0) { //tikrinama ar prieso gyvybes 0 ar maziau
							playerMoney += enemyVector[j]->getGold();  //prideda zaidejui golda uz sunaikinta priesa
							deleteEnemy(j);	//istrina priesa
							totalEnemiesDefeated++; //padidina tower'iu sunaikintu priesu skaiciu
						}
						towerVector[i]->changeShoot();//nustatoma kad kazkuri laika negaletu tower sauti
						break;
					}
				}
			}
			else towerVector[i]->rechargeShoot();//tikrinama ar jau gali tower sauti, jei gali shoot == true
		}
	}
	void printBase() { //isprintinama baze
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
			COORD pos = enemyVector[i]->position();
			if (pos.X == baseX && pos.Y == baseY) {
				playerHealth -= enemyVector[i]->getDamageDealt();
				deleteEnemy(i);
				printBase();
			}
		}
		if (playerHealth <= 0) {
			gameEnd();
		}
	}
	void gameEnd() {// jei pasibaige zaidimas arba buvo paspausta ESC, isvedamas sitas meniu ir grazinama i level editor
		textField gg;
		gg.set(15, 15, 23, 4, baseColor, 2, "GAME OVER, PRESS ESC TO CONTINUE...");
		while (playGame) {
			if (_kbhit()) {
				int ch = getch();
				if (ch == 27) { //jei escape
					playGame = false;
					enemiesCreated = round.returnNumberOfEnemies() + 1;
					enemyVector.clear();
					towerVector.clear();
				}
			}
		}
	}
	void printField(int x, int y) {//kai enemy mirsta ikvieciama sita funkcija, enemy istrinamas nuo consoles lango
		lib::setCursorPosition(x, y);
		lib::setColor(fieldColor);
		for (int i = -1; i <= 1; i++) {
			lib::setCursorPosition(x - 1, y + i);
			for (int j = -1; j <= 1; j++) {
				cout << " ";
			}
		}
	}
	void deleteEnemy(int i) { //istrina priesa – konsolej, vektoriuje
		if (enemyVector.size() > 0) {	// tikrinama ar priesu vektoriuje yra daugiau nei 0
			COORD pos = enemyVector[i]->position();
			enemyVector[i]->del();
			enemyVector.erase(enemyVector.begin() + i);
			printField(pos.X, pos.Y);
		}
	}
	void buttonPressed() {// tikrina koks buvo paspaustas mygtukas
		char ch = getch();
		if (ch == '1') {	//jei paspaustas 1, tai nuperkamas tower
			if (checkPlacement()) {//tikrina ar toje vietoje gali buti padetas tower
				tower * a = new tower;
				COORD pos = lib::getMousePosition();
				if (pos.X != 0 && pos.Y != 0) {
					towerVector.push_back(a);// i vektoriu ikeliamas naujas tower, nustatoma jo pozicija ir nupiesiamas bei sumokama uz ji pinigus
					towerVector[towerVector.size() - 1]->setXY(pos.X,pos.Y);
					towerVector[towerVector.size() - 1]->placeTower();
				}
			}
		}
		else if (ch == '0') {	//jei paspausta 0 sunaikinamas tower
			destroyTower();
		}
		else if (ch == '2') {	//jei paspaustas 3, tai nuperkamas close range tower
			if (checkPlacement()) {//tikrina ar toje vietoje gali buti padetas tower
				closeRange * a = new closeRange;
				COORD pos = lib::getMousePosition();
				if (pos.X != 0 && pos.Y != 0) {
					towerVector.push_back(a);
					towerVector[towerVector.size() - 1]->setXY(pos.X, pos.Y);
					towerVector[towerVector.size() - 1]->placeTower();
				}
			}
		}
		else if (ch == '8') {	//jei paspaustas 8, tai iskvieciamas langas pasirinkti kuri zaidima paloadint'i
			loadGame();
		}
		else if (ch == '9') {	//jei paspaustas 9, tai iskvieciamas langas, kad issaugoti zaidima
			saveToFile();
		}
		if (ch == 27) { //jei escape
			gameEnd();//zaidimas baigiasi
		}
	}
	void destroyTower() {	// istrina tower'i
		COORD mousePos = lib::getMousePosition();
		for (int i = 0; i < towerVector.size(); i++) {
			COORD towerPos = towerVector[i]->returnPosition();
			if (mousePos.X == towerPos.X && mousePos.Y == towerPos.Y) { // tikrinama ant kurio tower yra zaidejo mouse
				towerVector[i]->del();
				towerVector.erase(towerVector.begin() + i);
			}
		}
	}
	bool checkPlacement() { //tikrinama ar galima pa'place'int tower'i
		COORD pos = lib::getMousePosition();
		if (pos.X == 0 && pos.Y == 0) return false;
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {//tikrina ar toje pozicijoje nera jokio path ar kito tower, tikrina ar ten visi 0
				if (laukas[pos.X + j][pos.Y + i] != 0) return false;
			}
		}
		return true;
	}
	void freezeTime() {	// funkcija iskvieciama padaryti pauzei pries round pradzia
		variableText freezeTime;
		freezeTime.setXY(5, 38);
		laikas = std::chrono::steady_clock::now();	//nustatomas dabartinis laikas
		auto freezeLaikas = std::chrono::steady_clock::now();	//nustatomas laikas kada prasidejo freeze time
		auto intervalas = std::chrono::duration_cast<std::chrono::seconds>(laikas - freezeLaikas);
		int t = 0;
		while ( (intervalas.count() <= round.returnFreezeTime()) && playGame) {

			intervalas = std::chrono::duration_cast<std::chrono::seconds>(laikas - freezeLaikas);
			laikas = std::chrono::steady_clock::now();	//nustatomas dabartinis laikas

			if (intervalas.count() > t) {//kiekviena sekunde isvedamas naujas message parodantis kiek liko freeze time laiko
				freezeTime.set(0, 38, "Time Until Next Round: ", 1 + 15 * 16, round.returnFreezeTime() - intervalas.count(), true, true);
				t++;
			}

			if (_kbhit()) {//per freeze time leidziama pirki bei atlikti kitas operacijas
				buttonPressed();	//tikrinam ar buvo paspaustas koksnors mygtukas
				printStatus();
			}
		}
		freezeTime.remove();
	}
	void printStatus() {	// isveda konsolje zaidejo pinigus, gyvybes, sunaikintu priesu skaiciu, dabartinio round skaiciu
		money.setVariable(playerMoney);
		money.create();
		health.setVariable(playerHealth);
		health.create();
		enemies.setVariable(totalEnemiesDefeated);
		enemies.create();
		currentRound.setVariable(round.returnRounds());
		currentRound.create();
	}
	void reset() {//visi parametrai grazinami i pradine stadija kad zaidima butu vel galima zaisti
		enemiesCreated = 0;
		totalEnemiesDefeated = 0;
		playGame = true;
		playerHealth = basePlayerHealth;
		playerMoney = basePlayerMoney;
		enemyVector.clear();
		round.reset();
	}
	void printTowers() {//isprintinami visi esantys tower
		for (int i = 0; i < towerVector.size(); i++) {
			towerVector[i]->print();
		}
	}
	void start() { //pagrindine zaidimo funkcija
		lib::setFontSize(16, 16);
		lib::setConsoleResolution(1350, 800);
		lib::clearscreen(backgroundColor);
		lib::remove_scrollbar();
		lib::setCursorVisibility(false);
		//pagalbinis tekstas kaireje
		lib::printText(52, 1, "press 1 - tower | price - 10", backgroundColor);
		lib::printText(52, 2, "press 2 - close range T.|price-25", backgroundColor);
		lib::printText(52, 3, "press 0 - to delete tower", backgroundColor);
		lib::printText(52, 4, "press 8 - to load game", backgroundColor);
		lib::printText(52, 5, "press 9 - to save game", backgroundColor);
		lib::printText(52, 6, "press ESC - to end game", backgroundColor);
		//dabartinis zaidejo status
		money.setXY(53, 10);
		money.set(53, 10, "Money: ", 1 + 15 * 16, playerMoney, true, true);
		health.setXY(53, 11);
		health.set(53, 11, "Health: ", 1 + 15 * 16, playerHealth, true, true);
		enemies.setXY(53, 12);
		enemies.set(53, 12, "Enemies Defeated: ", 1 + 15 * 16, totalEnemiesDefeated, true, true);
		currentRound.setXY(53, 13);
		currentRound.set(53, 13, "Round: ", 1 + 15 * 16, round.returnRounds(), true, true);
		//---------

		drawCurrentSession(); //perpiesia visa konsole

		while (playGame) {
			freezeTime(); // pauze pries round pradzia
			enemiesCreated = 0;
			int k = 0;	//reikalingas tikrinti spawn rate - kas kiek langeliu iseis naujas priesas
			createNewEnemy(); //sukuria nauja priesa
			while ( (enemiesCreated < round.returnNumberOfEnemies() || enemyVector.size() > 0) && playGame) {//zaidimas vyskta kol visi enemies buvo sukurti ir sunaikinti arba buvo paspaustas esc
				printStatus();//isvedami dabartiniai zaidejo duomenys, pinigai ir tt...
				printSpawn(); //nupiesia spawn

				laikas = std::chrono::steady_clock::now();	//nustatomas dabartinis laikas
				for (int i = 0; i < enemyVector.size(); i++) {
					enemyVector[i]->followPath();//kiekvienam enemy liepama paeiti viena pozicija
					baseReached(i);//tikrinama ar pasieke baze
				}
				enemyHit();	//tikrina ar tower pataike i enemy
				Sleep(round.returnSleepTime());//nusako kiek greitai judes preisai
				if (k == round.returnSpawnRate() && enemiesCreated < round.returnNumberOfEnemies()) {	//kas 10 paejimu sukuriamas naujas priesas
					createNewEnemy();
					k = 0;
				}
				k++; //padidinimas, nes jau egzistuojantys enemy pajudejo vienu langeliu
				if (_kbhit()) buttonPressed();	//tikrinam ar buvo paspaustas koksnors mygtukas
			}
			round.increase();//kiekvieno roundo pabaigoje pasunkinamas zaidimas
		}
		reset();//pasibaigus zaidimui viskas grazinama i pradine busena
	}
	void createNewEnemy() { //sukuria nauja priesa – sukuriamas naujas priesas vektoriuje ir nupaisomas konsoleje
		if ( (enemiesCreated + 1) % 50 == 0) {
			enemy * a = new boss;
			enemyVector.push_back(a);
			enemyVector[enemyVector.size() - 1]->setXY(spawnX, spawnY);
			enemyVector[enemyVector.size() - 1]->create();
			enemiesCreated++;
		}
		else {
			enemy * a = new enemy;
			enemyVector.push_back(a);
			enemyVector[enemyVector.size() - 1]->setXY(spawnX, spawnY);
			enemyVector[enemyVector.size() - 1]->create();
			enemiesCreated++;
		}
		
	}
	void drawBorder() {
		langas lang;	//game langas remeliai
		lang.set(0, 0, 51, 34, backgroundColor, 3);
	}
	void saveToFile() {//save langas
		saveMode = true;//save pradeda veikti
		string filename;//failo pavadinimas
		langas save;
		save.set(14, 15, 30, 8, saveColor, 1);//nupaisomas naujas langas kur parasytas tekstas ir ivedamas failo pavadinimas
		lib::printText(15, 16, "Enter file name: ", saveColor);	//message
		lib::printText(15, 17, "ESC - exit ", saveColor);	//message
		lib::printText(15, 18, "ENTER - save game", saveColor);	//message

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
					saveGameToFile(filename);
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
	void exitSaveMode() {//iseinama is save lango
		saveMode = false;
		lib::printText(20, 20, "Exiting... ", messageColor);	//message
		Sleep(20);
		drawCurrentSession();
	}
	void drawCurrentSession() {	//nupaiso tai kas jau yra editoriuje, skirtas tam kad panaikinti nebereikalingus issokusius meniu
		drawBorder();
		printLaukas();
		printSpawn();
		printStatus();
		printTowers();
	}
	void saveGameToFile(string file) {//game issaugomas nurodytame faile
		saveMode = false;
		if (file.length() > 0) {
			lib::printText(20, 20, "Saving... ", messageColor);	//message
			Sleep(20);
			ofstream fr(file + ".map");
			for (int i = 0; i < 33; i++) {
				for (int j = 0; j < 50; j++) {
					if (laukas[j][i] == 4) fr << "0 ";//jei tai yra 4 printinti 0 , 4 reikalingas tik zaidimo metu
					else fr << laukas[j][i] << " ";//viskas kas buvo lauke yra issaugoma i faila
				}
				fr << endl;
			}
			fr.close();

			ofstream frr(file + ".game");
			frr << playerHealth << endl;//issaugomi zaidejo parametrai
			frr << playerMoney << endl;
			frr << round.returnRounds() << endl;
			frr << towerVector.size() << endl;	//nurodoma kiek bus tower
			for (int i = 0; i < towerVector.size(); i++) {
				COORD pos = towerVector[i]->returnPosition();// issaugo tower pozicija ir tipa
				frr << pos.X << " " << pos.Y << " " << towerVector[i]->returnType() << endl;
			}
			frr.close();
		}

	}
	void loadGame() {
		auto path = std::experimental::filesystem::current_path();//nustatoma dabartine direktorija .game failu radimui
		vector<string> files = lib::fileTypeInFolder(path.string(), "game");	//randami visi game failai
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
		mapFiles.setFunction(0, bind(&mainGame::exitLoadMode, this));

		//priskiriama print funkcija:
		for (int i = 0; i < files.size(); i++) {
			mapFiles.setFunction(i + 1, bind(&mainGame::fromFileToMap, this, files[i]));	//nustatoma kad paspaudus ant meniu butu uzkraunamas failas
		}

		loadMode = true;
		while (loadMode) {
			mapFiles.check(); //tikrinama ar hoverina/uzvestas mouse
		}
		drawCurrentSession(); //perpiesia visa konsole
	}
	void exitLoadMode() {//iseinama is load mode
		loadMode = false;
		lib::printText(20, 20, "Exiting... ", messageColor);	//message
		Sleep(20);
		drawCurrentSession();
		printTowers();
	}
	void fromFileToMap(string filename) {
		loadMode = false;
		lib::printText(20, 20, "Loading... ", messageColor);	//message
		Sleep(20);
		string file = filename.substr(0, filename.length() - 5);
		ifstream df(file + ".map");//nuskaitomas su .game failu susijes .map failas ir uzkraunamas laukas masyve
		for (int i = 0; i < 33; i++) {
			for (int j = 0; j < 50; j++) {
				df >> laukas[j][i];
				if (laukas[j][i] == 2) {//nustatomos bazes ir spawn koordinates
					baseX = j;
					baseY = i;
				}
				else if (laukas[j][i] == 3) {
					spawnX = j;
					spawnY = i;
				}
			}
		}

		ifstream dff(filename);
		drawCurrentSession();
		reset();
		dff >> playerHealth;//nuskaitomi parametrai
		dff >> playerMoney;
		int rounds;
		dff >> rounds;
		for (int i = 0; i < rounds; i++) {
			round.increase();//nustaotmas raundas ir jo parametrai
		}
		int towers;
		dff >> towers;
		for (int i = 0; i < towers; i++) {
			COORD pos;
			int type;
			dff >> pos.X >> pos.Y >> type;
			if (type == 1) {
				tower * a = new tower;
				if (pos.X != 0 && pos.Y != 0) { //isvedami tower pagal ju tipus ir idedami i tower vektoriu
					towerVector.push_back(a);
					towerVector[towerVector.size() - 1]->setXY(pos.X,pos.Y);
					towerVector[towerVector.size() - 1]->print();
				}
			}
			else if (type == 2) {
				tower * a = new closeRange;
				if (pos.X != 0 && pos.Y != 0) {
					towerVector.push_back(a);
					towerVector[towerVector.size() - 1]->setXY(pos.X, pos.Y);
					towerVector[towerVector.size() - 1]->print();
				}
			}
		}
		
		start();
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
			lang.set(56, 30, 1, 1, fieldColor, 0);
		}
	}
	void drawCurrentSession() {	//nupaiso tai kas jau yra editoriuje, skirtas tam kad panaikinti nebereikalingus issokusius meniu
		lib::setColor(fieldColor);
		//reikia nutrinti kas pries tai buvo ant ekrano
		for (int i = 1; i < 33; i++) {
			lib::setCursorPosition(1, i + 1);
			for (int j = 1; j < 50; j++) {
				if (laukas[j][i] == 1) {
					print(j, i, fieldColor);//path block
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
	void startGame() {//iseinama is level editor
		editMode = false;
		lib::printText(20, 20, "Loading... ", messageColor);	//koks yra pasirinktas blokas pasako
		mainGame game;
		game.start();
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
	void fromFileToMap(string filename) {//nuskaitomi failo duomenys ir ikeliami i laukas masyva
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
	void clearAll() { //isvalo kintamaji laukas ir perpiesia visa konsole
		for (int i = 0; i < 33; i++) {
			for (int j = 0; j < 50; j++) {
				laukas[j][i] = 0;
			}
		}
		drawCurrentSession();
	}
	void start() { //pagrindine funkcija
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
		edit.setText(6, "Clear all");
		edit.set(50, 0, 14, 2, backgroundColor, 7, 3);
		edit.setFunction(0, bind(&levelEditor::selectedBlock, this, 3));	//paspaudus nusistato norimas blokas
		edit.setFunction(1, bind(&levelEditor::selectedBlock, this, 2));
		edit.setFunction(2, bind(&levelEditor::selectedBlock, this, 1));
		edit.setFunction(3, bind(&levelEditor::saveToFile, this));//atidaroma save funkcija
		edit.setFunction(4, bind(&levelEditor::loadMap, this));	//parodomi kokie map yra papkeje
		edit.setFunction(5, bind(&levelEditor::startGame, this));	//iseinama is editoriaus
		edit.setFunction(6, bind(&levelEditor::clearAll, this));	//clear viska

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
								print(pos.X, pos.Y, fieldColor);
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
int main()
{
	ios_base::sync_with_stdio(false);//pagreitina isvedima
	cin.tie(NULL);
	while (1) {
		startLevelEditor();//pradedamas zaidimas nuo level editor
	}

	getchar();

	return 0;
}

void startLevelEditor() {
	levelEditor editor;
	editor.start();
}
