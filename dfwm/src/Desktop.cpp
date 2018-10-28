#include "../include/Desktop.h"

Desktop::Desktop (Display* disp, Window* parent, int x, int y, int width, int height, int wgap, int border, int borderColor) {
	this->disp		= disp;
	this->x			= x;
	this->y			= y;
	this->width		= width;
	this->height		= height;
	this->root		= parent;

	//std::cout << "Height of screen: " << height << std::endl;
	this->wgap		= wgap;
	this->border		= border;
	this->borderColor	= borderColor;

	this->size		= 5;
	this->amountLeft	= 0;
	this->amountRight	= 0;

	this->left		= new Window[size];	
	this->right		= new Window[size];
}

Desktop::~Desktop () {
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
	for(int i = 0; i < amountLeft; i++) {
		XMapWindow(disp, left[i]);
	}
	for(int i = 0; i < amountRight; i++) {
		XMapWindow(disp, right[i]);
	}
}

void Desktop::hide() {
	for(int i = 0; i < amountRight; i++) {
		XUnmapWindow(disp, right[i]);
	}
	for(int i = 0; i < amountLeft; i++) {
		XUnmapWindow(disp, left[i]);
	}
}

void Desktop::redraw() {}

void Desktop::moveToLeft(Window window) {
	int index = findWindow(window, right, amountRight);
	/* If the window is found */
	if(index != -1) {
		//std::cout << "Left amount: " << amountLeft << std::endl;
		addWindow(window, left, amountLeft);
		//std::cout << "Index: " << index << ", ArraySize: " << size << ", AmountLeft: " << amountLeft << std::endl;
		if(amountRight > 1) {
			for(int i = index; i < (amountRight - 1); i++) {
				right[i] = right[i + 1];
			}
		}
		amountRight--;
		//std::cout << "Right amount: " << amountRight << std::endl;
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
	
	XSetWindowAttributes wndAttr;
	wndAttr.border_pixel = border;
	XChangeWindowAttributes(disp, window, CWBorderPixel, &wndAttr);

	XSetWindowBorder(disp, window, borderColor);
	
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
		if(arr[i] == window) {
			found	= true;
			index 	= i;
		}
	}
	return index;
}

void Desktop::removeWindow(Window window) {
	bool found 		= false;
	int index 		= -1;
	int *arrSize		= NULL;
	Window* foundArr 	= NULL;

	/* Search for window on the left side */
	index = findWindow(window, left, amountLeft);
	if(index != -1) {
		found 		= true;
		foundArr 	= left;
		arrSize 	= &amountLeft;
	}

	/* Search for window on the right side, if not found on the left side */
	if(!found) {
		index = findWindow(window, right, amountRight);
		if(index != -1) {
			found 		= true;
			foundArr 	= right;
			arrSize 	= &amountRight;
		}
	}

	/* If the window is found, remove it and shrink the according array */
	if(foundArr != NULL) {
		XDestroyWindow(disp, foundArr[index]);
		for (int i = index; i < (*arrSize - 1); i++) {
			foundArr[i] = foundArr[i + 1]; 
		}
		*arrSize--;

		resizeWindows();
	}
}

void Desktop::expandArray(Window*& arr, int amount) {
	if(size <= amount) {
		//std::cout << "Amount: " << amount << std::endl;
		size *= 2;
		Window* tmp = new Window[size];

		for(int i = 0; i < amount; i++) {
			tmp[i] = arr[i];
		}

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

Window Desktop::findAllWindows(Window* known, int amount) {
	unsigned int nrOfWindows;
        Window tmp, tmp1;
        Window* windows;
        XQueryTree(disp, *root, &tmp, &tmp1, &windows, &nrOfWindows);

        for(int i = 0; i < nrOfWindows; i++) {
                XWindowAttributes wndAttr;
                XGetWindowAttributes(disp, windows[i], &wndAttr);
                if(wndAttr.map_state == IsViewable) {
			bool found = false;
			for(int j = 0; j < amount && !found; j++) {
                        	if(windows[i] == known[j]) {
					found = true;
					break;
				}
			}
			if(!found) return windows[i];
                }
        }

	return -1;
}


Window Desktop::openProgram(std::string program, Window* known, int amount) {
	if(system("/usr/bin/mate-terminal &") != -1) {
		std::cout << "Opened!" <<  std::endl;		
		Window newWnd = findAllWindows(known, amount);
		if(newWnd != -1) addWindow(newWnd);
		resizeWindows();
		return newWnd;
	}

	return -1;
}
/* https://www.linuxquestions.org/questions/programming-9/getting-the-pid-of-the-top-active-window-776938/ */
Window* Desktop::get_win_list_stacked(unsigned long *len){
    Atom type;
    int form;
    unsigned long remain;
    unsigned char *list;

//NET_ACTIVE_WINDOW = disp.intern_atom('_NET_ACTIVE_WINDOW')
//NET_WM_NAME = disp.intern_atom('_NET_WM_NAME')  # UTF-8
//WM_NAME = disp.intern_atom('WM_NAME') 

    X_NET_CLIENT_LIST_STACKING = disp.intern_atom('_X_NET_CLIENT_LIST_STACKING');

    errno = 0;
    if(XGetWindowProperty(disp, root, X_NET_CLIENT_LIST_STACKING, 0, 1024, False, XA_WINDOW, &type, &form, len, &remain, &list) != Success)
    {
        return 0;
    }

    return (Window*)list;
}

int Desktop::minimized_window(Window win)
{
    Atom type;
    int form;
    unsigned long i, len, remain;
    Atom *atoms;

    atoms = NULL;

    XGetWindowProperty(disp, win, X_NET_WM_STATE, 0, 1024, False, XA_ATOM, &type, &form, &len, &remain, (unsigned char**)&atoms);

    for(i=0; i<len; ++i) {
        if(atoms[i]==X_NET_WM_STATE_HIDDEN) {
            XFree(atoms);
            return 1;
        }
    }
    XFree(atoms);
    return 0;
}

int Desktop::is_skip_taskbar_or_pager_set(Window win)
{
    Atom type;
    int form;
    unsigned long i, len, remain;
    Atom *atoms;

    atoms = NULL;

    XGetWindowProperty(disp, win, X_NET_WM_STATE, 0, 1024, False, XA_ATOM, &type, &form, &len, &remain, (unsigned char**)&atoms);

    for(i=0; i<len; ++i)
    {
        if((atoms[i]==X_NET_WM_STATE_SKIP_TASKBAR) || (atoms[i]==X_NET_WM_STATE_SKIP_PAGER))
        {
            XFree(atoms);
            return 1;
        }
    }
    XFree(atoms);
    return 0;
}

Window Desktop::get_top_window_from_stack()
{
    int i;
    unsigned long len;
    Window *list;
    Window ret;

    ret = 0;

    list = (Window*) get_win_list_stacked(&len);

    for (i = 0; i < (int) len; i++) {
        if (!is_skip_taskbar_or_pager_set((Window) list[i]) && !minimized_window((Window) list[i]))
            ret = list[i];
    }

    XFree(list);

    return ret;
}















