#include "../include/Dfwm.h"
#include "../include/logger.h"
#include "../include/DfwmWindow.h"

Dfwm::Dfwm () {
	disp = XOpenDisplay(NULL);
	if(disp != NULL) init();
}

Dfwm::Dfwm (std::string displayName) {
	disp = XOpenDisplay(displayName.c_str());
	if(disp != NULL) init();
}

Dfwm::~Dfwm () {
	LOGGER_DEBUG("Calls destructor");
	for(int i = 0; i < maxDesktops; i++) {
		delete desktop[i];
	}

	delete[] desktop;
	delete this->keys;
	delete this->bar;
	delete this->menu;
	delete this->launcher;
	delete[] this->desktopHaveWindow;
	XUngrabKey(disp, code, AnyKey, root);
	XCloseDisplay(disp);
}


Display* Dfwm::getDisplay() {
        return this->disp;
}

Window Dfwm::getRoot() {
        return this->root;
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
        this->configuration = new Configuration();
	this->screen 	= DefaultScreen(disp);
	this->sWidth 	= XDisplayWidth(disp, screen);
	this->sHeight	= XDisplayHeight(disp, screen);
	this->root	= RootWindow(disp, screen);	
	this->maxDesktops = 5;
	if(maxDesktops > 9) maxDesktops = 9;
	if(maxDesktops <= 0) maxDesktops = 1;

	/* Change property of root */
	initAtoms();
	XSetWindowAttributes rootNewAttr;
	rootNewAttr.event_mask = ROOT_EVENT_MASK; 
	XChangeWindowAttributes(disp, root, CWEventMask, &rootNewAttr);
	//XSelectInput(disp, root, rootNewAttr.event_mask);
        //
        XSelectInput(disp, root, SubstructureRedirectMask|
                        SubstructureNotifyMask|ButtonPressMask|
                        PointerMotionMask);
	
	/* Find all open windows */
	this->size 	= 20;
	this->nrOfMapped= 0;
	this->mapped	= new Window[size];

	unsigned int nrOfWindows;
	Window* windows = findAllWindows(nrOfWindows);
	LOGGER_DEBUGF("NrOfWindows: %d", nrOfWindows);
	
	XWindowAttributes rootAttr;
	XGetWindowAttributes(disp, root, &rootAttr);

	//XSelectInput (disp, root, ROOT_EVENT_MASK); //| SubstructureRedirectMask);
	XMapWindow (disp, root);
	
	/* Add windows to dfwm */
	this->selected 	= 1;
	this->running 	= true;
	this->keys 	= new KeyBindings(disp);
	this->bar 	= new StatusBar(disp, &root, &selected);
	this->menu 	= new Menu(disp, &root, 150, &selected, maxDesktops);
	this->bar->redraw();
	this->desktop	= new Desktop*[maxDesktops];
	this->desktopHaveWindow = new bool[maxDesktops];
	this->launcher	= new Launcher(disp, &root);	

	for(int i = 0; i < maxDesktops; i++) {
		this->desktop[i] = new Desktop(disp, &root, 0, bar->getHeight(), sWidth, sHeight);
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

void Dfwm::initAtoms() {
	NET_CLIENT_LIST 		= XInternAtom(disp, "_NET_CLIENT_LIST", False);
	NET_WM_WINDOW_TYPE_NORMAL 	= XInternAtom(disp, "_NET_WM_WINDOW_TYPE_NORMAL", False);
	NET_WM_WINDOW_TYPE 		= XInternAtom(disp, "_NET_WM_WINDOW_TYPE", False);
	NET_WM_STATE 			= XInternAtom(disp, "_NET_WM_STATE", False);
	NET_SUPPORTED			= XInternAtom(disp, "_NET_SUPPORTED", False);
	NET_ACTIVE_WINDOW 		= XInternAtom(disp, "_NET_ACTIVE_WINDOW", False);
	NET_WM_NAME	 		= XInternAtom(disp, "_NET_WM_NAME", False);
	NET_WM_STATE_FULLSCREEN		= XInternAtom(disp, "_NET_WM_STATE_FULLSCREEN", False);
	NET_WM_WINDOW_TYPE_DIALOG 	= XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DIALOG", False);

	WM_DELETE_WINDOW   		= XInternAtom(disp, "WM_DELETE_WINDOW", False);
	WM_PROTOCOLS			= XInternAtom(disp, "WM_PROTOCOLS", False);
	WM_TAKE_FOCUS			= XInternAtom(disp, "WM_TAKE_FOCUS", False);
	WM_STATE			= XInternAtom(disp, "NET_STATE", False);

        XChangeProperty(disp, root, NET_SUPPORTED, XA_ATOM, 32, PropModeReplace, (unsigned char *) &NET_CLIENT_LIST, 1);
        XChangeProperty(disp, root, NET_SUPPORTED, XA_ATOM, 32, PropModeReplace, (unsigned char *) &NET_WM_WINDOW_TYPE_NORMAL, 1);
        XChangeProperty(disp, root, NET_SUPPORTED, XA_ATOM, 32, PropModeReplace, (unsigned char *) &NET_WM_WINDOW_TYPE, 1);
        XChangeProperty(disp, root, NET_SUPPORTED, XA_ATOM, 32, PropModeReplace, (unsigned char *) &NET_WM_STATE, 1);
        XChangeProperty(disp, root, NET_SUPPORTED, XA_ATOM, 32, PropModeReplace, (unsigned char *) &NET_SUPPORTED, 1);
        XChangeProperty(disp, root, NET_SUPPORTED, XA_ATOM, 32, PropModeReplace, (unsigned char *) &NET_ACTIVE_WINDOW, 1);
        XChangeProperty(disp, root, NET_SUPPORTED, XA_ATOM, 32, PropModeReplace, (unsigned char *) &NET_WM_NAME, 1);
        XChangeProperty(disp, root, NET_SUPPORTED, XA_ATOM, 32, PropModeReplace, (unsigned char *) &NET_WM_STATE_FULLSCREEN, 1);
        XChangeProperty(disp, root, NET_SUPPORTED, XA_ATOM, 32, PropModeReplace, (unsigned char *) &NET_WM_WINDOW_TYPE_DIALOG, 1);
	
        XDeleteProperty(disp, root, NET_CLIENT_LIST);
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

void Dfwm::translateClientMessage(XClientMessageEvent xclient) {
	LOGGER_DEBUGF("MESSAGE TYPE: %s", XGetAtomName(disp, xclient.message_type));

	if(xclient.message_type == NET_WM_STATE) {
		//addWindowToDesktop(xclient.window);
	}
}

void Dfwm::checkWindow(Window window) {

        DfwmWindow win = DfwmWindow();
        DfwmStatus status = win.init(configuration, this->disp, window, root);
        if(status.isOk()) {
                LOGGER_DEBUG("Status is OK");
                addWindowToDesktop(win.getWindow());
        } else {
                LOGGER_DEBUG("FAILED TO INIT WINDOW");
        }
	
}

void Dfwm::handleXEvent() {
        switch(e.type) {
                case Expose:
                        drawGraphics(e.xexpose.window);
                        break;
                case KeyPress:
                        keys->translate_KeyDown(this, &e.xkey);
                        break;
                case KeyRelease:
                        keys->translate_KeyUp(this, &e.xkey);
                        break;
                case KeymapNotify:
                        LOGGER_INFO("KeymapNotify");
                        XRefreshKeyboardMapping(&e.xmapping);
                        break;
                case ClientMessage:
			std::cout << "ClientMessage" << std::endl;
                        translateClientMessage(e.xclient);
                        break;
                case DestroyNotify:
                        removeWindowFromDesktop(e.xdestroywindow.window);
                        break;
                case ConfigureRequest:
                        LOGGER_INFO("ConfigureRequest");

			if(this->desktop[selected - 1]->windowExists(e.xconfigurerequest.window)) {
				this->desktop[selected - 1]->resizeWindows();
			} else {
                                LOGGER_INFO("Unknown window");
				XConfigureRequestEvent *ev = &e.xconfigurerequest;
				/*XWindowChanges wc;

				wc.x = ev->x;
		                wc.y = ev->y;
	        	        wc.width = ev->width;
		                wc.height = ev->height;
		                wc.border_width = ev->border_width;
		                wc.sibling = ev->above;
		                wc.stack_mode = ev->detail;
		                XConfigureWindow(disp, ev->window, ev->value_mask, &wc);*/

                                checkWindow(ev->window);
			}

                        XSync(this->disp, False);
                        break;
                case EnterNotify:
			grabFocused(e.xcrossing.window, e.xcrossing.mode);
                        break;
                case FocusIn:
                        grabFocused(e.xfocus.window, e.xfocus.mode);
                        break;
                case MappingNotify:
			LOGGER_INFO("MappingNotify");
                        break;
                case MapNotify:
                       	LOGGER_INFO("MapNotify");
                        break;
                case MapRequest:
                        LOGGER_INFO("MapRequest");
			checkWindow(e.xmaprequest.window);
			redraw();
                        break;
                case PropertyNotify:
                        LOGGER_INFO("PropertyNotify");
                        this->propertyNotify(&e.xproperty);
                        break;
                case UnmapNotify: 
                        LOGGER_INFO("UnmapNotify");
                        break;
		//case MotionNotify:
			//std::cout << "MotionNotify" << std::endl;
			//break;
        }
}

void Dfwm::propertyNotify(XPropertyEvent *ev) {
	if ((ev->window == root) && (ev->atom == XA_WM_NAME)) {
                LOGGER_INFO("root && XA_WM_NAME");
        } else if(ev->state == PropertyDelete) {
                LOGGER_INFO("PropertyDelete");
        } else if(this->desktop[selected - 1]->windowExists(ev->window)) {
                LOGGER_INFO("Window Exists");
        }
}

void Dfwm::run () {

        int x11_fd = ConnectionNumber(disp);
        fd_set in_fds;

        struct timeval tv;
        unsigned int count = 0;

        this->keys->setup(this);

	while(running) {
                FD_ZERO(&in_fds);
                FD_SET(x11_fd, &in_fds);

                tv.tv_usec = 50;
                tv.tv_sec = 0;

                select(x11_fd + 1, &in_fds, NULL, NULL, &tv);

                while(XPending(disp)) {
                	XNextEvent(disp, &e);
                	this->handleXEvent();
                }

                if(count % 100 == 0) {
                        this->bar->redraw();
                }

                count++;
	}
}

void Dfwm::quit() { running = false; }

void Dfwm::setSelected(int selected) { 
	if(this->selected != selected) {
		this->desktop[this->selected - 1]->hide();
		this->selected = selected; 
		if(this->selected > maxDesktops) this->selected = maxDesktops;
		this->desktop[this->selected - 1]->show();
	}
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

void Dfwm::drawGraphics(Window window) {	
	if(window == bar->getWindowID()) bar->draw();
	else if(window == menu->getWindowID()) {
		for(int i = 0; i < maxDesktops; i++) {
			 this->desktopHaveWindow[i] = this->desktop[i]->gotWindows();
		} 
		menu->draw(desktopHaveWindow);
	} else if(window == launcher->getLauncherWindow()) launcher->draw();
}

void Dfwm::addWindowToDesktop(Window window) {
	LOGGER_DEBUG("addWindowToDesktop");
	XWindowAttributes wndAttr;
	XGetWindowAttributes(disp, window, &wndAttr);
        
        Atom type;
        Atom* atoms;
        unsigned long len, remain;
        int form;

        XGetWindowProperty(disp, window, NET_WM_WINDOW_TYPE, 0, 1024, False, XA_ATOM, &type, &form, &len, &remain, (unsigned char**)&atoms);

        if(len == 0) {
                this->desktop[selected - 1]->addWindow(window);
                addMapped(window);
        }


        for(int i = 0; i < (int)len; i++) { 
                LOGGER_DEBUGF("addWinToDesk: %d:%s", i, XGetAtomName(disp, atoms[i]));
                if(atoms[i] == NET_WM_WINDOW_TYPE_NORMAL) {
                        this->desktop[selected - 1]->addWindow(window);
                        addMapped(window);
                } 
        }
}

void Dfwm::removeWindowFromDesktop(Window window) {
        removeMapped(window);
        this->desktop[selected - 1]->removeWindow(window);
}

bool Dfwm::windowIsNotDfwm(Window window) {
	if (window == this->bar->getWindowID()) {
		LOGGER_DEBUG("Window is bar");
		std::cout << "Window is bar" << std::endl;
 		return false;
	}
	else if (window == this->menu->getWindowID()) {
		LOGGER_DEBUG("Window is menu");
		std::cout << "Window is menu" << std::endl;
		return false; 
	}
	else if (window == this->launcher->getWindowID()) { 
		LOGGER_DEBUG("Window is launcher");
		std::cout << "Window is launcher" << std::endl;
		return false;
	}
	else if (window == this->root) {
		LOGGER_DEBUG("Window is root");
		std::cout << "Window is root" << std::endl;
		return false;
	}

	return true;
}

void Dfwm::grabFocused(Window window, int mode) {
	if(windowIsNotDfwm(window) && window != 0) {
		XWindowAttributes wndAttr;
		XGetWindowAttributes(disp, window, &wndAttr);

		if(mode != NotifyUngrab && wndAttr.map_state == IsViewable) {
			Atom type;
			Atom* atoms;
			unsigned long len, remain;
			int form;

			try {
				LOGGER_DEBUGF("%d", XGetWindowProperty(disp, window, NET_WM_WINDOW_TYPE, 0, 1024, False, XA_ATOM, &type, &form, &len, &remain, (unsigned char**)&atoms));
				for(int i = 0; i < (int)len; i++) { 
					LOGGER_DEBUGF("%s", XGetAtomName(disp, atoms[i]));
					if(atoms[i] == NET_WM_WINDOW_TYPE_NORMAL) {
						char* name;
						if(XFetchName(disp, window, &name)) {
							std::string s_name = name;
							this->bar->setText(s_name);
						} else this->bar->setText("Window X");
						this->bar->redraw();
						this->desktop[selected - 1]->setCurrentFocusedWindow(window);
					} 
				}
			} catch (char* e) {}
		}
	}
	
}



void Dfwm::moveCurrentWindowToDesktop(int moveToDesktop) {
	if(moveToDesktop != selected && moveToDesktop <= maxDesktops && moveToDesktop > 0) {
		Window wnd = this->desktop[selected - 1]->popCurrentWindow();
		if((int)wnd != -1) {
			this->desktop[moveToDesktop - 1]->addWindow(wnd);
			XUnmapWindow(disp, wnd);
		}
	}	
}
