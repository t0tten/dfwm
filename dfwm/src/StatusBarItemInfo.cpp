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

void StatusBarItemInfo::checkClick(int x, int y) {
	/*std::cout << "checking click" << std::endl;
	std::cout << "X: " << x << ", Y: " << y << std::endl;
	std::cout << "MyX: " << getX() << ", MyY: " << getY() << std::endl;
	std::cout << "MyWidth: " << getWidth() << ", MyHeight: " << getHeight() << std::endl;*/

	int tmpX = xWindowAttributes.width - getX();
	if(x > tmpX && x < (tmpX + getWidth()) && y > getY() && y < (getY() + getHeight())) {
		std::cout << "CLICKED ON ME: " << getText() << std::endl;
	}
}
