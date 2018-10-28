#include "../include/StatusBarItemDesktop.h"

StatusBarItemDesktop::StatusBarItemDesktop(int x, int color): StatusBarItem (x + 8, color) {
	this->bgX = x;
}

StatusBarItemDesktop::~StatusBarItemDesktop () {
}

void StatusBarItemDesktop::draw (Display* disp, Window* wnd, GC* gc, int* selected){
	setText(std::to_string(*selected));

	XSetForeground(disp, *gc, COL_STATUS_BAR_DESKTOP_BG);
	XFillArc(disp, *wnd, *gc, bgX, 2, 20, 20, CIRCLE[0], CIRCLE[1]);

	StatusBarItem::draw(disp, wnd, gc);

}

int StatusBarItemDesktop::getWidth () { return this->getHeight() + bgX; }
