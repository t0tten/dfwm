#ifndef _CONFIG_
#define _CONFIG_

#define ACTION_SHELL            0x0001
#define ACTION_MENU             0x0002
#define ACTION_DESKTOP_1        0x0003
#define ACTION_DESKTOP_2        0x0004
#define ACTION_DESKTOP_3        0x0005
#define ACTION_DESKTOP_4        0x0006
#define ACTION_DESKTOP_5        0x0007
#define ACTION_DESKTOP_6        0x0008
#define ACTION_DESKTOP_7        0x0009
#define ACTION_DESKTOP_8        0x000a
#define ACTION_DESKTOP_9        0x000b
#define ACTION_DESKTOP_0        0x000c
#define ACTION_NEXT_DESKTOP     0x000d
#define ACTION_PREV_DESKTOP     0x000e
#define ACTION_QUIT_WM          0x000f
#define ACTION_LAUNCHER         0x0010
#define ACTION_KILL_ACTIVE      0x0011
#define ACTION_HIDE             0x0012

#define DEFAULT_TERMINAL "/usr/bin/mate-terminal"

#define NUM 16
#define CTRL 4
#define MOD (1<<3)

#define NUM_HOTKEYS (sizeof(hotkeys)/sizeof(hotkey))

#include <X11/keysym.h>

typedef struct hotkey {
        unsigned int action;
        unsigned int modifier;
        unsigned int key;
} hotkey;

const hotkey hotkeys[] = 
{
        {ACTION_SHELL, CTRL | MOD, XK_t},
        {ACTION_MENU, 0, XK_Alt_L},
        {ACTION_DESKTOP_1, MOD, XK_1},
        {ACTION_DESKTOP_2, MOD, XK_2},
        {ACTION_DESKTOP_3, MOD, XK_3},
        {ACTION_DESKTOP_4, MOD, XK_4},
        {ACTION_DESKTOP_5, MOD, XK_5},
        {ACTION_DESKTOP_6, MOD, XK_6},
        {ACTION_DESKTOP_7, MOD, XK_7},
        {ACTION_DESKTOP_8, MOD, XK_8},
        {ACTION_DESKTOP_9, MOD, XK_9},
        {ACTION_DESKTOP_0, MOD, XK_0},
        {ACTION_NEXT_DESKTOP, MOD, XK_Right},
        {ACTION_NEXT_DESKTOP, MOD, XK_Up},
        {ACTION_PREV_DESKTOP, MOD, XK_Down},
        {ACTION_PREV_DESKTOP, MOD, XK_Left},
        {ACTION_LAUNCHER, MOD, XK_Return},
        {ACTION_KILL_ACTIVE, MOD, XK_q},
        {ACTION_HIDE, 0, XK_Escape},
        {ACTION_QUIT_WM, MOD, XK_Escape}
};

#endif


