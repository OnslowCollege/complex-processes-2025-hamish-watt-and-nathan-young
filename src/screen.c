#include "./screen.h"
#include "./graphics.h"
#include "./utils.h"
#include "./vwnd.h"
#include <windows.h>
#include <stdio.h>
#include <math.h>

struct VScreen *createvscreen(unsigned int w, unsigned int h)
{
    struct VScreen *screen = malloc(sizeof(struct VScreen));
    screen->w = w;
    screen->h = h;
    screen->windows = createvec(30);

    return screen;
}

VWNDIDX bindvwnd(struct VScreen *vscreen, struct VWnd *vwnd)
{
    static int next_vwndidx = 0;
    pushvec(&vscreen->windows, vwnd);

    return next_vwndidx++;
}

void updatevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, HDC hdc, LPRECT wnddim)
{
    LONG wndw = wnddim->right - wnddim->left;
    LONG wndh = wnddim->bottom - wnddim->top;
    float wndsclx = (float) wndw / (float) vscreen->w;
    float wndscly = (float) wndh / (float) vscreen->h;
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    float rlx = ceil((float)vwnd->x * wndsclx);
    float rly = ceil((float)vwnd->y * wndscly);
    float rlw = ceil((float)vwnd->w * wndsclx);
    float rlh = ceil((float)vwnd->h * wndscly);
    drawstylerect(hdc, rlx, rly, rlw, rlh);
}

void scalevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, int sclx, int scly)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    vwnd->w += sclx;
    vwnd->h += scly;
}
