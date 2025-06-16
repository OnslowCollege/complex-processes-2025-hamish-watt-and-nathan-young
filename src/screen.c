#include "./screen.h"
#include "./graphics.h"
#include "./utils.h"
#include "./vwnd.h"
#include <windows.h>
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
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    int rlx = vwnd->x;
    int rly = vwnd->y;
    int rlw = vwnd->w;
    int rlh = vwnd->h;
    vcoordcvt(vscreen, &rlx, &rly, wnddim);
    vcoordcvt(vscreen, &rlw, &rlh, wnddim);
    drawstylerect(hdc, rlx, rly, rlw, rlh);
}

void vcoordcvt(struct VScreen *vscreen, int *x, int *y, LPRECT wnddim)
{
    LONG wndw = wnddim->right - wnddim->left;
    LONG wndh = wnddim->bottom - wnddim->right;
    float wndsclx = (float) wndw / (float) vscreen->w;
    float wndscly = (float) wndh / (float) vscreen->h;
    float aspctscl = wndsclx < wndscly ? wndsclx : wndscly;
    *x = (float)*x * aspctscl;
    *y = (float)*x * aspctscl;
}

void scalevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, int sclx, int scly)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    vwnd->w += sclx;
    vwnd->h += scly;
}
