#ifndef _CONSTANTS_
#define _CONSTANTS_

#include "./Color.h"

#define BUTTONMASK              (ButtonPressMask|ButtonReleaseMask)
#define WINDOWS_EVENT_MASK	(EnterWindowMask|FocusChangeMask|PropertyChangeMask|StructureNotifyMask)
#define ROOT_EVENT_MASK		(SubstructureRedirectMask|SubstructureNotifyMask|ButtonPressMask|PointerMotionMask|EnterWindowMask|LeaveWindowMask|StructureNotifyMask|PropertyChangeMask)

//#define ROOT_EVENT_MASK		(SubstructureRedirectMask|SubstructureNotifyMask|PointerMotionMask|EnterWindowMask|LeaveWindowMask|StructureNotifyMask|PropertyChangeMask)
enum Alignment {CENTER, LEFT, RIGHT};
enum LauncherState {SHOWING, HIDING};

#define COL_SELECTED_BG 		YELLOW
#define COL_SELECTED_FG 		BLACK
#define COL_MENU_BG			GRAY
#define WINDOW_GAP			10
#define COL_MENU_DESKTOP_BG		GRAY
#define COL_MENU_DESKTOP_FG		LGT_GRAY
#define COL_MENU_DESKTOP_IND_POP	GREEN
#define COL_MENU_DESKTOP_IND_EMPTY	MEDIUM_GRAY2
#define COL_MENU_SETTINGS_BG		GRAY
#define COL_MENU_SETTINGS_FG		LGT_GRAY
#define COL_STATUS_BAR_BG		GRAY
#define COL_STATUS_BAR_CLOCK		WHITE
#define COL_STATUS_BAR_TITLE		YELLOW
#define COL_STATUS_BAR_INFO		WHITE
#define COL_STATUS_BAR_DESKTOP_FG	BLACK
#define COL_STATUS_BAR_DESKTOP_BG	YELLOW
#define COL_BORDER_SELECTED		YELLOW
#define COL_BORDER			LGT_GRAY
#define COL_LAUNCHER_BG                 GRAY
#define COL_LAUNCHER_FG                 LGT_GRAY
#define COL_LAUNCHER_RESULT_BG          MEDIUM_GRAY2
#define COL_LAUNCHER_RESULT_SEL_BG	MEDIUM_GRAY
#define COL_LAUNCHER_RESULT_FG          YELLOW 
#define BORDER_WIDTH			1
#define MENU_POSITION                   CENTER
static int CIRCLE[2]			= {0, 360 * 64};

static Atom NET_CLIENT_LIST;
static Atom NET_WM_WINDOW_TYPE;
static Atom NET_WM_WINDOW_TYPE_NORMAL;
static Atom NET_WM_STATE;
static Atom NET_SUPPORTED;
static Atom NET_ACTIVE_WINDOW;
static Atom NET_WM_NAME; 
static Atom NET_WM_STATE_FULLSCREEN; 
static Atom NET_WM_WINDOW_TYPE_DIALOG; 

static Atom WM_DELETE_WINDOW;
static Atom WM_PROTOCOLS;
static Atom WM_TAKE_FOCUS;
static Atom WM_STATE;

#endif //_CONSTANTS_
