#ifndef _STATUS_BAR_ITEM_INFO_
#define _STATUS_BAR_ITEM_INFO_

#include "./StatusBarItem.h"

class StatusBarItemInfo : public StatusBarItem {
	private:
	public:
		StatusBarItemInfo(int color, int x = 0);
		~StatusBarItemInfo();

		void setText(std::string text);

		void draw(Display*, Window*, GC*);
};

#endif //_STATUS_BAR_ITEM_INFO_
