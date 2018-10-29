#include "../include/KeyBindings.h" 
#include "../Config.h"

#define CTRL 4

KeyBindings::KeyBindings (Display* disp){ 
        this->disp = disp; 
}

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

void KeyBindings::setup(Dfwm* dfwm) {
        for (int i=0; i < NUM_HOTKEYS; i++) {
                /* Setup hotkey */
                XGrabKey(
                        dfwm->getDisplay(),
                        XKeysymToKeycode(dfwm->getDisplay(), hotkeys[i].key),
                        hotkeys[i].modifier,
                        dfwm->getRoot(),
                        true,
                        GrabModeAsync,
                        GrabModeAsync);

                /* Setup hotkey for when numlock is active */
                XGrabKey(
                        dfwm->getDisplay(),
                        XKeysymToKeycode(dfwm->getDisplay(), hotkeys[i].key),
                        hotkeys[i].modifier | NUM,
                        dfwm->getRoot(),
                        true,
                        GrabModeAsync,
                        GrabModeAsync);
        }
}

int KeyBindings::getAction(XKeyEvent* keyCode) {
        unsigned int val = XLookupKeysym(keyCode, 0) << (keyCode->state);

        std::cout << "val=" << val << std::endl;
        std::cout << "hotkeys=" << NUM_HOTKEYS << std::endl;

        for (int i=0; i < NUM_HOTKEYS; i++) {
                if ( ((hotkeys[i].key << hotkeys[i].modifier)) == val ||
                     (hotkeys[i].key << (hotkeys[i].modifier | NUM)) == val) {
                        return hotkeys[i].action;
                }
        }
        
        return -1;
}

void KeyBindings::executeAction(Dfwm* dfwm, int action) {
        switch(action) {
                case ACTION_SHELL:
                        dfwm->getCurrentDesktop()->openProgram(DEFAULT_TERMINAL);
                        break;
                case ACTION_MENU:
                        dfwm->getMenu()->show();
                        break;
                case ACTION_DESKTOP_1:
                        changeDesktop(dfwm, 1);
                        break;
                case ACTION_DESKTOP_2:
                        changeDesktop(dfwm, 2);
                        break;
                case ACTION_DESKTOP_3:
                        changeDesktop(dfwm, 3);
                        break;
                case ACTION_DESKTOP_4:
                        changeDesktop(dfwm, 4);
                        break;
                case ACTION_DESKTOP_5:
                        changeDesktop(dfwm, 5);
                        break;
                case ACTION_DESKTOP_6:
                        changeDesktop(dfwm, 6);
                        break;
                case ACTION_DESKTOP_7:
                        changeDesktop(dfwm, 7);
                        break;
                case ACTION_DESKTOP_8:
                        changeDesktop(dfwm, 8);
                        break;
                case ACTION_DESKTOP_9:
                        changeDesktop(dfwm, 9);
                        break;
                case ACTION_DESKTOP_0:
                        changeDesktop(dfwm, 0);
                        break;
                case ACTION_NEXT_DESKTOP:
                        changeDesktop(dfwm, true);
                        break;
                case ACTION_PREV_DESKTOP:
                        changeDesktop(dfwm, false);
                        break;
                case ACTION_QUIT_WM:
                        dfwm->quit();
                        break;
                case ACTION_LAUNCHER:
                        dfwm->getLauncher()->show();
                        break;
                case ACTION_KILL_ACTIVE:
                        std::cout << "Killing window" << std::endl;
                        break;
                case ACTION_DMENU:
                        dfwm->getCurrentDesktop()->openProgram(DMENU);
                        break;
                default:
                        std::cout << "err: unknown action" << std::endl;
                        break;
        }
}

void KeyBindings::translate_KeyDown (Dfwm* dfwm, XKeyEvent* keyCode) {
        int action = this->getAction(keyCode);
        int key = XLookupKeysym(keyCode, 0) << (keyCode->state - 16);

	std::string keyTxt = "KeyCode: " + std::to_string(keyCode->keycode) + ", State: " + std::to_string(keyCode->state);
	dfwm->getStatusBar()->setText(keyTxt);
	dfwm->getStatusBar()->redraw();

	if(dfwm->getLauncher()->getState() == HIDING) {
                this->executeAction(dfwm, action);
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
        if (this->getAction(keyCode)) {
                dfwm->getMenu()->hide();
        }
}
