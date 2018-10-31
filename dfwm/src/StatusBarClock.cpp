#include <stdio.h>
#include <time.h>

#include "../include/StatusBarClock.h"

StatusBarClock::StatusBarClock(int x, int color) : StatusBarItem(x, color) {
	this->setText(getTime());
}

StatusBarClock::~StatusBarClock() {}

std::string StatusBarClock::getTime() {
        char buf[64];

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        snprintf(buf, sizeof(buf), "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900,
                        tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
                        tm.tm_sec);

        std::string now = buf;

        return now;
}

void StatusBarClock::update() {
	this->setText(getTime());
}
