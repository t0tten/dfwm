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

		int wgap;

		int amountLeft;
		int amountRight;
		int size;	

		int border;
		int borderColor;
		
		int findWindow(Window, Window*, int);

		void expandArray(Window*&, int amount);
		void resizeWindows();
		void addWindow(Window, Window*&, int&);

		Window findAllWindows(Window*, int);
		
		Window* get_win_list_stacked(unsigned long*);
		int minimized_window(Window);
		int is_skip_taskbar_or_pager_set(Window);
		Window get_top_window_from_stack();

	public:
		Desktop(Display*, Window*, int, int, int, int, int, int, int);
		~Desktop();

		void show();
		void hide();

		void redraw();

		void moveToLeft(Window);
		void moveToRight(Window);

		void addWindow(Window);
		void removeWindow(Window);
		Window openProgram(std::string, Window*, int);
};

#endif //_DESKTOP_
