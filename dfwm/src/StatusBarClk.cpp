#include "../include/StatusBarClk.h"

StatusBarClk::StatusBarClk(int x, int color) : StatusBarItem(x, color) {
	this->setText(getTime());
}

StatusBarClk::~StatusBarClk() {}

std::string StatusBarClk::getTime() {
	return "[2018/10/02 23:54]";
}
