#include "../include/Desktop.h"
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
	std::cout << "Desktop::~Desktop" << std::endl;
		/*Atom DELETE 	= XInternAtom(disp, "WM_DELETE_WINDOW", False);
		Atom PROTO 	= XInternAtom(disp, "WM_PROTOCOLS", True);	
	
		XEvent eKill;
		eKill.xclient.type = ClientMessage;
		eKill.xclient.message_type = XInternAtom(disp, "WM_PROTOCOLS", true);
		eKill.xclient.format = 32;
		eKill.xclient.data.l[0] = XInternAtom(disp, "WM_DELETE_WINDOW", false);
		eKill.xclient.data.l[1] = CurrentTime;
		*/

	for(int i = 0; i < amountLeft; i++) {
		XDestroyWindow(disp, left[i]);
		//eKill.xclient.window = left[i];
		//XSendEvent(disp, left[i], False, NoEventMask, &eKill);
	}
	for(int i = 0; i < amountRight; i++) {
		XDestroyWindow(disp, right[i]);
		//eKill.xclient.window = left[i];
		//XSendEvent(disp, left[i], False, NoEventMask, &eKill);
	}

	delete[] left;
	delete[] right;
}

void Desktop::show() {
	std::cout << "void Desktop::show()" << std::endl;
	for(int i = 0; i < amountLeft; i++) 	XMapWindow(disp, left[i]);
	for(int i = 0; i < amountRight; i++) 	XMapWindow(disp, right[i]);
}

void Desktop::hide() {
	std::cout << "void Desktop::hide() {" << std::endl;
	for(int i = 0; i < amountRight; i++) 	XUnmapWindow(disp, right[i]);
	for(int i = 0; i < amountLeft; i++)	XUnmapWindow(disp, left[i]);
}

void Desktop::redraw() {}

void Desktop::moveToLeft(Window window) {
	std::cout << "void Desktop::moveToLeft(Window window)" << std::endl;
	int index = findWindow(window, right, amountRight);
	/* If the window is found */
	if(index != -1) {
		addWindow(window, left, amountLeft);
		if(amountRight > 1) {
			for(int i = index; i < (amountRight - 1); i++) right[i] = right[i + 1];
		}
		amountRight--;
	}
}

void Desktop::moveToRight(Window window) {
	std::cout << "void Desktop::moveToRight(Window window)" << std::endl;
	int index = findWindow(window, left, amountLeft);
	/* If the window is found */
	if(index != -1) {
		addWindow(left[index], right, amountRight);
		if(amountLeft > 1) {
			for(int i = index; i < (amountLeft - 1); i++) {
				left[i] = left[i + 1];
			}
		}
		amountLeft--;
	}
}

void Desktop::addWindow(Window window, Window*& arr, int& size) {
	std::cout << "void Desktop::addWindow(Window window, Window*& arr, int& size)" << std::endl;
	expandArray(arr, size);
	XSetWindowBorder(disp, window, COL_BORDER);
	
	XWindowChanges wChange;
	wChange.border_width = BORDER_WIDTH;
	XConfigureWindow(disp, window, CWBorderWidth, &wChange);

	arr[size] = window;
	size++;
        XSelectInput(disp, window, EVENT_MASK);
}

void Desktop::addWindow(Window window) {
	std::cout << "void Desktop::addWindow(Window window)" << std::endl;
	if(amountLeft <= 0) 	addWindow(window, left, amountLeft);
	else  			addWindow(window, right, amountRight);
	resizeWindows();
}

int Desktop::findWindow(Window window, Window* arr, int size) {
	std::cout << "int Desktop::findWindow(Window window, Window* arr, int size)" << std::endl;
	bool found = false;
	int index = -1;
	for(int i = 0; i < size && !found; i++) {
		std::cout << arr[i] << std::endl;
		if(arr[i] == window) {
			found	= true;
			index 	= i;
			std::cout << "Found at: " << index << std::endl;
		}
	}
	return index;
}

