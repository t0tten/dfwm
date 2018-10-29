#ifndef _LAUNCHER_
#define _LAUNCHER_

#include <X11/Xlib.h>
#include <string>
#include <iostream>

#include "./Constants.h"

class Launcher {
	private:
		Display* disp;
		GC gc;

		Window launcher;

		int x, y;
		int width, height;
		int color;
		int textColor;

		std::string* results;
		std::string path;
		
		LauncherState state;

		std::string searchPhrase;
		
		void search();

	public:
		Launcher(Display*, Window*, int, int);
		~Launcher();

		void show();
		void hide();

		void draw();
		void redraw();
	
		void addChar(char*);
		void removeLastChar();

		Window getLauncherWindow();

		std::string getText();

		LauncherState getState();
		void setState(LauncherState);
		
		int getWindowID();
};

#endif //_LAUNCHER_
