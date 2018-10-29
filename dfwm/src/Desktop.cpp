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
	for(int i = 0; i < amountLeft; i++)	XDestroyWindow(disp, left[i]);
	for(int i = 0; i < amountRight; i++)	XDestroyWindow(disp, right[i]);
	delete[] left;
	delete[] right;
}

void Desktop::show() {
	for(int i = 0; i < amountLeft; i++) 	XMapWindow(disp, left[i]);
	for(int i = 0; i < amountRight; i++) 	XMapWindow(disp, right[i]);
}

void Desktop::hide() {
	for(int i = 0; i < amountRight; i++) 	XUnmapWindow(disp, right[i]);
	for(int i = 0; i < amountLeft; i++)	XUnmapWindow(disp, left[i]);
}

void Desktop::redraw() {}

void Desktop::moveToLeft(Window window) {
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
	if(amountLeft <= 0) 	addWindow(window, left, amountLeft);
	else  			addWindow(window, right, amountRight);
	resizeWindows();
}

int Desktop::findWindow(Window window, Window* arr, int size) {
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
	if(currFocus != -1) {
		if(removeWindow(currFocus)) XDestroyWindow(disp, currFocus);
		this->currFocus = -1;
	}
}

bool Desktop::removeWindow(Window window) {
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
		if(isLeft) {
			for(int i = index; i < (amountLeft - 1); i++) left[i] = left[i + 1];
			amountLeft--;
		} else {
			for(int i = index; i < (amountRight - 1); i++) right[i] = right[i + 1];
			amountRight--;
		}		
		
		if(amountLeft <= 0 && amountRight > 0) moveToLeft(right[0]);
		resizeWindows();
	}
}

void Desktop::expandArray(Window*& arr, int amount) {
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
	/* Resize windows on the left side */
	if(amountLeft > 0) {
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
	program += " &";
	system(program.c_str());
}

void Desktop::setCurrentFocusedWindow(Window window) {
	if(currFocus != -1) {
		XSetWindowBorder(disp, currFocus, COL_BORDER);
	}
	
	this->currFocus = window;
	XSetWindowBorder(disp, window, COL_BORDER_SELECTED);
	
	XWindowChanges wChange;
	wChange.border_width = BORDER_WIDTH;
	XConfigureWindow(disp, window, CWBorderWidth, &wChange);
}
