#ifndef _DESKTOP_
#define _DESKTOP_

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <iostream>
#include <string>

#include "./Constants.h"

class Desktop {
	private:
		Display* disp;
		
		int x, y;
		int width, height;

		Window* left;
		Window* right;
		Window* root;
		Window currFocus;

		int wgap;

		int amountLeft;
		int amountRight;
		int size;	

		int findWindow(Window, Window*, int);

		void expandArray(Window*&, int amount);
		void resizeWindows();
		void addWindow(Window, Window*&, int&);

	public:
		Desktop(Display*, Window*, int, int, int, int);
		~Desktop();

		void show();
		void hide();

		void redraw();

		void moveToLeft(Window);
		void moveToRight(Window);
		
		void moveFirstWndToLeft();
		void moveLastWndToRight();
		void moveCurrWndToLeft();
		void moveCurrWndToRight();
		void moveCurrWndUpByOne();
		void moveCurrWndDownByOne();
		void swapFirstWindows();

		void addWindow(Window);
		void killCurrentWindow();
		bool removeWindow(Window);
		void openProgram(std::string);
		void setCurrentFocusedWindow(Window);
		bool gotWindows();
		Window popCurrentWindow();
                
                Window getCurrentFocusedWindow();
};

#endif //_DESKTOP_
