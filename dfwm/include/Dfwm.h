#ifndef _DFWM_
#define _DFWM_

#include <X11/Xlib.h>

#include "./KeyBindings.h"
#include "./StatusBar.h"
#include "./Menu.h"
#include "./Desktop.h"
#include "./Launcher.h"

#include <iostream>
#include <string>
class KeyBindings;
class Dfwm {
	private:
		/* X11 */
		Display* disp;
		Window root;

		XEvent e;
		
		/* Dfwm */	
		KeyBindings* keys;
		StatusBar* bar;
		Menu* menu;
		Desktop** desktop;
		Launcher* launcher;
		
		int size;
		int nrOfMapped;
		Window* mapped;

		int screen;
		int sWidth, sHeight;
		int selected;
		int maxDesktops;
		KeyCode code;
		bool running;
		bool* desktopHaveWindow;
		
		void init();
		Window* findAllWindows(unsigned int&);
		bool windowIsNotDfwm(Window);
		void drawGraphics(Window);
		void addWindowToDesktop(Window);
		void translateClientMessage(XClientMessageEvent);
		void removeWindowFromDesktop(Window);
                void handleXEvent();
                void ungrabKeys();

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

		Menu* 		getMenu();
		StatusBar* 	getStatusBar();
		Desktop* 	getCurrentDesktop();
		Launcher* 	getLauncher();	

		void addMapped(Window);
		void removeMapped(Window);
		Window* getMappedList();
		int getNrOfMapped();
		void grabFocused(Window, int);
		void grabFocused(Window);
		void moveCurrentWindowToDesktop(int);

                Display* getDisplay();
                Window getRoot();

};

#endif //_DFWM_
