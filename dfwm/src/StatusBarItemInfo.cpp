#include "../include/StatusBarItemInfo.h"

StatusBarItemInfo::StatusBarItemInfo(int color, XWindowAttributes winAttrs,
                int x) : StatusBarItem(x, color) {
        this->xWindowAttributes = winAttrs;
}

StatusBarItemInfo::~StatusBarItemInfo() {}

void StatusBarItemInfo::setText(std::string text) {
	this->StatusBarItem::setText(text);
	this->setX((text.length() * 10) + getX());
}

void StatusBarItemInfo::draw(Display* disp, Window* wnd, GC* gc) {
	XSetForeground(disp, *gc, getColor());
	XDrawString(disp, *wnd, *gc, xWindowAttributes.width - getX(), 
                        TEXT_HORIZONTAL_CENTER, getText().c_str(), 
                        getText().length());
}


