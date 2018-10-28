#ifndef _CONSTANTS_
#define _CONSTANTS_

#include "./Color.h"

static long EVENT_MASK			= ExposureMask | KeyPressMask | KeyReleaseMask | ButtonReleaseMask | SubstructureNotifyMask;
static int COL_SELECTED_BG 		= YELLOW;
static int COL_SELECTED_FG 		= BLACK;
static int COL_MENU_BG			= GRAY;
static int COL_MENU_DESKTOP_BG		= GRAY;
static int COL_MENU_DESKTOP_FG		= LGT_GRAY;
static int COL_MENU_SETTINGS_BG		= GRAY;
static int COL_MENU_SETTINGS_FG		= LGT_GRAY;
static int COL_STATUS_BAR_BG		= GRAY;
static int COL_STATUS_BAR_CLK		= WHITE;
static int COL_STATUS_BAR_TITLE		= YELLOW;
static int COL_STATUS_BAR_INFO		= WHITE;
static int COL_STATUS_BAR_DESKTOP_FG	= BLACK;
static int COL_STATUS_BAR_DESKTOP_BG	= YELLOW;
static int COL_BORDER_COLOR		= YELLOW;

static int CIRCLE[2]			= {0, 360 * 64};

enum Alignment {CENTER, LEFT, RIGHT};

#endif //_CONSTANTS_
