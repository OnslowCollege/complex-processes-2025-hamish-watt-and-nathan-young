#pragma once

#include "./msg.h"
#include <windows.h>

#define TOOLBAR_HEIGHT 20

struct VWnd
{
    int vwndidx;
    int focused;
    unsigned int top, bottom, left, right;
    unsigned int *pxarr;
    VEC elements;
    enum VWndStyle *vwndstyle;
    enum VWndState *vwndstate;
    enum VWndMsg *msg;

    struct MsgFlags *msgflags;
};

enum VWndStyle
{
    DEFAULT = 1,
    TASKBAR,
    DESKTOP,
};

enum VWndState
{
    FLOATING,
    MAXIMIZED,
    MINIMIZED,
};

struct VWnd *createvwnd(unsigned int top, unsigned int bottom, unsigned int left, unsigned int right,
                        enum VWndStyle vwndstyle);

int isfocused(struct VScreen *vscreen, VWNDIDX vwndidx);
void focusvwnd(struct VScreen *vscreen, VWNDIDX vwndidx);
VWNDIDX bindvwnd(struct VScreen *vscreen, struct VWnd *vwnd);
void clrvwnd(struct VScreen *vscreen, VWNDIDX vwndidx);
