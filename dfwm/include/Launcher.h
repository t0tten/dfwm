#ifndef _LAUNCHER_
#define _LAUNCHER_

#include <X11/Xlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <dirent.h>
#include <sstream>

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

		std::string path;
		
		LauncherState state;

		std::string searchPhrase;
		std::vector<std::string> files;
		std::vector<std::string> results;
		int autoCorrectIndex;
		
		void search();
		bool fetchFiles(std::string);
		void getPaths();

	public:
		Launcher(Display*, Window*);
		~Launcher();

		void show();
		void hide();

		void draw();
		void redraw();
	
		void addChar(std::string);
		void removeLastChar();

		Window getLauncherWindow();

		std::string getText();
		void autoComplete();

		LauncherState getState();
		void setState(LauncherState);
		
		int getWindowID();
};

#endif //_LAUNCHER_
