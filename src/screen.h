#pragma once

#include "./vwnd.h"

#define VWNDIDX unsigned int

struct VScreen
{
    unsigned int w;
    unsigned int h;
    struct VWnd **windows;
};

struct VScreen *createvscreen(unsigned int w, unsigned int h);

VWNDIDX bindvwnd(struct VScreen *vscreen, struct VWnd *vwnd, unsigned int x, unsigned int y);
void updatevwnd(struct VScreen *vscreen, VWNDIDX vwndidx);

struct VWnd *editvwnd(struct VScreen *vscreen, VWNDIDX vwndidx);
