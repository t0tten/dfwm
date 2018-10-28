#ifndef _STATUS_BAR_CLK_
#define _STATUS_BAR_CLK_

#include "./StatusBarItem.h"

class StatusBarClk : public StatusBarItem {
	private:
		std::string getTime();
	public:
		StatusBarClk(int x, int color);
		~StatusBarClk();
};

#endif //_STATUS_BAR_CLK_
