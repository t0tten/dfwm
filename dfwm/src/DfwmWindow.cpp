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

#include "../include/DfwmWindow.h"
#include "../include/logger.h"
#include "../include/status_codes.h"
#include "../include/DfwmStatus.h"

DfwmWindow::DfwmWindow() {
}

DfwmWindow::~DfwmWindow() {
}

DfwmStatus DfwmWindow::init(Configuration* configuration, 
                Display* display, Window* window) {
        int ret;

        if(configuration == NULL || display == NULL || window == NULL) {
                return DFWM_STATUS(DFWM_NULL_POINTER);
        }

        this->configuration = configuration;
        this->display = display;
        this->window = window;


        if((ret = XGetWindowAttributes(this->display, *this->window, 
                                &this->xWindowAttributes)) == 0) {
                LOGGER_ERR("Failed to XGetWindowAttributes");
                switch(ret) {
                        case BadDrawable:
                                return DFWM_STATUS(DFWM_BAD_DRAWABLE);
                        case BadWindow:
                                return DFWM_STATUS(DFWM_BAD_WINDOW);
                        default:
                                return DFWM_STATUS(DFWM_GENERIC_ERROR);
                }
        }

        if((ret = this->updateBorderWidth()) != DFWM_OK) {
                return DFWM_STATUS(ret);
        }

        if(XSelectInput(this->display, *this->window, EnterWindowMask | 
                                FocusChangeMask | PropertyChangeMask |
                                StructureNotifyMask) == BadWindow) {
                LOGGER_ERR("XSelectInput: Bad Window");
                return DFWM_STATUS(DFWM_BAD_WINDOW);
        }

        if(this->map() == BadWindow) {
                LOGGER_ERR("map: Bad Window");
                return DFWM_STATUS(DFWM_BAD_WINDOW);
        }

        return DFWM_STATUS(DFWM_OK);
}

Window* DfwmWindow::getWindow() {
        return this->window;
}


int DfwmWindow::map() {
        return XMapWindow(this->display, *this->window);
}

int DfwmWindow::unmap() {
        return XUnmapWindow(this->display, *this->window);
}

int DfwmWindow::mapSubwindows() {
        return XMapSubwindows(this->display, *this->window);
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

        if( !(ret = XConfigureWindow(this->display, *this->window, CWBorderWidth,
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

        if( !(ret = XSetWindowBorder(this->display, *this->window,
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
