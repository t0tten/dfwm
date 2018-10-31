#ifndef _CONSTANTS_
#define _CONSTANTS_

#include "./Color.h"

enum Alignment {CENTER, LEFT, RIGHT};
enum LauncherState {SHOWING, HIDING};
static long EVENT_MASK			= ExposureMask | KeyPressMask | KeyReleaseMask | ButtonReleaseMask | SubstructureNotifyMask | FocusChangeMask | EnterWindowMask | PointerMotionMask;
static long ROOT_EVENT_MASK		= SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask | StructureNotifyMask | PropertyChangeMask| FocusChangeMask;
static int COL_SELECTED_BG 		= YELLOW;
static int COL_SELECTED_FG 		= BLACK;
static int COL_MENU_BG			= GRAY;
static int WINDOW_GAP			= 10;
static int COL_MENU_DESKTOP_BG		= GRAY;
static int COL_MENU_DESKTOP_FG		= LGT_GRAY;
static int COL_MENU_DESKTOP_IND_POP	= GREEN;
static int COL_MENU_DESKTOP_IND_EMPTY	= MEDIUM_GRAY2;
static int COL_MENU_SETTINGS_BG		= GRAY;
static int COL_MENU_SETTINGS_FG		= LGT_GRAY;
static int COL_STATUS_BAR_BG		= GRAY;
static int COL_STATUS_BAR_CLOCK		= WHITE;
static int COL_STATUS_BAR_TITLE		= YELLOW;
static int COL_STATUS_BAR_INFO		= WHITE;
static int COL_STATUS_BAR_DESKTOP_FG	= BLACK;
static int COL_STATUS_BAR_DESKTOP_BG	= YELLOW;
static int COL_BORDER_SELECTED		= YELLOW;
static int COL_BORDER			= LGT_GRAY;
static int COL_LAUNCHER_BG		= GRAY;
static int COL_LAUNCHER_FG		= LGT_GRAY;
static int COL_LAUNCHER_RESULT_BG	= MEDIUM_GRAY2;
static int COL_LAUNCHER_RESULT_SEL_BG	= MEDIUM_GRAY;
static int COL_LAUNCHER_RESULT_FG	= YELLOW; 
static int BORDER_WIDTH			= 1;
static int MENU_POSITION		= CENTER;
static int CIRCLE[2]			= {0, 360 * 64};

#endif //_CONSTANTS_
