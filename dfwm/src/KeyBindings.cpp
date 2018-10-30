#include "../include/KeyBindings.h" 
#include "../include/logger.h"
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

void KeyBindings::setupGrabKey(Dfwm* dfwm, unsigned int key, unsigned int modifier) {
        XGrabKey(
                dfwm->getDisplay(),
                XKeysymToKeycode(dfwm->getDisplay(), key),
                modifier,
                dfwm->getRoot(),
                true,
                GrabModeAsync,
                GrabModeAsync);
}

void KeyBindings::setup(Dfwm* dfwm) {
        for (int i=0; i < NUM_HOTKEYS; i++) {
                this->setupGrabKey(dfwm, hotkeys[i].key, hotkeys[i].modifier);

                this->setupGrabKey(dfwm, hotkeys[i].key, hotkeys[i].modifier 
                                | Mod2Mask);
                this->setupGrabKey(dfwm, hotkeys[i].key, hotkeys[i].modifier 
                                | LockMask);
                this->setupGrabKey(dfwm, hotkeys[i].key, hotkeys[i].modifier 
                                | Mod3Mask);

                this->setupGrabKey(dfwm, hotkeys[i].key, hotkeys[i].modifier 
                                | Mod2Mask | LockMask);
                this->setupGrabKey(dfwm, hotkeys[i].key, hotkeys[i].modifier 
                                | Mod2Mask | Mod3Mask);
                this->setupGrabKey(dfwm, hotkeys[i].key, hotkeys[i].modifier 
                                | LockMask | Mod3Mask);

                this->setupGrabKey(dfwm, hotkeys[i].key, hotkeys[i].modifier 
                                | Mod2Mask | Mod3Mask | LockMask);
        }
}

/* Unset the CapsLock, NumLock, SrollLock modifiers */
int KeyBindings::unmask(int n) {
        n &= ~(1UL << 1); // CapsLock
        n &= ~(1UL << 4); // NumLock
        n &= ~(1UL << 5); // ScrollLock
        return n;
}

int KeyBindings::getAction(XKeyEvent* keyCode) {
        unsigned int val = XLookupKeysym(keyCode, 0) << unmask(keyCode->state);

        for (int i=0; i < NUM_HOTKEYS; i++) {
                if ( ((hotkeys[i].key << hotkeys[i].modifier)) == val) {
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
                        LOGGER_INFO("Killing window");
			dfwm->getCurrentDesktop()->killCurrentWindow();
                        break;
                default:
                        LOGGER_INFO("err: unknown action");
                        break;
        }
}

void KeyBindings::translate_KeyDown (Dfwm* dfwm, XKeyEvent* keyCode) {
        int action = this->getAction(keyCode);

        int key = XLookupKeysym(keyCode, 0);

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
		} else if(key == XK_Tab) {
			dfwm->getLauncher()->autoComplete();
			dfwm->getLauncher()->draw();
			dfwm->getLauncher()->redraw();
		} else {
                        char text[32] = {};
                        XComposeStatus status;
                        KeySym keysym = NoSymbol;

                        keyCode->state &= ~ControlMask;

                        XLookupString(keyCode, text, sizeof(text)-1, &keysym, &status);
                        
                        if(text[0] >= 0x20 && text[0] <= 0x7e) {
				dfwm->getLauncher()->addChar(text[0]);
				dfwm->getLauncher()->draw();
				dfwm->getLauncher()->redraw();
                       }
		}
	}
}

void KeyBindings::translate_KeyUp (Dfwm* dfwm, XKeyEvent* keyCode) {
        if (this->getAction(keyCode)) {
                dfwm->getMenu()->hide();
        }
}
