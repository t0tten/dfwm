#ifndef _STATUS_BAR_ITEM_DESKTOP_
#define _STATUS_BAR_ITEM_DESKTOP_

#include <X11/Xlib.h> 
#include <string>
#include <iostream>

#include "./StatusBarItem.h"
#include "./Constants.h"

class StatusBarItemDesktop : public StatusBarItem {
	private:
		int bgX;
	public:
		StatusBarItemDesktop(int x, int color);
		~StatusBarItemDesktop();
		void draw(Display*, Window*, GC*, int*);
		int getWidth();
};

#endif //_STATUS_BAR_ITEM_DESKTOP_
