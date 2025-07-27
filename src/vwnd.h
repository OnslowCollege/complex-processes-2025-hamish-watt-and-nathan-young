#pragma once

#include "./msg.h"
#include <windows.h>

#define TOOLBAR_HEIGHT 20

struct VWnd
{
    int vwndidx;
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
    DEFAULT,
};

enum VWndState
{
    FLOATING,
    MAXIMIZED,
    MINIMIZED,
};

struct VWnd *createvwnd(unsigned int top, unsigned int bottom, unsigned int left, unsigned int right,
                        enum VWndStyle vwndstyle);
VWNDIDX bindvwnd(struct VScreen *vscreen, struct VWnd *vwnd);

void clrvwnd(struct VWnd *vwnd);
