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

static Atom NET_CLIENT_LIST;
static Atom NET_WM_WINDOW_TYPE;
static Atom NET_WM_WINDOW_TYPE_NORMAL;
static Atom NET_WM_STATE;
static Atom NET_SUPPORTED;
static Atom NET_ACTIVE_WINDOW; //= XInternAtom(disp, "_NET_ACTIVE_WINDOW", False);
static Atom NET_WM_NAME; //= XInternAtom(disp, "_NET_WM_NAME", False);
static Atom NET_WM_STATE_FULLSCREEN; //= XInternAtom(disp, "_NET_WM_STATE_FULLSCREEN", False);
static Atom NET_WM_WINDOW_TYPE_DIALOG; //= XInternAtom(disp, "_NET_WM_WINDOW_TYPE_DIALOG", False);

/*
netatom[NetActiveWindow] = XInternAtom(dpy, "_NET_ACTIVE_WINDOW", False);
netatom[NetWMName] = XInternAtom(dpy, "_NET_WM_NAME", False);
netatom[NetWMFullscreen] = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);
netatom[NetWMWindowTypeDialog] = XInternAtom(dpy, "_NET_WM_WINDOW_TYPE_DIALOG", False);
*/

static Atom WM_DELETE_WINDOW;
static Atom WM_PROTOCOLS;
static Atom WM_TAKE_FOCUS;
static Atom WM_STATE;

#endif //_CONSTANTS_
