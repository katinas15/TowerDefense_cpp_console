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
	void nothing();
	string getPath();
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
	void printBMP(string, int, int);
	vector<string> fileTypeInFolder(const string&, string);
	void cursorDraw(int,int);
	void errorMessage(string);
};
class langas {	//uzspalvinama pasirinkta vieta
protected:
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;
	int color = 0;
	int borderType = 0;
public:
	bool setXY(int, int);
	bool setWidth(int);
	bool setHeight(int);
	bool setColor(int);
	bool setBorder(int);
	void fill(int);
	void create();
	void remove();
	void set(int, int, int, int, int, int);
};

class textField : public langas {
private:
	string text;
public:
	void setText(string);
	void create();
	void set(int, int, int, int, int, int, string);
};

class table : public langas {
protected:
	int rows = 0;
	string text[10000];
public:
	
	bool setRows(int);
	bool setText(int, string);
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
private:
	clickableObject * object;
public:
	void create();
	void check();
	void setFunction(int, function<void()>);
	void set(int, int, int, int, int, int, int);
};

class variableText {
private:
	short color = 0;
	double data = 0;
	string message;
	COORD coord;
	bool position = true;
	bool show = true;
public:
	bool setXY(int, int);
	void setText(string);
	bool setColor(short);
	void setVariable(double);
	void setSide(bool);
	void create();
	void showVariable(bool);
	//set leidzia viska iskarto padaryti
	void set(int, int, string, short, double, bool);
};
#endif