void Desktop::killCurrentWindow() {
	std::cout << "void Desktop::killCurrentWindow()" << std::endl;
	if(currFocus != -1) {
		std::cout << "TRYING TO KILL" << std::endl;
		Atom DELETE 	= XInternAtom(disp, "WM_DELETE_WINDOW", False);
		Atom PROTO 	= XInternAtom(disp, "WM_PROTOCOLS", True);	
		
		XEvent eKill;
		eKill.xclient.type = ClientMessage;
		eKill.xclient.window = currFocus;
		eKill.xclient.message_type = XInternAtom(disp, "WM_PROTOCOLS", true);
		eKill.xclient.format = 32;
		eKill.xclient.data.l[0] = XInternAtom(disp, "WM_DELETE_WINDOW", false);
		eKill.xclient.data.l[1] = CurrentTime;
		XSendEvent(disp, currFocus, False, NoEventMask, &eKill);

		//XDestroyWindow(disp, currFocus);
	}
}

bool Desktop::removeWindow(Window window) {
	std::cout << "bool Desktop::removeWindow(Window window)" << std::endl;
	std::cout << "removeWindow: " << window << std::endl;
	if(window != 0) {
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
				std::cout << "Found in left column" << std::endl;
				for(int i = index; i < (amountLeft - 1); i++) left[i] = left[i + 1];
				amountLeft--;
			} else {
				std::cout << "Found in right column" << std::endl;
				for(int i = index; i < (amountRight - 1); i++) right[i] = right[i + 1];
				amountRight--;
			}		
					
			std::cout << "amountLeft: " << amountLeft << ", amountRight: " << amountRight << std::endl;
			if(amountLeft <= 0 && amountRight > 0) moveToLeft(right[0]);
			resizeWindows();
		}
	}
}

void Desktop::expandArray(Window*& arr, int amount) {
	std::cout << "void Desktop::expandArray(Window*& arr, int amount)" << std::endl;
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
	std::cout << "void Desktop::resizeWindows()" << std::endl; 
	std::cout << "amountLeft: " << amountLeft << ", amountRight: " << amountRight << std::endl;
	/* Resize windows on the left side */
	if(amountLeft > 0) {
		std::cout << "Enter resize left" << std::endl;
		int wX		= x + wgap;
		int wY		= y + wgap;
		int wWidth 	= (amountRight == 0) ? width - (2 * wgap) : (width / 2) - (2 * wgap); 
		int wHeight	= (height - ((amountLeft + 1) * wgap) - y) / amountLeft;

		for(int i = 0; i < amountLeft; i++) {
			XMoveResizeWindow(disp, left[i], wX, wY, wWidth, wHeight);
			wY = wHeight + wgap + wY;
		}	
	}

	/* Resize windows on the right side */
	if(amountRight > 0) {
		std::cout << "Enter resize right" << std::endl;
		int wX		= (width / 2);
		int wY		= y + wgap;
		int wWidth 	= (width / 2) - wgap; 
		int wHeight	= (height - ((amountRight + 1) * wgap) - y) / amountRight;

		for(int i = 0; i < amountRight; i++) {
			XMoveResizeWindow(disp, right[i], wX, wY, wWidth, wHeight);
			wY = wHeight + wgap + wY;
		}	
	}
}

void Desktop::openProgram(std::string program) {
	std::cout << "void Desktop::openProgram(std::string program)" << std::endl;
	program += " &";
	system(program.c_str());
}

void Desktop::setCurrentFocusedWindow(Window window) {
	std::cout << "Desktop::setCurrentFocusedWindow(Window window)" << std::endl;
	if(currFocus != -1) {
		XSetWindowBorder(disp, currFocus, COL_BORDER);
	}
	
	this->currFocus = window;
	XSetWindowBorder(disp, window, COL_BORDER_SELECTED);
	
	XWindowChanges wChange;
	wChange.border_width = BORDER_WIDTH;
	XConfigureWindow(disp, window, CWBorderWidth, &wChange);
}
