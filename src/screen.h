#pragma once

#define VWNDIDX unsigned int

#include "./utils.h"
#include <windows.h>

#define WNDRGN enum WndRgn

#define REDRAW 1
#define NO_REDRAW 0

#define COORD short

typedef struct
{
    unsigned int w;
    unsigned int h;
    VEC windows;
} VScreen;

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

VScreen *createvscreen(unsigned int w, unsigned int h);

void vcoordcvt(VScreen *vscreen, COORD *x, COORD *y, LPRECT wnddim);

void rcoordcvt(VScreen *vscreen, COORD *x, COORD *y, LPRECT wnddim);

void scalevwnd(VScreen *vscreen, VWNDIDX vwndidx, WNDRGN wndrgn, short sclx, short scly);

void movevwnd(VScreen *vscreen, VWNDIDX vwndidx, short dx, short dy, COORD moveinitx, COORD moveinity);

WNDRGN inwndrgn(VScreen *vscreen, VWNDIDX vwndidx, int ptx, int pty, LPRECT wnddim);

void drawvwnd(VScreen *vscreen, VWNDIDX vwndidx, HDC hdc, LPRECT wnddim);

float getaspctscl(VScreen *vscreen, LPRECT wnddim);

void refreshvwndidx(VScreen *vscreen);
