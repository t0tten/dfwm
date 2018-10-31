#ifndef _STATUS_BAR_CLOCK_
#define _STATUS_BAR_CLOCK_

#include "./StatusBarItem.h"

class StatusBarClock : public StatusBarItem {
	private:
		std::string getTime();
	public:
		StatusBarClock(int x, int color);
		~StatusBarClock();
};

#endif //_STATUS_BAR_CLOCK_
