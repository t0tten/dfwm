#ifndef _STATUS_BAR_
#define _STATUS_BAR_

#include <X11/Xlib.h>
#include <string>

#include "./Constants.h"
#include "./StatusBarClock.h"
#include "./StatusBarItem.h"
#include "./StatusBarItemInfo.h"
#include "./StatusBarItemDesktop.h"

class StatusBar {
	private:
		Display* disp;
		Window bar;
		GC gc;

		StatusBarItemDesktop* dtop;
		StatusBarClock* clock;
		StatusBarItem* title;
		StatusBarItemInfo** infos;
		int nrOfInfos;

		int x, y;
		int width, height;

		int color;
		int* selected;

                void update();

	public:
		StatusBar(Display*, Window*, int*);
		~StatusBar ();

		void show();
		void draw();
		int getWindowID();
		void redraw();
		int getHeight();
		void setText(std::string);
};

#endif //_STATUS_BAR_
