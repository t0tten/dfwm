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
#define DFWM_GENERIC_ERROR      1
#define DFWM_GENERIC_ERROR_STR  "Generic error"
#define DFWM_UNKNOWN_ERROR      2
#define DFWM_UNKOWN_ERROR_STR   "Unknown error"
#define DFWM_NULL_POINTER       3
#define DFWM_NULL_POINTER_STR   "Null pointer"

#define DFWM_BAD_WINDOW         100
#define DFWM_BAD_WINDOW_STR     "Bad window"
#define DFWM_BAD_DRAWABLE       101
#define DFWM_BAD_DRAWABLE_STR   "Bad drawable"
#define DFWM_BAD_MATCH          102
#define DFWM_BAD_MATCH_STR      "Bad match"
#define DFWM_BAD_VALUE          103
#define DFWM_BAD_VALUE_STR      "Bad value"

#endif

