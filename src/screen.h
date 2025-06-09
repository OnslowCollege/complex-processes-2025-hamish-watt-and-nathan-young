#pragma once

#include "./vwnd.h"
#include <windows.h>

#define VWNDIDX unsigned int

struct VScreen
{
    unsigned int w;
    unsigned int h;
    struct VWnd **windows;
};

struct VScreen *createvscreen(unsigned int w, unsigned int h);

VWNDIDX bindvwnd(struct VScreen *vscreen, struct VWnd *vwnd, HDC hdc);
void updatevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, HDC hdc);

struct VWnd *editvwnd(struct VScreen *vscreen, VWNDIDX vwndidx);
