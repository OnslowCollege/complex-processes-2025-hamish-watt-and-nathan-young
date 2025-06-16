#pragma once

#include "./utils.h"
#include "./vwnd.h"
#include <windows.h>

#define VWNDIDX unsigned int

struct VScreen
{
    unsigned int w;
    unsigned int h;
    VEC windows;
};

struct VScreen *createvscreen(unsigned int w, unsigned int h);
VWNDIDX bindvwnd(struct VScreen *vscreen, struct VWnd *vwnd);
void scalevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, int sclx, int scly);
void updatevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, HDC hdc, LPRECT wnddim);
