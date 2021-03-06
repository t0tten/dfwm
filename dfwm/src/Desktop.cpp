#include "../include/Desktop.h"
#include "../include/logger.h"

#include <X11/Xatom.h>

Desktop::Desktop (Display* disp, Window* parent, int x, int y, int width, int height) {
	this->disp		= disp;
	this->x			= x;
	this->y			= y;
	this->width		= width;
	this->height		= height;
	this->root		= parent;

	this->wgap		= WINDOW_GAP;

	this->size		= 5;
	this->amountLeft	= 0;
	this->amountRight	= 0;

	this->left		= new Window[size];	
	this->right		= new Window[size];
	
	this->currFocus		= -1;
}

Desktop::~Desktop () {
	LOGGER_DEBUG("Desktop::~Desktop");
	for(int i = 0; i < amountLeft; i++) {
		XDestroyWindow(disp, left[i]);
	}
	for(int i = 0; i < amountRight; i++) {
		XDestroyWindow(disp, right[i]);
	}

	delete[] left;
	delete[] right;
}

void Desktop::show() {
	LOGGER_DEBUG("void Desktop::show()");
	for(int i = 0; i < amountLeft; i++) 	XMapWindow(disp, left[i]);
	for(int i = 0; i < amountRight; i++) 	XMapWindow(disp, right[i]);
}

void Desktop::hide() {
	LOGGER_DEBUG("void Desktop::hide() {");
	for(int i = 0; i < amountRight; i++) 	XUnmapWindow(disp, right[i]);
	for(int i = 0; i < amountLeft; i++)	XUnmapWindow(disp, left[i]);
}

void Desktop::redraw() {}

void Desktop::moveFirstWndToLeft() {
	if(amountRight > 0) {
		addWindow(right[0], left, amountLeft);
		for(int i = 0; i < (amountRight - 1); i++) {
			right[i] = right[i+1];
		}	
		amountRight--;
		resizeWindows();
	}
}

void Desktop::moveLastWndToRight() {
	if(amountLeft > 1) {
		amountRight++;
		for(int i = amountRight - 1; i >= 0; i--) {
			right[i] = right[i-1];
		}
		right[0] = left[amountLeft-1];
		amountLeft--;
		resizeWindows();
	}
}

void Desktop::moveCurrWndToLeft() {
	moveToLeft(currFocus);
}

void Desktop::moveCurrWndToRight() {
	moveToRight(currFocus);
}

void Desktop::moveCurrWndUpByOne() {
	int index = findWindow(currFocus, left, amountLeft);
	/* Window found on left side */
	if(index != -1 && index != 0) {
		Window tmp 	= left[index];
		left[index] 	= left[index-1];
		left[index-1] 	= tmp;
	} else {
		index = findWindow(currFocus, right, amountRight);
		/* Window found on right side */
		if(index != -1 && index != 0) {
			Window tmp 	= right[index];
			right[index] 	= right[index-1];
			right[index-1] 	= tmp;
			}
	}

	if(index != -1) resizeWindows();
}

void Desktop::moveCurrWndDownByOne() {
	int index = findWindow(currFocus, left, amountLeft);
	/* Window found on left side */
	if(index != -1 && index != (amountLeft - 1)) {
		Window tmp 	= left[index];
		left[index] 	= left[index+1];
		left[index+1] 	= tmp;
	} else {
		index = findWindow(currFocus, right, amountRight);
		/* Window found on right side */
		if(index != -1 && index != (amountRight - 1)) {
			Window tmp 	= right[index];
			right[index] 	= right[index+1];
			right[index+1] 	= tmp;
		}
	}

	if(index != -1) resizeWindows();
}

void Desktop::moveToLeft(Window window) {
	LOGGER_DEBUG("void Desktop::moveToLeft(Window window)");
	if(amountRight > 0) {
		int index = findWindow(window, right, amountRight);

		/* If the window is found */
		if(index != -1) {
			addWindow(window, left, amountLeft);
			//if(amountRight > 1) {
			for(int i = index; i < (amountRight - 1); i++) right[i] = right[i + 1];
			//}
			amountRight--;
		}
		resizeWindows();
	}
}

