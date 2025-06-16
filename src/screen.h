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
void vcoordcvt(struct VScreen *vscreen, int *x, int *y, LPRECT wnddim);
void vscalecvt(struct VScreen *vscreen, int *w, int *h, LPRECT wnddim);   
void scalevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, int sclx, int scly);
int insclrgn(struct VScreen *vscreen, struct VWnd *vwnd, int ptx, int pty, LPRECT wnddim);
void updatevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, HDC hdc, LPRECT wnddim);
