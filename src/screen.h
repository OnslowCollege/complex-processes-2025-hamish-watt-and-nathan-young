#pragma once

#include "./utils.h"
#include "./vwnd.h"
#include <windows.h>

#define VWNDIDX unsigned int
#define SCLRGN enum SclRgn

#define REDRAW 1
#define NO_REDRAW 0

#define COORD short

struct VScreen
{
    unsigned int w;
    unsigned int h;
    VEC windows;
};

enum SclRgn
{
    TOPLEFT = 1,
    TOPRIGHT,
    BOTTOMLEFT,
    BOTTOMRIGHT,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
};

struct VScreen *createvscreen(unsigned int w, unsigned int h);
VWNDIDX bindvwnd(struct VScreen *vscreen, struct VWnd *vwnd);
void vcoordcvt(struct VScreen *vscreen, COORD *x, COORD *y, LPRECT wnddim);
void rcoordcvt(struct VScreen *vscreen, COORD *x, COORD *y, LPRECT wnddim);
void scalevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, SCLRGN sclrgn, short sclx, short scly);
void movevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, short dx, short dy);
SCLRGN insclrgn(struct VScreen *vscreen, struct VWnd *vwnd, int ptx, int pty, LPRECT wnddim);
int inmvrgn(struct VScreen *vscreen, struct VWnd *vwnd, int ptx, int pty, LPRECT wnddim);
void drawvwnd(struct VScreen *vscreen, VWNDIDX vwndidx, HDC hdc, LPRECT wnddim);
float getaspctscl(struct VScreen *vscreen, LPRECT wnddim);
