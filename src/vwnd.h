#pragma once

#include "./msg.h"
#include <windows.h>
#include "applications/applications.h"

#define TOOLBAR_HEIGHT 20
#define TOPBAR_HEIGHT 18
#define TASKBAR_HEIGHT 40
#define VWNDSTYLE enum VWndStyle

typedef struct
{
    int vwndidx;
    int focused;

    Application *application;

    HBITMAP bmp;
    int bitmap_w, bitmap_h;
    HBITMAP toolbarbmp;

    unsigned int top, bottom, left, right;
    VEC elements;
    enum VWndStyle *vwndstyle;
    enum VWndState *vwndstate;
    enum VWndMsg *msg;

    MsgFlags *msgflags;
} VWnd;

enum VWndStyle
{
    DEFAULT = 1,
    TASKBAR,
    DESKTOP,
    TOPBAR,
    STATIC,
};

enum VWndState
{
    FLOATING,
    MAXIMIZED,
    MINIMIZED,
};

VWnd *createvwnd(unsigned int top, unsigned int bottom, unsigned int left, unsigned int right, VWNDSTYLE vwndstyle);

int isfocused(VScreen *vscreen, VWNDIDX vwndidx);
void focusvwnd(VScreen *vscreen, VWNDIDX vwndidx);
VWNDIDX bindvwnd(VScreen *vscreen, VWnd *vwnd);
void clrvwnd(VScreen *vscreen, VWNDIDX vwndidx);
