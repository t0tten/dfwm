#include "../include/KeyBindings.h" 

#define MOD Mod1Mask
#define CTRL 4

KeyBindings::KeyBindings (Display* disp) { this->disp = disp; }
KeyBindings::~KeyBindings () {}

void KeyBindings::changeDesktop(Dfwm* dfwm, bool increment) {
	if(increment) 	dfwm->incrementSelected();	
	else		dfwm->decrementSelected();
	dfwm->getMenu()->redraw();
	dfwm->getStatusBar()->redraw();
}

void KeyBindings::changeDesktop(Dfwm* dfwm, int desktop) {
	dfwm->setSelected(desktop);
	dfwm->getMenu()->redraw();
	dfwm->getStatusBar()->redraw();
}

void KeyBindings::translate_KeyDown (Dfwm* dfwm, XKeyEvent* keyCode) {
	int key = XLookupKeysym(keyCode, 0) << (keyCode->state - 16);

	std::string keyTxt = "KeyCode: " + std::to_string(keyCode->keycode) + ", State: " + std::to_string(keyCode->state);
	//dfwm->getStatusBar()->setText(keyTxt);
	//dfwm->getStatusBar()->redraw();

	if(dfwm->getLauncher()->getState() == HIDING) {
		if (key == XK_Escape) 			dfwm->quit();
		else if (key == XK_Alt_L) 		dfwm->getMenu()->show();
		else if (key == (XK_1 << MOD)) 		changeDesktop(dfwm, 1); 
		else if (key == (XK_2 << MOD)) 		changeDesktop(dfwm, 2);
		else if (key == (XK_3 << MOD))		changeDesktop(dfwm, 3);
	 	else if (key == (XK_4 << MOD))		changeDesktop(dfwm, 4);
		else if (key == (XK_5 << MOD))		changeDesktop(dfwm, 5);
	 	else if (key == (XK_6 << MOD))		changeDesktop(dfwm, 6);
	 	else if (key == (XK_7 << MOD))		changeDesktop(dfwm, 7);
	 	else if (key == (XK_8 << MOD))		changeDesktop(dfwm, 8);
	 	else if (key == (XK_9 << MOD))		changeDesktop(dfwm, 9);
	 	else if (key == (XK_0 << MOD))		changeDesktop(dfwm, 10);
	 	else if (key == (XK_Up << MOD))		changeDesktop(dfwm, true);	
	 	else if (key == (XK_Right << MOD))	changeDesktop(dfwm, true);
	 	else if (key == (XK_Down << MOD))	changeDesktop(dfwm, false);
	 	else if (key == (XK_Left << MOD))	changeDesktop(dfwm, false);
	 	else if (key == (XK_Return << MOD))	dfwm->getLauncher()->show();
	 	else if (key == (XK_q << MOD))		dfwm->getCurrentDesktop()->killCurrentWindow();
	 	else if (key == (XK_t << (MOD + CTRL)))	dfwm->getCurrentDesktop()->openProgram("/usr/bin/mate-terminal");

	} else if(dfwm->getLauncher()->getState() == SHOWING) {
		if(key == XK_Escape)		dfwm->getLauncher()->hide();
		else if(key == XK_Return) {
			dfwm->getCurrentDesktop()->openProgram(dfwm->getLauncher()->getText());
			dfwm->getLauncher()->hide();
		} else if(key == XK_BackSpace) {
			dfwm->getLauncher()->removeLastChar();
			dfwm->getLauncher()->draw();
			dfwm->getLauncher()->redraw();
		} else {
			dfwm->getLauncher()->addChar(XKeysymToString(key));
			dfwm->getLauncher()->draw();
			dfwm->getLauncher()->redraw();
		}
	}
}

void KeyBindings::translate_KeyUp (Dfwm* dfwm, XKeyEvent* keyCode) {
	switch(XLookupKeysym(keyCode, 0)) {
		case XK_Alt_L:
			dfwm->getMenu()->hide();
			break;
	}
}
