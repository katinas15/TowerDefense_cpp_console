#pragma once
#ifndef LIB_H
#define LIB_H
#include <iostream>
#include <windows.h>
#include <string>
#include <functional>
#include <vector>
#include <filesystem>
#include <fstream>
#pragma warning(disable:4996)
using namespace std;

namespace lib {


	void clearscreen(int);
	void setColor(int);
	void printText(int, int, string, int);
	void remove_scrollbar();
	COORD getMousePosition();
	void setConsoleResolution(int, int);
	void goFullscreen();
	bool mouseLeftClick();
	bool mouseRightClick();
	bool mouseEvent();
	void setCursorPosition(int, int);
	void setCursorVisibility(bool);
	void setFontSize(int, int);
	void printPixel(int, int, COLORREF);
	void printBMP(const char*, int, int);
	vector<string> fileTypeInFolder(const string&, string);
	void cursorDraw();




	
};
class langas {	//uzspalvinama pasirinkta vieta
public:
	int x;
	int y;
	int width;
	int height;
	int color;
	int borderType;

	void fill(int);
	void create();
	void remove();
	void set(int, int, int, int, int, int);
};

class table : public langas {
public:
	int rows;
	string text[10000];

	void fill(int color);
	void create();
	void remove();
	void set(int, int, int, int, int, int, int);
};

struct clickableObject {	//nurodoma kokioje pozicijoje galima paspasti objekta ir kokia funkcija jis atliks
	int topLeftX, topLeftY;
	int bottomRightX, bottomRightY;
	function<void()> funkcija;
};

class menu : public table {

public:
	clickableObject * object;

	void create();
	void check();
	void set(int, int, int, int, int, int, int);
};

class variableText {
private:
	short color;
	double data;
	string message;
	COORD coord;
	bool position;
public:
	void setCoord(int, int);
	void Message(string);
	void setColor(short);
	void setVariable(double);
	void setPosition(bool);
	void create();
	//set leidzia viska iskarto padaryti
	void set(int, int, string, short, double, bool);
};
#endif