#include "../include/Launcher.h"

Launcher::Launcher (Display* disp, Window* parent) {
	this->disp 	= disp;
	
	XWindowAttributes wndAttr;
        XGetWindowAttributes (disp, *parent, &wndAttr);

	this->width 	= 300;
	this->height 	= 50;

        this->x    	= (wndAttr.width / 2) - (width / 2);
        this->y		= (wndAttr.height / 4) - (height / 2);	

	this->results	= NULL;

	this->launcher	= XCreateSimpleWindow(disp, *parent, x, y, width, height, 0, 0, -1);
	this->gc 	= XCreateGC(disp, launcher, 0, NULL);

	this->setState(HIDING);
}

Launcher::~Launcher () {
	XDestroyWindow(disp, launcher);
	if(results != NULL) delete[] results;
}

void Launcher::show() {
	this->setState(SHOWING);
	XMapWindow(disp, launcher);
	XRaiseWindow(disp, launcher);
	this->draw();
}

void Launcher::hide() {
	this->searchPhrase = "";
	this->setState(HIDING);
	XUnmapWindow(disp, launcher);
}

void Launcher::draw() {
	XSetForeground(disp, gc, COL_LAUNCHER_BG);
	XFillRectangle(disp, launcher, gc, 0, 0, width, height);

	XSetForeground(disp, gc, COL_LAUNCHER_FG);
	if(searchPhrase.length() == 0) {
		XDrawString(disp, launcher, gc, 20, (height / 2) + 5, "Command...", 10);
	} else {
		XDrawString(disp, launcher, gc, 20, (height / 2) + 5, searchPhrase.c_str(), searchPhrase.length());
	}
}

void Launcher::redraw() {
	XEvent exp;
        exp.type                = Expose;
        exp.xexpose.window      = launcher;
        XSendEvent(disp, launcher, False, ExposureMask, &exp);
}

void Launcher::addChar(char* c) {
	std::string toAdd = c;
	if(toAdd == "slash") 		this->searchPhrase += "/";
	else if(toAdd == "semicolon") 	this->searchPhrase += ";";
	else if(toAdd == "equal") 	this->searchPhrase += "=";
	else if(toAdd == "backslash") 	this->searchPhrase += "\\";
	else if(toAdd == "space") 	this->searchPhrase += " ";
	else if(toAdd == "minus") 	this->searchPhrase += "-";
	else if(toAdd.length() == 1)	this->searchPhrase += toAdd;

	this->search();	
	this->redraw();
}

void Launcher::removeLastChar() {
	if(searchPhrase.length() > 0) this->searchPhrase.pop_back();
	this->search();
}

Window Launcher::getLauncherWindow() { return this->launcher; }

std::string Launcher::getText() { return this->searchPhrase; }

void Launcher::search() {
	/* Used to search in path after suggestions */
	this->path =  std::getenv("PATH");
}

LauncherState Launcher::getState() { return this->state; }
void Launcher::setState(LauncherState state) { this->state = state; }
int Launcher::getWindowID() { return this->launcher; }

