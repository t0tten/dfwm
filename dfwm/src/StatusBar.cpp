#include "../include/StatusBar.h"

StatusBar::StatusBar (Display* disp, Window* parent, int* selected) {
	XWindowAttributes parentAttr;
	XGetWindowAttributes(disp, *parent, &parentAttr);

	this->x = this->y = 0;
	this->width = parentAttr.width;
	this->height 	= 25;
	this->selected 	= selected;
	
	this->disp 	= disp;
	this->bar 	= XCreateSimpleWindow(disp, *parent, x, y, width, height, 0, 0, 0);
	this->gc = XCreateGC(disp, bar, 0, NULL);

	/* Desktop */
	this->dtop	= new StatusBarItemDesktop(10, COL_STATUS_BAR_DESKTOP_FG);

	/* Clock */
	this->clock 	= new StatusBarClock(dtop->getWidth(), COL_STATUS_BAR_CLOCK);

	/* Title */
	this->title	= new StatusBarItem(clock->getWidth(), COL_STATUS_BAR_TITLE);
	//this->title->setText("Terminal");
	
	/* Infos */
	this->nrOfInfos = 2;
	this->infos 	= new StatusBarItemInfo*[nrOfInfos];
	this->infos[0] 	= new StatusBarItemInfo(COL_STATUS_BAR_INFO, parentAttr);
	this->infos[0]->setText("V: 100%");
	this->infos[1] 	= new StatusBarItemInfo(COL_STATUS_BAR_INFO, parentAttr, 
                        infos[0]->getX());
	this->infos[1]->setText("B: 100%");

	this->show();
	XSelectInput(disp, bar, ExposureMask);
}

StatusBar::~StatusBar () {
	delete this->clock;
	delete this->title;
	for(int i = 0; i < nrOfInfos; i++) {
		delete infos[i];
	}
	delete[] infos;
	XDestroyWindow(disp, bar);
}

void StatusBar::show() {
	XMapWindow(disp, bar);
}

void StatusBar::draw() {
	XSetForeground(disp, gc, COL_STATUS_BAR_BG);
	XFillRectangle(disp, bar, gc, x, y, width, height);

	this->dtop->draw(disp, &bar, &gc, selected);
	this->clock->draw(disp, &bar, &gc);
	this->title->draw(disp, &bar, &gc);
	for(int i = 0; i < nrOfInfos; i++) {
		this->infos[i]->draw(disp, &bar, &gc);	
	}
}

int StatusBar::getWindowID() { return bar; }

void StatusBar::redraw() {
        this->update();

	XEvent exp;
	exp.type 		= Expose;
	exp.xexpose.window 	= bar;
	XSendEvent(disp, bar, False, ExposureMask, &exp);
}

int StatusBar::getHeight() { return this->height; }
void StatusBar::setText(std::string text) { this->title->setText(text); }

void StatusBar::update() {
        this->clock->update();
}

