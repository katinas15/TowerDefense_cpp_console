#include "pch.h"
#include "Library.h"

int main()
{
	lib::clearscreen(15);	//istrina spalva ir nuspalvina ekrana i nurodyta spalva
	lib::remove_scrollbar(); //nuima is console scroll bar	
	lib::setFontSize(10, 20);	//fonto dydis
	//lib::goFullscreen();	//fullscreen
	//COORD a = lib::getMousePosition();	//get mouse position
	lib::setConsoleResolution(1280, 720); // set resolution
//	lib::setCursorPosition(a.X,a.Y);
	lib::setCursorVisibility(false);


	/*----------------------------------------------------------------------
	------------------------------------------------------------------------
	-----SKIRTINGI BUDAI KAIP GALIMA SUKURTI LANGA, TABLE AR MENIU----------*/

	langas a;	//paprastas langas,   x,y,width,height,color,bordertype
	a.setXY(22,13);
	a.setWidth(10);
	a.setHeight(10);
	a.setColor(15);
	a.setBorder(2);

	//lib::goFullscreen();	//fullscreen
	lib::printText(0, 30, "Jei neveikia pele: 2-mouse ant consoles->Properties->Options->'uncheck' Quick Edit Mode", 15);

	textField test; //teksto langas
	test.set(70, 15, 25, 5, 2+15*16, 1, "Kad galetumete naudotis meniu, reikia paspausti 1-peles klavisa ant pasirinkto meniu punkto");
	
	//bliac juk ngl compilint xd
	menu b;	//menu,   x,y,width,height,color,rows,text,bordertype, object
	b.setText(0, "MENU");
	b.setText(1, "langas create");
	b.setText(2,"langas remove");
	b.setText(3,"resolution 1280x720");
	b.setText(4,"resolution 800x600");
	b.setText(5,"print Text");

	b.set(0, 1, 20, 2, 15, 6, 2);	//arba su set

	b.setFunction(0, bind(&lib::nothing));//objecktui priskiriamos funkcijos
	b.setFunction(1, bind(&langas::create, a));
	b.setFunction(2, bind(&langas::remove, a));
	b.setFunction(3, bind(&lib::setConsoleResolution, 1280, 720));
	b.setFunction(4, bind(&lib::setConsoleResolution, 800, 600));
	b.setFunction(5, bind(lib::printText,30, 20, "cat", 2 + 16 * 4));

	//teisingas rasymo formatas C:/pictures ------------ pasviras bruksnelis "/" o ne "\"
	vector<string> files = lib::fileTypeInFolder("C:/Users/katinas/Downloads/tower_defense_game-master/tower_defense_game-master/pictures", "bmp");	//nustatyti kur yra visi pictures (ideta i project pictures papke)
	//table,   x,y,width,height,color,rows,text,bordertype
	menu bmpFiles;
	bmpFiles.setXY(60, 1);
	bmpFiles.setWidth(40);
	bmpFiles.setHeight(2);
	bmpFiles.setColor(1 + 16 * 15);
	bmpFiles.setRows(files.size() + 1);
	bmpFiles.setBorder(1);
	bmpFiles.setText(0 , "(CLICK TO PRINT)");
	for (int i = 0; i < files.size(); i++) {
		bmpFiles.setText(i+1, "..." + files[i].substr(files[i].length() - 20, 20)); //isvesti tik 20 char
	}

	bmpFiles.create();
	bmpFiles.setFunction(0, bind(&lib::nothing));
	//priskiriama print funkcija
	for (int i = 0; i < files.size(); i++) {
		bmpFiles.setFunction(i+1, bind(&lib::printBMP, files[i], 800, 200));
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
//Darba atliko Laimonas Janutėnas ir Gediminas Kailiūnas
