// TwGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
//#include <iostream>
#include "Library.h"

int main()
{
	lib::clearscreen(15);	//istrina spalva ir nuspalvina ekrana i nurodyta spalva
	lib::remove_scrollbar(); //nuima is console scroll bar	
	lib::setFontSize(10, 20);	//fonto dydis
	//lib::goFullscreen();	//fullscreen
	//lib::printText(30, 20, "nibba", 2+16*4); //x,y,string,color
	//COORD a = lib::getMousePosition();	//get mouse position
	lib::setConsoleResolution(1280, 720); // set resolution
//	lib::setCursorPosition(a.X,a.Y);
	lib::setCursorVisibility(false);


	/*----------------------------------
	-------------------------------------
	-----SKIRTINGI BUDAI KAIP GALIMA SUKURTI LANGA, TABLE AR MENIU*/

	langas a;	//paprastas langas,   x,y,width,height,color,bordertype
	a.x = 22;
	a.y = 13;
	a.width = 10;
	a.height = 10;
	a.color = 15;
	a.borderType = 2;

	//lib::goFullscreen();	//fullscreen
	lib::printText(0, 30, "Jei neveikia pele: 2-mouse ant consoles->Properties->Options->'uncheck' Quick Edit Mode", 15);
	
	menu b;	//menu,   x,y,width,height,color,rows,text,bordertype, object
	b.text[0] = "MENU";
	b.text[1] = "langas create";
	b.text[2] = "langas remove";
	b.text[3] = "resolution 1280x720";
	b.text[4] = "resolution 800x600";
	b.text[5] = "print Text";

	b.set(0, 1, 20, 2, 15, 6, 2);	//arba su set
	b.object[0].funkcija = bind(&lib::nothing);	//objecktui priskiriamos funkcijos
	b.object[1].funkcija = bind(&langas::create, a);	
	b.object[2].funkcija = bind(&langas::remove, a);
	b.object[3].funkcija = bind(&lib::setConsoleResolution, 1280, 720);
	b.object[4].funkcija = bind(&lib::setConsoleResolution, 800, 600);
	b.object[5].funkcija = bind(&lib::printText, 40,20,"Hello",196);

	//teisingas rasymo formatas C:/pictures ------------ pasviras bruksnelis "/" o ne "\"
	vector<string> files = lib::fileTypeInFolder("I:/chrome/tower_defense_game-master/tower_defense_game-master/pictures", "bmp");	//nustatyti kur yra visi pictures (ideta i project pictures papke)
	//table,   x,y,width,height,color,rows,text,bordertype
	menu bmpFiles;
	bmpFiles.x = 60;
	bmpFiles.y = 1;
	bmpFiles.width = 40;
	bmpFiles.height = 2;
	bmpFiles.color = 1 + 16 * 15;
	bmpFiles.rows = files.size() + 1;
	bmpFiles.borderType = 1;
	bmpFiles.text[0] = "(CLICK TO PRINT)";
	for (int i = 0; i < files.size(); i++) {
		bmpFiles.text[i+1] = "..." + files[i].substr(files[i].length() - 20, 20); //isvesti tik 20 char
	}

	bmpFiles.create();

	//FUNKCIJAS APIBREZTI TIKTAI PO CREATE ARBA SET!!!!
	bmpFiles.object[0].funkcija = bind(&lib::nothing);
	//priskiriama print funkcija
	for (int i = 0; i < files.size(); i++) {
		bmpFiles.object[i + 1].funkcija = bind(&lib::printBMP, files[i], 800, 200);
	}

	while (1) {

		if (lib::mouseEvent()) {
			//	lib::cursorDraw();	//dar bandoma
			b.check();
			bmpFiles.check();
		}
	}
	return 0;
}
