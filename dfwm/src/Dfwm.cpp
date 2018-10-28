#include "../include/Dfwm.h"

Dfwm::Dfwm () {
	disp = XOpenDisplay(NULL);
	if(disp != NULL) init();
}

Dfwm::Dfwm (std::string displayName) {
	disp = XOpenDisplay(displayName.c_str());
	if(disp != NULL) init();
}

Dfwm::~Dfwm () {
	XUngrabKey(disp, code, AnyKey, root);
	XCloseDisplay(disp);
	delete this->keys;
	delete this->bar;
	delete this->menu;
	for(int i = 0; i < maxDesktops; i++) {
		delete desktop[i];
	}
	delete[] desktop;
}

Window* Dfwm::findAllWindows(unsigned int &nrOfWindows) {
	Window tmp, tmp1;
	Window* windows;
	XQueryTree(disp, root, &tmp, &tmp1, &windows, &nrOfWindows);

	int size  = 10;
	int nrWin = 0;
	Window* viewable = new Window[size];
	for(int i = 0; i < nrOfWindows; i++) {
		XWindowAttributes wndAttr;
		XGetWindowAttributes(disp, windows[i], &wndAttr);
		if(wndAttr.map_state == IsViewable) {
			viewable[nrWin++] = windows[i];

			if(size <= nrWin) {
				size *= 2;
				Window* tmp = new Window[size];

				for(int j = 0; j < nrWin; j++) {
					tmp[i] = viewable[i];
				}

				delete[] viewable;
				viewable = NULL;
				viewable = tmp;
			}
		}
	}

	nrOfWindows = nrWin;
	return windows;
}

void Dfwm::init () {
	this->screen 	= DefaultScreen(disp);
	this->nrScreens	= ScreenCount(disp);
	for(int i = 0; i < nrScreens; i++) {
		this->screenis = ScreenOfDisplay(disp, i);
		std::cout << "Screen: " << screenis->width << ", " << screenis->height << std::endl;
	}
	std::cout << "NrScreens: " << nrScreens << std::endl;
	this->sWidth 	= XDisplayWidth(disp, screen);
	this->sHeight	= XDisplayHeight(disp, screen);
	this->root	= RootWindow(disp, screen);	
	this->maxDesktops = 5;
	if(maxDesktops > 9) maxDesktops = 9;

	/* Change property of root */
	XSetWindowAttributes rootNewAttr;
	rootNewAttr.event_mask = ROOT_EVENT_MASK; 
	XChangeWindowAttributes(disp, root, CWEventMask|CWCursor, &rootNewAttr);
	
	/* Find all open windows */
	this->size 	= 20;
	this->nrOfOpen	= 0;
	this->opened	= new Window[size];

	unsigned int nrOfWindows;
	Window* windows = findAllWindows(nrOfWindows);
	std::cout << "NrOfWindows: " <<  nrOfWindows << std::endl;
	
	XWindowAttributes rootAttr;
	XGetWindowAttributes(disp, root, &rootAttr);

	//KeyCode code;
	XGrabKey(disp, code, AnyKey, root,
        	True, GrabModeAsync, GrabModeAsync);

	XSelectInput (disp, root, EVENT_MASK);

	XMapWindow (disp, root);
	this->selected 	= 1;
	this->running 	= true;
	this->keys 	= new KeyBindings();
	this->bar 	= new StatusBar(disp, &root, &selected);
	this->menu 	= new Menu(disp, &root, 150, GRAY, LEFT, &selected, maxDesktops);
	this->bar->redraw();
	this->desktop	= new Desktop*[maxDesktops];
	
	for(int i = 0; i < maxDesktops; i++) {
		this->desktop[i] = new Desktop(disp, &root, 0, bar->getHeight(), sWidth, sHeight, 10, 1, COL_BORDER_COLOR);	
	}

	addOpen(bar->getWindowID());
	addOpen(menu->getWindowID());		

	/* Assign all found windows to a desktop */
	for(int i = 0; i < nrOfWindows; i++) {
		/* REMOVE WHEN TESTING */
		//this->desktop[selected - 1]->addWindow(windows[i]);
		addOpen(windows[i]);
	}

	//this->desktop[selected - 1]->openProgram("hej", opened, nrOfOpen);
	delete[] windows;
	this->desktop[selected - 1]->show();
}

void Dfwm::addOpen(Window window) {
	if(size <= nrOfOpen) {
		size *= 2;
		Window* tmp = new Window[size];
		for(int i = 0; i < nrOfOpen; i++) {
			tmp[i] = opened[i];
		}

		delete[] opened;
		opened = NULL;
		opened = tmp;
	}
	
	opened[nrOfOpen++] = window;
}

void Dfwm::removeOpen(Window window) {

}

void Dfwm::run () {
	while(running) {
		XNextEvent (disp, &e);
		
		Window wnd = this->desktop[selected - 1]->findAllWindows(this->opened, this->nrOfOpen);
		if(wnd != -1) {
			this->desktop[selected - 1]->addWindow(wnd);
			this->addOpen(wnd);
		}		

		//std::cout << "Event" << std::endl;
		if (e.type == Expose) {
			//std::cout << "Expose" << std::endl;
			if(e.xexpose.window == bar->getWindowID()) bar->draw();
			if(e.xexpose.window == menu->getWindowID()) menu->draw();
		}
		if (e.type == KeyPress) {
			//std::cout << "KeyPress" << std::endl;
			keys->translate_KeyDown(this, &e.xkey);
		}
		if (e.type == KeyRelease) keys->translate_KeyUp(this, &e.xkey);
		if (e.type == ButtonRelease) std::cout << "ButtonRelease" << std::endl;
		if (e.type == PropertyNotify) {
			std::cout << "PropertyNotify" << std::endl;
			std::string txt = "PropertyNotify";

			this->bar->setText(txt);	
			this->bar->redraw();
		}
	}
}

void Dfwm::quit() { running = false; }

void Dfwm::setSelected(int selected) { 
	this->desktop[this->selected - 1]->hide();
	this->selected = selected; 
	if(this->selected > maxDesktops) this->selected = maxDesktops;
	this->desktop[this->selected - 1]->show();
}

void Dfwm::incrementSelected() {
	this->desktop[selected - 1]->hide();
	selected++;
	if(selected > maxDesktops) selected = 1;
	this->desktop[selected - 1]->show();
} 

void Dfwm::decrementSelected() {
	this->desktop[selected - 1]->hide();
	selected--;
	if(selected < 1) selected = maxDesktops;
	this->desktop[selected - 1]->show();
}

void Dfwm::redraw() {
	XClearArea(disp, root, 0,0, 500, 500, True);
}

Menu* Dfwm::getMenu() { return this->menu; }
StatusBar* Dfwm::getStatusBar() { return this->bar; }
Desktop* Dfwm::getCurrentDesktop() { return this->desktop[selected - 1]; }

Window* Dfwm::getOpened() { return this->opened; }
int Dfwm::getNrOfOpen() { return this->nrOfOpen; }
