#ifndef _MENU_H_
#define _MENU_H_

#include <X11/Xlib.h>
#include "./Constants.h"

#include <iostream>
#include <string>
#include <math.h>

class Menu {
	private:
		Display* disp;
		Window menu;
		GC gc;

		int x, y;
		int diameter, dd;
		int color;

		int pWidth, pHeight;
		int nrOfDesktops;
		int* selected;

	public:
		Menu(Display*, Window*, int, int, int, int*, int);
		~Menu();

		void changeAlignment(int);
		void draw();
		void redraw();

		int getWindowID ();

		void show();
		void hide();
};

#endif //_MENU_H_