void Desktop::moveToRight(Window window) {
	LOGGER_DEBUG("void Desktop::moveToRight(Window window)");
	if(amountLeft > 1) {
		int index = findWindow(window, left, amountLeft);
		/* If the window is found */
		if(index != -1) {
			addWindow(left[index], right, amountRight);
			//if(amountLeft > 1) {
				for(int i = index; i < (amountLeft - 1); i++) {
					left[i] = left[i + 1];
				}
			//}
			amountLeft--;
		}
		resizeWindows();
	}
}

void Desktop::swapFirstWindows() {
	if(amountLeft > 0 && amountRight > 0) {
		Window tmp 	= left[0];
		left[0] 	= right[0];
		right[0] 	= tmp;
	}
	
	resizeWindows();
}

void Desktop::addWindow(Window window, Window*& arr, int& size) {
	LOGGER_DEBUG("void Desktop::addWindow(Window window, Window*& arr, int& size)");

        expandArray(arr, size);
        arr[size] = window;
        size++;
}

void Desktop::addWindow(Window window) {
	LOGGER_DEBUG("void Desktop::addWindow(Window window)");
	if(findWindow(window, left, amountLeft) == -1 && findWindow(window, right, amountRight) == -1) {
		if(amountLeft <= 0 && findWindow(window, left, amountLeft) == -1) {
	 		addWindow(window, left, amountLeft);
			setCurrentFocusedWindow(window);
		} else	addWindow(window, right, amountRight);
		resizeWindows();
	}
}

int Desktop::findWindow(Window window, Window* arr, int size) {
	bool found = false;
	int index = -1;
        int i;
	for(int i = 0; i < size && !found; i++) {
		if(arr[i] == window) {
			found	= true;
			index 	= i;
		}
	}
	return index;
}

void Desktop::killCurrentWindow() {
	LOGGER_DEBUG("void Desktop::killCurrentWindow()");

	if(currFocus != -1 && this->windowExists(currFocus)) {
                XGrabServer(this->disp);

		LOGGER_DEBUG("TRYING TO KILL");
		
		XEvent eKill;
		eKill.xclient.type = ClientMessage;
		eKill.xclient.window = currFocus;
		eKill.xclient.message_type = XInternAtom(disp, "WM_PROTOCOLS", true);
		eKill.xclient.format = 32;
		eKill.xclient.data.l[0] = XInternAtom(disp, "WM_DELETE_WINDOW", false);
		eKill.xclient.data.l[1] = CurrentTime;
		XSendEvent(disp, currFocus, False, NoEventMask, &eKill);

                //XUnmapWindow(this->disp, currFocus);
                //removeWindow(currFocus);

                XSync(this->disp, False);
                XUngrabServer(this->disp);
	}
}

bool Desktop::removeWindow(Window window) {
	LOGGER_DEBUGF("removeWindow: %lu, left=%d, right=%d", window, 
                        amountLeft, amountRight);

	if(window != 0 && window != *this->root && this->windowExists(window)) {

		/* Search left side */
		int index	= -1;
		bool isLeft	= true;
		index = findWindow(window, left, amountLeft);

		/* If not found, search right side */
		if(index == -1) {
			index = findWindow(window, right, amountRight);
			isLeft = false;
		}

		/* Remove from array if found */
		if(index != -1) {
			if(window == currFocus) currFocus = -1;
			if(isLeft) {
				for(int i = index; i < (amountLeft - 1); i++) left[i] = left[i + 1];
				amountLeft--;
			} else {
				for(int i = index; i < (amountRight - 1); i++) right[i] = right[i + 1];
				amountRight--;
			}		
					
			if(amountLeft <= 0 && amountRight > 0) moveToLeft(right[0]);

                        if(amountLeft > 0) {
                                resizeWindows();
                        }
		}

	        LOGGER_DEBUGF("removed window: %lu, left=%d, right=%d", window, 
                                amountLeft, amountRight);
	}
}

