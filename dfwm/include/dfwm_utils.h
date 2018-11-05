#ifndef __DFWM_UTILS__
#define __DFWM_UTILS__

#include <X11/Xlib.h>

namespace dfwm {
        
        int sendEvent(Display *display, Window window, Atom proto);
        
        void grabButtons(Display *display, Window window);
        void ungrabButtons(Display *display, Window window);

        void focus(Display *display, Window root, Window window);
        void unfocus(Display *display, Window root, Window window);

}

#endif
