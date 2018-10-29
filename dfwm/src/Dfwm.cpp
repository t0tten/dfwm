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
	delete this->launcher;
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
	this->nrOfMapped	= 0;
	this->mapped	= new Window[size];

	unsigned int nrOfWindows;
	Window* windows = findAllWindows(nrOfWindows);
	std::cout << "NrOfWindows: " <<  nrOfWindows << std::endl;
	
	XWindowAttributes rootAttr;
	XGetWindowAttributes(disp, root, &rootAttr);

	XGrabKey(disp, code, AnyKey, root,
        	True, GrabModeAsync, GrabModeAsync);

	XSelectInput (disp, root, EVENT_MASK);

	XMapWindow (disp, root);
	this->selected 	= 1;
	this->running 	= true;
	this->keys 	= new KeyBindings(disp);

	this->bar 	= new StatusBar(disp, &root, &selected);
	this->menu 	= new Menu(disp, &root, 150, GRAY, LEFT, &selected, maxDesktops);
	this->bar->redraw();
	this->desktop	= new Desktop*[maxDesktops];
	this->launcher	= new Launcher(disp, &root, GRAY, LGT_GRAY);	

	for(int i = 0; i < maxDesktops; i++) {
		this->desktop[i] = new Desktop(disp, &root, 0, bar->getHeight(), sWidth, sHeight, 10, 1, COL_BORDER_COLOR);	
	}

	/* Register windows */
	addMapped(bar->getWindowID());
	addMapped(menu->getWindowID());		
	addMapped(launcher->getWindowID());

	/* Assign all found windows to a desktop */
	for(int i = 0; i < nrOfWindows; i++) {
		this->desktop[selected - 1]->addWindow(windows[i]);
		addMapped(windows[i]);
	}

	delete[] windows;
	this->desktop[selected - 1]->show();
}

void Dfwm::addMapped(Window window) {
	if(size <= nrOfMapped) {
		size *= 2;
		Window* tmp = new Window[size];
		for(int i = 0; i < nrOfMapped; i++) {
			tmp[i] = mapped[i];
		}

		delete[] mapped;
		mapped = NULL;
		mapped = tmp;
	}
	
	mapped[nrOfMapped++] = window;
}

void Dfwm::removeMapped(Window window) {
	bool found = false;
	int index = 0;
	for(; index < nrOfMapped && !found; index++) {
		if(window == mapped[index]) {
			found = true;
			break;
		}
	}

	if(found) {
		for(int i = index; i < (nrOfMapped - 1); i++) {
			mapped[i] = mapped[i + 1];	
		}

		nrOfMapped--;
	}
	
}

void Dfwm::run () {
	while(running) {
		XNextEvent (disp, &e);
		if (e.type == Expose)		drawGraphics(e.xexpose.window);
		if (e.type == KeyPress) 	keys->translate_KeyDown(this, &e.xkey);
		if (e.type == KeyRelease) 	keys->translate_KeyUp(this, &e.xkey);
		if (e.type == ClientMessage) 	addWindowToDesktop(e.xclient.window);
		if (e.type == DestroyNotify) 	removeWindowFromDesktop(e.xdestroywindow.window);

		if (e.type == ConfigureRequest)std::cout << "ConfigureRequest" << std::endl;
		if (e.type == ButtonRelease) 	quit();
		if (e.type == EnterNotify)  	std::cout << "EnterNotify" << std::endl;
		if (e.type == FocusIn)  	std::cout << "FocusIn" << std::endl;
		if (e.type == MappingNotify)  std::cout << "MappingNotify" << std::endl;
		if (e.type == MapRequest)  	std::cout << "MapRequest" << std::endl;
		if (e.type == MotionNotify)  	std::cout << "MotionNotify" << std::endl;
		if (e.type == PropertyNotify) std::cout << "PropertyNotify" << std::endl;
		if (e.type == UnmapNotify)  	std::cout << "UnmapNotify" << std::endl;
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

Menu* Dfwm::getMenu() 			{ return this->menu; }
StatusBar* Dfwm::getStatusBar() 	{ return this->bar; }
Desktop* Dfwm::getCurrentDesktop() 	{ return this->desktop[selected - 1]; }
Launcher* Dfwm::getLauncher() 		{ return this->launcher; }

Window* Dfwm::getMappedList() 		{ return this->mapped; }
int Dfwm::getNrOfMapped() 		{ return this->nrOfMapped; }

bool Dfwm::isMapped(Window wnd) {
	bool found = false;
	for(int i = 0; i < nrOfMapped && !found; i++) {
		if(mapped[i] == wnd) { found = true; break; }
	}

	return found;
}

void Dfwm::drawGraphics(Window window) {	
	if(window == bar->getWindowID()) bar->draw();
	else if(window == menu->getWindowID()) menu->draw();
	else if(window == launcher->getLauncherWindow()) launcher->draw();
}

void Dfwm::addWindowToDesktop(Window window) {
	XWindowAttributes wndAttr;
	XGetWindowAttributes(disp, window, &wndAttr);
	if(wndAttr.map_state == IsViewable && !isMapped(window)) {
		Atom type;
		Atom* atoms;
		unsigned long len, remain;
		int form;

		XGetWindowProperty(disp, window, XInternAtom(disp, "_NET_WM_WINDOW_TYPE", True), 0, 1024, False, XA_ATOM, &type, &form, &len, &remain, (unsigned char**)&atoms);

		for(int i = 0; i < (int)len; i++) { 
			std::cout << XGetAtomName(disp, atoms[i]) << std::endl;
			if(atoms[i] == XInternAtom(disp, "_NET_WM_WINDOW_TYPE_NORMAL", True)) {

				this->desktop[selected - 1]->addWindow(window);
				addMapped(window);
			} 
		}


	}
}

void Dfwm::removeWindowFromDesktop(Window window) {
	if(isMapped(window)) {
		removeMapped(window);
		this->desktop[selected - 1]->removeWindow(window);
	}
}

