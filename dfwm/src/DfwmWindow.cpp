/*  MIT License
 *
 *  Copyright (c) 2018 Rasmus Schenström
 *  Copyright (c) 2018 Robin Karlsson <robin.karlsson(at)protonmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a 
 *  copy of this software and associated documentation files (the "Software"), 
 *  to deal in the Software without restriction, including without limitation 
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 *  and/or sell copies of the Software, and to permit persons to whom the 
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in 
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 *  DEALINGS IN THE SOFTWARE.
 */

#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include "../include/DfwmWindow.h"
#include "../include/logger.h"
#include "../include/status_codes.h"
#include "../include/DfwmStatus.h"
#include "../include/Constants.h"

DfwmWindow::DfwmWindow() {
}

DfwmWindow::~DfwmWindow() {
}

DfwmStatus DfwmWindow::init(Configuration* configuration, 
                Display* display, Window window, Window root) {
        int ret;

        if(configuration == NULL || display == NULL) {
                return DFWM_STATUS(DFWM_NULL_POINTER);
        }

        this->configuration = configuration;
        this->display = display;
        this->window = window;
        this->root = root;

        if(this->root == this->window) {
                LOGGER_DEBUG("Window is root");
                return DFWM_STATUS(DFWM_GENERIC_ERROR);
        }

        DfwmStatus status = this->getWindowAttributes();
        if(!status.isOk()) {
                return status;
        }

        /* Check if the window is a popup */
        if(this->xWindowAttributes.override_redirect) {
                return DFWM_STATUS(DFWM_GENERIC_ERROR);
        }

        /*if((ret = this->updateBorderWidth()) != DFWM_OK) {
                return DFWM_STATUS(ret);
        }*/

        status = this->sendConfigureEvent();
        if(!status.isOk()) {
                return status;
        }

        XRaiseWindow(this->display, this->window);

        XChangeProperty(this->display, this->root, XInternAtom(this->display, "_NET_CLIENT_LIST", False),
                        XA_WINDOW, 32, PropModeAppend, (unsigned char*) 
                        &(this->window), 1);

        status = this->resize(this->xWindowAttributes.x, this->xWindowAttributes.y,
                        this->xWindowAttributes.width, this->xWindowAttributes.height);
        if(!status.isOk()) {
                //return status;
        }

        this->setClientState(NormalState);
        this->map();

        /*XGrabButton(this->display, AnyButton, AnyModifier, this->window,
                        False, BUTTONMASK, GrabModeAsync, GrabModeSync,
                        None, None);
                        */

        return DFWM_STATUS(DFWM_OK);
}

void DfwmWindow::setClientState(long state) {
        long data[] = { state, None };

        XChangeProperty(this->display, this->window, 
                        XInternAtom(this->display, "WM_STATE", False),
                        XInternAtom(this->display, "WM_STATE", False),
                        32, PropModeReplace, (unsigned char*)data, 2);
}

DfwmStatus DfwmWindow::getWindowAttributes() {
        if(!XGetWindowAttributes(this->display, this->window, 
                                &this->xWindowAttributes)) {
                return DFWM_STATUS(DFWM_GENERIC_ERROR);
        }
        return DFWM_STATUS(DFWM_OK);
}

DfwmStatus DfwmWindow::resize(int x, int y, int width, int height) {
        int ret;
        
        if((ret = XMoveResizeWindow(this->display, this->window, x,
                                        y, width, height)
                                != 0)) {
                return DFWM_STATUS(ret);
        }

        return this->getWindowAttributes();
}

Window DfwmWindow::getWindow() {
        return this->window;
}


int DfwmWindow::map() {
        return XMapWindow(this->display, this->window);
}

int DfwmWindow::unmap() {
        return XUnmapWindow(this->display, this->window);
}

int DfwmWindow::mapSubwindows() {
        return XMapSubwindows(this->display, this->window);
}

int DfwmWindow::getX() {
        return this->xWindowAttributes.x;
}

int DfwmWindow::getY() {
        return this->xWindowAttributes.y;
}

int DfwmWindow::getHeight() {
        return this->xWindowAttributes.height;
}

int DfwmWindow::getWidth() {
        return this->xWindowAttributes.width;
}

int DfwmWindow::updateBorderWidth() {
        int ret;
        this->xWindowAttributes.border_width = this->configuration->
                getBorderWidth();

        if( !(ret = XConfigureWindow(this->display, this->window, CWBorderWidth,
                        &this->xWindowChanges))) {
                switch(ret) {
                        case BadMatch:
                                return DFWM_BAD_MATCH;
                        case BadValue:
                                return DFWM_BAD_MATCH;
                        case BadWindow:
                                return DFWM_BAD_WINDOW;
                        default:
                                return DFWM_GENERIC_ERROR;
                }
        }

        if( !(ret = XSetWindowBorder(this->display, this->window,
                                        this->configuration->getBorderColor()))) {
                switch(ret) {
                        case BadMatch:
                                return DFWM_BAD_MATCH;
                        case BadValue:
                                return DFWM_BAD_MATCH;
                        case BadWindow:
                                return DFWM_BAD_WINDOW;
                        default:
                                return DFWM_GENERIC_ERROR;
                }
        }

        return DFWM_OK;
}

DfwmStatus DfwmWindow::sendConfigureEvent() {
        XConfigureEvent ce;
        
        ce.type = ConfigureNotify;
        ce.display = this->display;
        ce.event = this->window;
        ce.window = this->window;
        ce.x = this->xWindowAttributes.x;
        ce.y = this->xWindowAttributes.y;
        ce.width = this->xWindowAttributes.width;
        ce.height = this->xWindowAttributes.height;
        ce.border_width = this->xWindowAttributes.border_width;
        ce.above = None;
        ce.override_redirect = False;
        
        if(XSendEvent(this->display, this->window, False, StructureNotifyMask, 
                                (XEvent *) &ce) == 0) {
                DFWM_STATUS(DFWM_GENERIC_ERROR);
        }

        return DFWM_STATUS(DFWM_OK);
}