void Desktop::expandArray(Window*& arr, int amount) {
	LOGGER_DEBUG("void Desktop::expandArray(Window*& arr, int amount)");
	if(size <= amount) {
		size *= 2;
		Window* tmp = new Window[size];
		for(int i = 0; i < amount; i++) tmp[i] = arr[i];
		delete[] arr;
		arr = NULL;
		arr = tmp;
	}
}

void Desktop::resizeWindows() {
        LOGGER_INFO("resizing windows()");

	/* Resize windows on the left side */
	if(amountLeft > 0) {
		int wX		= x + wgap;
		int wY		= y + wgap;
		int wWidth 	= (amountRight == 0) ? width - (2 * wgap) : (width / 2) - (2 * wgap); 
		int wHeight	= (height - ((amountLeft + 1) * wgap) - y) / amountLeft;

		for(int i = 0; i < amountLeft; i++) {
			//--------
			XConfigureEvent ce;

                	ce.type = ConfigureNotify;
               	 	ce.display = disp;
               	 	ce.event = left[i];
               	 	ce.window = left[i];
               	 	ce.x = wX;
               	 	ce.y = wY;
               	 	ce.width = wWidth;
               	 	ce.height = wHeight;
               	 	ce.border_width = 1;
               	 	ce.above = None;
               	 	ce.override_redirect = False;
               	 	XSendEvent(disp, left[i], False, StructureNotifyMask, (XEvent *)&ce);
			//-------
			XMoveResizeWindow(disp, left[i], wX, wY, wWidth, wHeight);
			wY = wHeight + wgap + wY;
		}	
	}

	/* Resize windows on the right side */
	if(amountRight > 0) {
		int wX		= (width / 2);
		int wY		= y + wgap;
		int wWidth 	= (width / 2) - wgap; 
		int wHeight	= (height - ((amountRight + 1) * wgap) - y) / amountRight;

		for(int i = 0; i < amountRight; i++) {
			//--------
			XConfigureEvent ce;

                	ce.type = ConfigureNotify;
               	 	ce.display = disp;
               	 	ce.event = right[i];
               	 	ce.window = right[i];
               	 	ce.x = wX;
               	 	ce.y = wY;
               	 	ce.width = wWidth;
               	 	ce.height = wHeight;
               	 	ce.border_width = 1;
               	 	ce.above = None;
               	 	ce.override_redirect = False;
               	 	XSendEvent(disp, right[i], False, StructureNotifyMask, (XEvent *)&ce);
			//-------
			XMoveResizeWindow(disp, right[i], wX, wY, wWidth, wHeight);
			wY = wHeight + wgap + wY;
		}	
	}
}

void Desktop::openProgram(std::string program) {
	LOGGER_DEBUG("void Desktop::openProgram(std::string program)");
	program += " &";
	system(program.c_str());
}

void Desktop::setCurrentFocusedWindow(Window window) {
	LOGGER_DEBUG("Desktop::setCurrentFocusedWindow(Window window)");
	if(currFocus != -1) {
		XSetWindowBorder(disp, currFocus, COL_BORDER);
	}
	
	this->currFocus = window;
	XSetWindowBorder(disp, window, COL_BORDER_SELECTED);
	
	XWindowChanges wChange;
	wChange.border_width = BORDER_WIDTH;
	XConfigureWindow(disp, window, CWBorderWidth, &wChange);
	resizeWindows();
}

bool Desktop::gotWindows() {
	return ((amountLeft + amountRight) > 0) ? true : false;
}

Window Desktop::getCurrentFocusedWindow() {
        return this->currFocus;
}

Window Desktop::popCurrentWindow() {
	Window wnd = currFocus;
	removeWindow(wnd);
	return wnd;
}

bool Desktop::windowExists(Window window) {
	int index = findWindow(window, left, amountLeft);
	if(index == -1) index = findWindow(window, right, amountRight);

	if(index == -1) return false;
	else		return true;
}









