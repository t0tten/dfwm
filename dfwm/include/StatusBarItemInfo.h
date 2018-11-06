#ifndef _STATUS_BAR_ITEM_INFO_
#define _STATUS_BAR_ITEM_INFO_

#include "./StatusBarItem.h"
#include <iostream>

class StatusBarItemInfo : public StatusBarItem {
	private:
                XWindowAttributes xWindowAttributes; 

	public:
		StatusBarItemInfo(int color, XWindowAttributes winAttrs,
                                int x = 0);
		~StatusBarItemInfo();

		void setText(std::string text);

		void draw(Display*, Window*, GC*);
		void checkClick(int, int);
};

#endif //_STATUS_BAR_ITEM_INFO_
