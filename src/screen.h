#pragma once

#define VWNDIDX unsigned int

#include "./utils.h"
#include <windows.h>

#define WNDRGN enum WndRgn

#define REDRAW 1
#define NO_REDRAW 0

#define COORD short

struct VScreen
{
    unsigned int w;
    unsigned int h;
    VEC windows;
};

enum WndRgn
{
    INWINDOW = 1,
    TOPLEFT,
    TOPRIGHT,
    BOTTOMLEFT,
    BOTTOMRIGHT,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    MOVEREGION,
};

struct VScreen *createvscreen(unsigned int w, unsigned int h);
void vcoordcvt(struct VScreen *vscreen, COORD *x, COORD *y, LPRECT wnddim);
void rcoordcvt(struct VScreen *vscreen, COORD *x, COORD *y, LPRECT wnddim);
void scalevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, WNDRGN wndrgn, short sclx, short scly);
void movevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, short dx, short dy, COORD moveinitx, COORD moveinity);
WNDRGN inwndrgn(struct VScreen *vscreen, VWNDIDX vwndidx, int ptx, int pty, LPRECT wnddim);
void drawvwnd(struct VScreen *vscreen, VWNDIDX vwndidx, HDC hdc, LPRECT wnddim);
float getaspctscl(struct VScreen *vscreen, LPRECT wnddim);
void refreshvwndidx(struct VScreen *vscreen);
