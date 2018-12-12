#include "Library.h"
using namespace std;

namespace lib {
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InputRecord;
	RECT r;
	DWORD Events;
	COORD cursor;
	HWND console = GetConsoleWindow();
	CONSOLE_CURSOR_INFO cci;
	CONSOLE_FONT_INFOEX cfi;
	HDC hdc = GetDC(console);
	int globalColor;
	int x = -100;
	int y = -100;
	clickableObject * object;

	void nothing() {}

	string getPath()
	{
		char result[MAX_PATH];
		return std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
	}

	void clearscreen(int color) {	//istrinamas ekranas ir nustatoma global color
		COORD coordScreen = { 0, 0 };
		DWORD cCharsWritten;
		DWORD dwConSize;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		globalColor = color;
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
		/*
		COLOR CODES
		0   BLACK
		1   BLUE
		2   GREEN
		3   CYAN
		4   RED
		5   MAGENTA
		6   BROWN
		7   LIGHTGRAY
		8   DARKGRAY
		9   LIGHTBLUE
		10  LIGHTGREEN
		11  LIGHTCYAN
		12  LIGHTRED
		13  LIGHTMAGENTA
		14  YELLOW
		15  WHITE
		*/
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

	void goFullscreen() {	//nustatomas fullscreen konsoles dydis
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
		if (GetKeyState(VK_RBUTTON) < 0)	//jei 2-mouse mygtukas laivas pastatomas
		{
			return true;
		}
		return false;
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
		SetPixel(hdc, x, y, color);	//nustato pixeli konsoleje
	}

	void printText(int x, int y, string text, int color) {	//isvedamas teksta i nurodyta pozicija, pasirinktos spalvos
		setColor(color);
		coord.X = x;
		coord.Y = y;
		setCursorPosition(x, y);
		cout << text;
	}

	void printBMP(string file, int x, int y) {

		//isveda bmp nuotrauka i konsole
		//https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file
		// read BMP file
		const char* filename = file.c_str();
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

		int tempy = y + height; // reikia tempy-- nes kitaip apversta isveda
		for (int i = 0; i < height; i++)
		{
			int tempx = x;
			fread(data, sizeof(unsigned char), row_padded, f);
			for (int j = 0; j < width * 3; j += 3)
			{
				// Convert (B, G, R) to (R, G, B)
				tmp = data[j];
				data[j] = data[j + 2];
				data[j + 2] = tmp;
				// Now data should contain the (R, G, B) values of the pixels.
				//The color of pixel (i, j) is stored at data[3 * (i * width + j)],
				//data[3 * (i * width + j) + 1] and data[3 * (i * width + j) + 2].
				printPixel(tempx, tempy, RGB((int)data[j], (int)data[j + 1], (int)data[j + 2]));
				tempx++;
			}
			tempy--;
		}
		fclose(f);
	}

	vector<string> fileTypeInFolder(const string& folder, string fileType)
	{
		//https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
		string path = folder;
		vector<string> names;
		ofstream fr("temp.txt");	//nuskaitomi visi failu pavadinimai direktorijoje
		for (const auto & p : experimental::filesystem::directory_iterator(path)) {
			fr << p << endl;
		}
		fr.close();
		ifstream fd("temp.txt");
		while (!fd.eof()) {//failu pavadinimai sudedami i vektoriu
			string temp;
			fd >> temp;
			names.push_back(temp);
		}
		fd.close();
		remove("temp.txt");


		for (int i = 0; i < names.size(); i++) {	//paliekami tik filetype nurodyti failai
			string temp;
			if (names[i].length() > fileType.length()) {
				temp = names[i].substr(names[i].length() - fileType.length(), fileType.length());
			}

			if (temp != fileType || names[i].length() < fileType.length()) {
				names.erase(names.begin() + i);
				i--;
			}
		}

		//for (int i = 0; i < names.size(); i++) { cout << names[i] << endl; }

		return names;
	}

	void cursorDraw(int backgroundColor, int color) {
		cursor = getMousePosition();
		setColor(backgroundColor);//istrina anksciau buvusi cursor
		for (int i = -1; i <= 1; i++) {
				setCursorPosition(x - 1, y + i);
				cout << "   ";
		}
		

		/*if (x > cursor.X) {	//kaire
			int j = x - cursor.X;
			for (int i = -2; i < 2; i++) {
				setCursorPosition(x + 1, y + i);
				cout << " ";
			}
		}
		else if (x < cursor.X) {	//desine
			for (int i = -2; i < 2; i++) {
				setCursorPosition(x - 2, y + i);
				cout << " ";
			}
		}

		else if (y > cursor.Y) {	//virsu
			for (int i = -2; i < 2; i++) {
				setCursorPosition(x + i, y + 1);
				cout << " ";
			}
		}

		else if (y < cursor.Y) {	//apacia
			for (int i = -2; i < 2; i++) {
				setCursorPosition(x + i, y - 2);
				cout << " ";
			}
		}*/

		x = cursor.X;//naujas cursor
		y = cursor.Y;
		setColor(color);
		for (int i = -1; i <= 1; i++) {
			setCursorPosition(x - 1, y + i);
			cout << "   ";
		}
		
		FlushConsoleInputBuffer(hin);
	}

	void errorMessage(string text) {
		printText(0, 0, text, 15 + 4 * 16);
	}
};

bool langas::setXY(int a, int b)
{
	if (a < 0 || b < 0) {
		lib::errorMessage("Failed to set XY");
		x = 0; y = 0;
		return false;
	} else{
		x = a; y = b;
		return true;
	}
}

bool langas::setWidth(int a)
{
	if (a < 0) {
		lib::errorMessage("Failed to set Width");
		width = 0;
		return false;
	}
	else {
		width = a;
		return true;
	}
}

bool langas::setHeight(int a)
{
	if (a < 0) {
		lib::errorMessage("Failed to set height");
		height = 0;
		return false;
	}
	else {
		height = a;
		return true;
	}
}

bool langas::setColor(int a)
{
	if (a < 0 || a > 255) {
		lib::errorMessage("Failed to set color");
		color = 0;
		return false;
	}
	else {
		color = a;
		return true;
	}
}

bool langas::setBorder(int a)

{
	if (a < 0 || a > 3) {
		lib::errorMessage("Failed to set border");
		borderType = 0;
		return false;
	}
	else {
		borderType = a;
		return true;
	}
}

void langas::fill(int color) {
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
}

void langas::create() {
	remove();
	int tempy = y;

	fill(color);

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

	else if (borderType == 3) {//dvigubas krastas
		v = char(219);
		b = char(219);
		vk = char(219);
		vd = char(219);
		ak = char(219);
		ad = char(219);
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

void langas::remove() {
	lib::setColor(lib::globalColor);
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

void langas::set(int sx, int sy, int swidth, int sheight, int scolor, int sborderType) {
	/*istrinama sena lentele ir sukuriama nauja lentele su naujais parametrais*/
	remove();
	setXY(sx,sy);
	setWidth(swidth);
	setHeight(sheight);
	setColor(scolor);
	setBorder(sborderType);
	create();
}




bool table::setRows(int a)
{
	if (rows < 0) {
		lib::errorMessage("Failed to set rows");
		rows = 0;
		return false;
	}
	else {
		rows = a;
		return true;
	}
}

bool table::setText(int index, string t)
{
	if (index < 0 || index > 10000) {
		lib::errorMessage("Failed to set text");
		return false;
	}
	else {
		text[index] = t;
		return true;
	}
}

void table::fill(int color) {
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
}

void table::create() {	//sukuriama lentele
	remove();
	int tempy = y;
	fill(color);

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

void table::remove() {
	lib::setColor(lib::globalColor);
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

void table::set(int sx, int sy, int swidth, int sheight, int scolor, int srows, int sborderType) {
	//istrinama sena lentele ir sukuriama nauja lentele su naujais parametrais
	remove();
	setXY(sx, sy);
	setWidth(swidth);
	setHeight(sheight);
	setColor(scolor);
	setRows(srows);
	setBorder(sborderType);
	create();
}

void menu::create() {
	remove();

	object = new clickableObject[rows];	//nustatomos meniu lenteliu pasirinkimai, kad juos butu galima paspasti su mouse ir ivyktu funkcija
	for (int i = 0; i < rows; i++) {
		object[i].topLeftX = x;
		object[i].topLeftY = y + height * i;
		object[i].bottomRightX = x + width;
		object[i].bottomRightY = y + height * (i + 1);
	}


	int tempy = y;
	fill(color);

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
		else if (borderType == 3) {//dvigubas krastas
			v = char(219);
			b = char(219);
			vk = char(219);
			vd = char(219);
			ak = char(219);
			ad = char(219);
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

void menu::check() {//tikrinama ar buvo paspausta ar pele uzvesta ant kazkurio is meniu lango
	COORD temp = lib::getMousePosition();	//kai hover pasikeicia spalva
	for (int r = 0; r < rows; r++) {	//tikrinama ant kurio lango uzvesta
		if (temp.X > object[r].topLeftX && temp.Y > object[r].topLeftY && object[r].bottomRightX > temp.X && object[r].bottomRightY > temp.Y) {
			int invertedColor = 255 - color;
			lib::setColor(invertedColor);

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
			break;
		}
	}
}

void menu::checkNoHover() {//tikrinama ar buvo paspausta ar pele uzvesta ant kazkurio is meniu lango
	if (lib::mouseLeftClick()) {
		COORD temp = lib::getMousePosition();	//kai hover pasikeicia spalva
		for (int r = 0; r < rows; r++) {	//tikrinama ant kurio lango uzvesta
			if (temp.X > object[r].topLeftX && temp.Y > object[r].topLeftY && object[r].bottomRightX > temp.X && object[r].bottomRightY > temp.Y) {
				lib::setColor(color);
				for (int i = 1; i < height; i++) {
					lib::setCursorPosition(object[lastRow].topLeftX + 1, object[lastRow].topLeftY + i);
					for (int j = 1; j < width; j++) {
						cout << " ";
					}
				}
				lib::setCursorPosition(object[lastRow].topLeftX + width / 2 - text[lastRow].length() / 2, object[lastRow].topLeftY + height / 2);
				cout << text[lastRow];
				lastRow = r;

				object[r].funkcija(); //jei buvo paspausta ivykdyti funkcija
				int invertedColor = 255 - color;
				lib::setColor(invertedColor);

				for (int i = 1; i < height; i++) {
					lib::setCursorPosition(object[r].topLeftX + 1, object[r].topLeftY + i);
					for (int j = 1; j < width; j++) {
						cout << " ";
					}
				}
				//kitos spalvos tekstas
				lib::setCursorPosition(object[r].topLeftX + width / 2 - text[r].length() / 2, object[r].topLeftY + height / 2);
				cout << text[r];
				break;
			}
		}
	}
}

void menu::set(int sx, int sy, int swidth, int sheight, int scolor, int srows, int sborderType) {
	//istrinama sena lentele ir sukuriama nauja lentele su naujais parametrais
	remove();
	setXY(sx, sy);
	setWidth(swidth);
	setHeight(sheight);
	setColor(scolor);
	setRows(srows);
	setBorder(sborderType);
	create();
}

void menu::setFunction(int index, function<void()> f)
{
	object[index].funkcija = f;
}

bool variableText::setXY(int a, int b) { 
	if (a < 0 || b < 0) {
		lib::errorMessage("Failed to set XY");
		coord.X = 0; coord.Y = 0;
		return false;
	}
	else {
		coord.X = a; coord.Y = b;
		return true;
	}
}
void variableText::setText(string msg) { message = msg; }
bool variableText::setColor(short a) {
	if (a < 0 || a > 255) {
		lib::errorMessage("Failed to set color");
		color = 0;
		return false;
	}
	else {
		color = a;
		return true;
	}
}
void variableText::setVariable(double dat) { data = dat; }
void variableText::showVariable(bool a) { show = a; }
void variableText::setSide(bool pos) { position = pos; }
void variableText::create() { //if position == true isveda pirmiau teksta
	remove();
	lib::setCursorPosition(coord.X, coord.Y);
	lib::setColor(color);
	if (show == false) {
		cout << message;
	}
	else {
		if (position) cout << message << data;
		else cout << data << message;
	}
}
void variableText::remove() {
	lib::setCursorPosition(coord.X, coord.Y);
	lib::setColor(color);
	for (int i = 0; i < message.length() + to_string(data).length(); i++) {
		cout << " ";
	}
}
//set leidzia viska iskarto padaryti
void variableText::set(int x1, int y1, string msg, short col, double dat, bool pos, bool sh) {
	remove();
	setXY(x1, y1);
	setText(msg);
	setColor(col);
	setVariable(dat);
	setSide(pos);
	showVariable(sh);
	create();
}

void textField::setText(string txt)
{
	text = txt;
}

void textField::create()
{
	remove();
	int tempy = y;

	fill(color);

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

	//tekstas suskaidomas i dalis ir sutalpinamas i teksto lauka
	int k = 0;
	for (int i = 0; i < height - 1; i ++)
	{
		lib::setCursorPosition(x + 1, y + i + 1);
		for (int j = 0; j < width - 1; j++)
		{
			//if (text[i] == ' ') cout << " ";
			if (text.length() < k) break;
			cout << text[k];
			if (j == width - 3) {	//la- labas
									//bas
				if (k + 2 < text.length()) {
					if (text[k + 1] != ' ' && text[k + 2] != ' ') {
						cout << "-";
						j++;
					}
				}
			}
			k++;
		}
	}
}

void textField::set(int sx, int sy, int swidth, int sheight, int scolor, int sborderType, string txt)
{
	/*istrinama sena lentele ir sukuriama nauja lentele su naujais parametrais*/
	remove();
	setXY(sx, sy);
	setWidth(swidth);
	setHeight(sheight);
	setColor(scolor);
	setBorder(sborderType);
	setText(txt);
	create();
}
