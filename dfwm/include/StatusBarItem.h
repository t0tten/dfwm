#ifndef _STATUS_BAR_ITEM_
#define _STATUS_BAR_ITEM_

#include <X11/Xlib.h>
#include <string>

class StatusBarItem {
	private:
		int x, y;
		int height;
		int color;
		std::string text;

	public:
		const int TEXT_HORIZONTAL_CENTER = 17;

		StatusBarItem(int x, int color);
		~StatusBarItem();

		void setText(std::string);
		std::string getText();

		void setX(int x);
		int getX();
		int getY();
		int getWidth();
		int getHeight();
		int getColor();

		void virtual draw(Display*, Window*, GC*);
};

#endif //_STATUS_BAR_ITEM_
