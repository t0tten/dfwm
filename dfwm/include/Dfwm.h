#ifndef _DFWM_
#define _DFWM_

#include <X11/Xlib.h>

#include "./KeyBindings.h"
#include "./StatusBar.h"
#include "./Menu.h"
#include "./Desktop.h"

#include <iostream>
#include <string>
class KeyBindings;
class Dfwm {
	private:
		/* X11 */
		Display* disp;
		Screen* screenis;
		Window root;

		XEvent e;
		
		/* Dfwm */	
		KeyBindings* keys;
		StatusBar* bar;
		Menu* menu;
		Desktop** desktop;
		
		int size;
		int nrOfOpen;
		Window* opened;

		int screen;
		int nrScreens;
		int sWidth, sHeight;
		int selected;
		int maxDesktops;
		KeyCode code;
		bool running;
		
		void init();
		Window* findAllWindows(unsigned int&);

	public:
		Dfwm();
		Dfwm(std::string);
		~Dfwm();
		void run();
		void quit();
		void setSelected(int);
		void incrementSelected();
		void decrementSelected();

		void redraw();

		Menu* getMenu();
		StatusBar* getStatusBar();
		Desktop* getCurrentDesktop();
		
		void addOpen(Window);
		void removeOpen(Window);
		Window* getOpened();
		int getNrOfOpen();

};

#endif //_DFWM_
