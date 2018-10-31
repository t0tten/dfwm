#include "../include/Menu.h"
#include "../include/logger.h"

Menu::Menu (Display* disp, Window* parent, int diameter, int* selected, int maxDesktops) {
	this->disp	= disp;
	this->selected 	= selected;
	this->diameter 	= diameter;
	this->dd 	= 40;
	this->indD	= 7;
	this->nrOfDesktops = maxDesktops;

	XWindowAttributes wndAttr;
	XGetWindowAttributes (disp, *parent, &wndAttr);

	this->pWidth	= wndAttr.width;
	this->pHeight	= wndAttr.height;
	
        LOGGER_INFOF("%d, %d", pWidth, pHeight);

	this->changeAlignment(MENU_POSITION);
	this->menu	= XCreateSimpleWindow(disp, *parent, x, y - (dd / 2), diameter + dd, diameter + dd, 0, 0, 0);
	this->gc	= XCreateGC(disp, menu, 0, NULL);

	XSelectInput(disp, menu, ExposureMask);
}

Menu::~Menu () {
	XDestroyWindow(disp, menu);
}

void Menu::changeAlignment(int alignment) {
	switch(alignment) {
		case CENTER:
			this->x = (pWidth / 2) - (diameter / 2) - (dd / 2); 
			break;
		case LEFT:
			this->x = dd;
			break;
		case RIGHT:
			this->x = pWidth - diameter - (dd * 2);
			break;
	}
	this->y = (pHeight / 2) - (diameter / 2) - (dd / 2);
}

void Menu::show() {
	XMapWindow(disp, menu);
	XRaiseWindow(disp, menu);
}

void Menu::hide() {
	XUnmapWindow(disp, menu);
}

void Menu::draw(bool* desktopHaveWindow) {
	XSetForeground(disp, gc, COL_MENU_BG);
	XFillArc(disp, menu, gc, dd / 2, dd / 2, diameter, diameter, CIRCLE[0], CIRCLE[1]);

	XSetForeground(disp, gc, COL_MENU_SETTINGS_FG);
	XDrawString(disp, menu, gc, (diameter + dd) / 2 - (2 * 5), (diameter + dd) / 2, "DFWM", 4);

	/* Draw desktops */
	int r = diameter / 2;
	float degreePerItr = (360 / nrOfDesktops) * 3.14159 / 180;

	for(int i = 0; i < nrOfDesktops; i++) {
		float angle = i * degreePerItr;
		int tX = (r * cos(angle)) + r;
		int tY = (r * sin(angle)) + r;
	
		int dColor = (*selected == (i + 1)) ? COL_SELECTED_BG : COL_MENU_DESKTOP_BG;
		XSetForeground(disp, gc, dColor);
		XFillArc(disp, menu, gc, tX, tY, dd, dd, CIRCLE[0], CIRCLE[1]);

		dColor = (*selected == (i + 1)) ? COL_SELECTED_FG : COL_MENU_DESKTOP_FG;
		XSetForeground(disp, gc, dColor);
		XDrawString(disp, menu, gc, tX + (dd/2) - 2, tY + (dd/2) + 5, std::to_string(i + 1).c_str(), std::to_string(i + 1).length());

		/* Draw gotWindow indication */
		if(desktopHaveWindow[i]) 	XSetForeground(disp, gc, COL_MENU_DESKTOP_IND_POP);
		else 				XSetForeground(disp, gc, COL_MENU_DESKTOP_IND_EMPTY);
		tX = ((r - (dd - indD)) * cos(angle)) + r + ((dd/2) - (indD/2));
		tY = ((r - (dd - indD)) * sin(angle)) + r + ((dd/2) - (indD/2));
		XFillArc(disp, menu, gc, tX, tY, indD, indD, CIRCLE[0], CIRCLE[1]);
	}
}

void Menu::redraw() {
	XEvent exp;
	exp.type 		= Expose;
	exp.xexpose.window 	= menu;
	XSendEvent(disp, menu, False, ExposureMask, &exp);
}

int Menu::getWindowID() { return menu; }
