#ifndef _KEY_BINDINGS_
#define _KEY_BINDINGS_

#include <X11/keysym.h>
#include <iostream>
#include <string>

#include "./Dfwm.h"

class Dfwm;
class KeyBindings {
	private:
		void changeDesktop(Dfwm*, bool);
		void changeDesktop(Dfwm*, int);
	public:
		KeyBindings ();
		~KeyBindings ();
		void translate_KeyDown (Dfwm*, XKeyEvent*);
		void translate_KeyUp (Dfwm*, XKeyEvent*);
};

#endif //_KEY_BINDINGS_
