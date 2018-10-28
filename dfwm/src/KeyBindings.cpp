#include "../include/KeyBindings.h" 

#define MOD Mod1Mask
//#define MOD 64
#define STATE 0

KeyBindings::KeyBindings () {}
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

void KeyBindings::openProgram(Dfwm* dfwm) {
	Window wnd = dfwm->getCurrentDesktop()->openProgram("hej", dfwm->getOpened(), dfwm->getNrOfOpen());
	std::string txt = std::to_string(wnd);
	if(wnd != -1) {
		//dfwm->addOpen(wnd);
		txt += " saved!";
	} else {
		txt += " not saved...";
	}

	//dfwm->getStatusBar()->setText(txt);
	//dfwm->getStatusBar()->redraw();
}

void KeyBindings::translate_KeyDown (Dfwm* dfwm, XKeyEvent* keyCode) {
	//int keyState = keyCode->state;
	int key = XLookupKeysym(keyCode, 0) << keyCode->state;
	std::string txt = "Mod: " + std::to_string(MOD) + ", State: " + std::to_string(keyCode->state) + ", KeyCode: " + std::to_string(keyCode->keycode); 
	//std::cout << "MOD: " << MOD << ", State: " << std::to_string(keyCode->state) << ", KeyCode: " << std::to_string(keyCode->keycode) << std::endl; 
	//dfwm->getStatusBar()->setText(txt);
	//dfwm->getStatusBar()->redraw();
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
 	else if (key == (XK_Return << MOD))	openProgram(dfwm);
}

void KeyBindings::translate_KeyUp (Dfwm* dfwm, XKeyEvent* keyCode) {
	switch(XLookupKeysym(keyCode, 0)) {
		case XK_Escape:
			dfwm->quit();
			break;
		case XK_Alt_L:
			dfwm->getMenu()->hide();
			break;
	}
}
