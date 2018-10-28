#include "../include/StatusBarItemInfo.h"

StatusBarItemInfo::StatusBarItemInfo(int color, int x) : StatusBarItem(x, color) {
}

StatusBarItemInfo::~StatusBarItemInfo() {}

void StatusBarItemInfo::setText(std::string text) {
	this->StatusBarItem::setText(text);
	this->setX((text.length() * 10) + getX());
}

void StatusBarItemInfo::draw(Display* disp, Window* wnd, GC* gc) {
	XWindowAttributes wndAttr;
	XGetWindowAttributes(disp, *wnd, &wndAttr);

	XSetForeground(disp, *gc, getColor());
	XDrawString(disp, *wnd, *gc, wndAttr.width - getX(), TEXT_HORIZONTAL_CENTER, getText().c_str(), getText().length());
}
