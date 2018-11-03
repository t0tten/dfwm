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

#ifndef __STATUS_CODES__
#define __STATUS_CODES__

#define DFWM_OK                 0
#define DFWM_OK_STR             "OK"
#define DFWM_GENERIC_ERROR      1000
#define DFWM_GENERIC_ERROR_STR  "Generic error"
#define DFWM_UNKNOWN_ERROR      1001 
#define DFWM_UNKOWN_ERROR_STR   "Unknown error"
#define DFWM_NULL_POINTER       1002
#define DFWM_NULL_POINTER_STR   "Null pointer"

/* Errors from X11/X.h */
#define DFWM_BAD_REQUEST        1
#define DFWM_BAD_REQUEST_STR    "Bad request"
#define DFWM_BAD_VALUE          2
#define DFWM_BAD_VALUE_STR      "Bad value"
#define DFWM_BAD_WINDOW         3
#define DFWM_BAD_WINDOW_STR     "Bad window"
#define DFWM_BAD_PIXMAP         4
#define DFWM_BAD_PIXMAP_STR     "Bad pixmap"
#define DFWM_BAD_ATOM           5
#define DFWM_BAD_ATOM_STR       "Bad atom"
#define DFWM_BAD_CURSOR         6
#define DFWM_BAD_CURSOS_STR     "Bad cursor"
#define DFWM_BAD_FONT           7
#define DFWM_BAD_FONT_STR       "Bad font"
#define DFWM_BAD_MATCH          8
#define DFWM_BAD_MATCH_STR      "Bad match"
#define DFWM_BAD_DRAWABLE       9
#define DFWM_BAD_DRAWABLE_STR   "Bad drawable"
#define DFWM_BAD_ACCESS         10
#define DFWM_BAD_ACCESS_STR     "Bad access"
#define DFWM_BAD_ALLOC          11
#define DFWM_BAD_ALLOC_STR      "Bad alloc"
#define DFWM_BAD_COLOR          12
#define DFWM_BAD_COLOR_STR      "Bad color"
#define DFWM_BAD_GC             13
#define DFWM_BAD_GC_STR         "Parameter not a GC"
#define DFWM_BAD_ID_CHOICE      14
#define DFWM_BAD_ID_CHOICE_STR  "Bad ID: choice not in range or alreeady used"
#define DFWM_BAD_NAME           15
#define DFWM_BAD_NAME_STR       "Font or color name does not exist"
#define DFWM_BAD_LENGTH         16
#define DFWM_BAD_LENGTH_STR     "Requst length incorrect"
#define DFWM_BAD_IMPLEMENTATION 17
#define DFWM_BAD_IMPLEMENTATION_STR "Server is defective"

#endif

