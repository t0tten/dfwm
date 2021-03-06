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

#ifndef __DFWMWINDOW__
#define __DFWMWINDOW__

#include <X11/Xlib.h>

#include "../include/DfwmStatus.h"
#include "../include/Configuration.h"

class DfwmWindow {
        private:
                Configuration *configuration;
                Display *display;
                //std::string name;
                Window window;
                Window root;
                XWindowAttributes xWindowAttributes;
                XWindowChanges xWindowChanges;


                void setClientState(long state);

        public:
                DfwmWindow();
                ~DfwmWindow();

                DfwmStatus init(Configuration*, Display*, Window, Window);

                Window getWindow();

                int map();
                int unmap();
                int mapSubwindows();

                int updateBorderWidth();

                int getX();
                int getY();
                int getHeight();
                int getWidth();

                DfwmStatus getWindowAttributes();
                DfwmStatus resize(int x, int y, int width, int height);
                DfwmStatus sendConfigureEvent();
};

#endif
