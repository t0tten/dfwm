#ifndef _KEY_BINDINGS_
#define _KEY_BINDINGS_

#include <X11/keysym.h>
#include <iostream>
#include <string>

#include "./Dfwm.h"

class Dfwm;
class KeyBindings {
	private:
		Display* disp;
		void changeDesktop(Dfwm*, bool);
		void changeDesktop(Dfwm*, int);

                int getAction(XKeyEvent* keyCode);
                void executeAction(Dfwm* dfwm, int action);
	public:
		KeyBindings (Display*);
		~KeyBindings ();
		void translate_KeyDown (Dfwm*, XKeyEvent*);
		void translate_KeyUp (Dfwm*, XKeyEvent*);

                void setup(Dfwm* dfwm);
};

#endif //_KEY_BINDINGS_
