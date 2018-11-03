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

#include "../include/DfwmStatus.h"
#include "../include/logger.h"
#include "../include/status_codes.h"

DfwmStatus::DfwmStatus(const char* file, const char* function,
                int line, unsigned int code) {
        this->code = code;
        this->file = file;
        this->function = function;
        this->line = line;

        if(!this->isOk()) {
                this->logError();
        }
}

DfwmStatus::~DfwmStatus() {
}

const char *DfwmStatus::getErrorMessage() {
        switch(code) {
                case DFWM_OK:
                        return DFWM_OK_STR;
                case DFWM_GENERIC_ERROR:
                        return DFWM_GENERIC_ERROR_STR;
                case DFWM_NULL_POINTER:
                        return DFWM_NULL_POINTER_STR;

                /* Xorg Errors */
                case DFWM_BAD_REQUEST:
                        return DFWM_BAD_REQUEST_STR;
                case DFWM_BAD_VALUE:
                        return DFWM_BAD_VALUE_STR;
                case DFWM_BAD_WINDOW:
                        return DFWM_BAD_WINDOW_STR;
                case DFWM_BAD_PIXMAP:
                        return DFWM_BAD_PIXMAP_STR;
                case DFWM_BAD_ATOM:
                        return DFWM_BAD_ATOM_STR;
                case DFWM_BAD_CURSOR:;
                        return DFWM_BAD_CURSOS_STR;
                case DFWM_BAD_FONT:
                        return DFWM_BAD_FONT_STR;
                case DFWM_BAD_MATCH:
                        return DFWM_BAD_MATCH_STR;     
                case DFWM_BAD_DRAWABLE:
                        return DFWM_BAD_DRAWABLE_STR;
                case DFWM_BAD_ACCESS:
                        return DFWM_BAD_ACCESS_STR;
                case DFWM_BAD_ALLOC:
                        return DFWM_BAD_ALLOC_STR;
                case DFWM_BAD_COLOR:
                        return DFWM_BAD_COLOR_STR;
                case DFWM_BAD_GC:
                        return DFWM_BAD_GC_STR;
                case DFWM_BAD_ID_CHOICE:
                        return DFWM_BAD_ID_CHOICE_STR;
                case DFWM_BAD_NAME:
                        return DFWM_BAD_NAME_STR;
                case DFWM_BAD_LENGTH:
                        return DFWM_BAD_LENGTH_STR;
                case DFWM_BAD_IMPLEMENTATION:
                        return DFWM_BAD_IMPLEMENTATION_STR;
                 /* End Xorg Errors */

                default:
                        return DFWM_UNKOWN_ERROR_STR;
        }
}

bool DfwmStatus::isOk() {
        return this->code == DFWM_OK;
}

void DfwmStatus::logError() {
        LOGGER_ERRF("<%s:%s:%d> %s", file, function, line, 
                        this->getErrorMessage());
}

