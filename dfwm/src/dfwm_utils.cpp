/* 
 * MIT/X Consortium License
 *
 * © 2018 Robin Karlsson <robin dot karlsson at prontmail dot com>
 * © 2006-2014 Anselm R Garbe <anselm@garbe.us>
 * © 2010-2014 Hiltjo Posthuma <hiltjo@codemadness.org>
 * © 2007-2011 Peter Hartlich <sgkkr at hartlich dot com>
 * © 2010-2011 Connor Lane Smith <cls@lubutu.com>
 * © 2006-2009 Jukka Salmi <jukka at salmi dot ch>
 * © 2007-2009 Premysl Hruby <dfenze at gmail dot com>
 * © 2007-2009 Szabolcs Nagy <nszabolcs at gmail dot com>
 * © 2007-2009 Christof Musik <christof at sendfax dot de>
 * © 2009 Mate Nagy <mnagy at port70 dot net>
 * © 2007-2008 Enno Gottox Boland <gottox at s01 dot de>
 * © 2008 Martin Hurton <martin dot hurton at gmail dot com>
 * © 2008 Neale Pickett <neale dot woozle dot org>
 * © 2006-2007 Sander van Dijk <a dot h dot vandijk at gmail dot com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

/*
 * The code is this file is based on DWM https://dwm.suckless.org/
 */

#include <X11/Xatom.h>
#include <X11/Xlib.h>

#include "../include/dfwm_utils.h"
#include "../include/Constants.h"

int dfwm::sendEvent(Display *display, Window window, Atom proto) {
        int n;
        Atom *protocols;
        int exists = 0;
        XEvent ev;

        if(XGetWMProtocols(display, window, &protocols, &n)) {
                while(!exists && n--) {
                        exists = protocols[n] == proto;
                }
                XFree(protocols);
        }

        if(exists) {
                ev.type = ClientMessage;
		ev.xclient.window = window;
		ev.xclient.message_type = WM_PROTOCOLS;
		ev.xclient.format = 32;
		ev.xclient.data.l[0] = proto;
		ev.xclient.data.l[1] = CurrentTime;
		XSendEvent(display, window, False, NoEventMask, &ev);
        }

        return exists;
}

void dfwm::grabButtons(Display *display, Window window) {
        XGrabButton(display, AnyButton, AnyModifier, window,
                        False, BUTTONMASK, GrabModeAsync, GrabModeSync,
                        None, None);
}

void dfwm::ungrabButtons(Display *display, Window window) {
        XUngrabButton(display, AnyButton, AnyModifier, window);
}

void dfwm::focus(Display *display, Window root, Window window) {
        ungrabButtons(display, window);

        XSetInputFocus(display, window, RevertToPointerRoot, CurrentTime);
        XChangeProperty(display, root, NET_ACTIVE_WINDOW, XA_WINDOW, 32,
                        PropModeReplace, (unsigned char*) &(window), 1);
        sendEvent(display, window, WM_TAKE_FOCUS);
}

void dfwm::unfocus(Display *display, Window root, Window window) {
        grabButtons(display, window);
        XSetInputFocus(display, root, RevertToPointerRoot, CurrentTime);
        XDeleteProperty(display, root, NET_ACTIVE_WINDOW);
}

