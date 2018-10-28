#include "../include/StatusBarItem.h"

StatusBarItem::StatusBarItem(int x, int color) {
	this->x = x;
	this->y = 0;
	this->height = 25;
	this->color = color;
}

StatusBarItem::~StatusBarItem() {}

void StatusBarItem::setText(std::string text) { this->text = text; }
std::string StatusBarItem::getText() { return this->text; }

void StatusBarItem::setX(int x) { this->x = x; }
int StatusBarItem::getX() { return this->x; }
int StatusBarItem::getY() { return this->y; }
int StatusBarItem::getWidth() { return (this->text.length() * 6.5) + x; }
int StatusBarItem::getHeight() { return this->height; }
int StatusBarItem::getColor() { return this->color; }

void StatusBarItem::draw(Display* disp, Window* wnd, GC* gc) {
	XSetForeground(disp, *gc, this->color);
	XDrawString(disp, *wnd, *gc, x, TEXT_HORIZONTAL_CENTER, text.c_str(), text.length());
}
