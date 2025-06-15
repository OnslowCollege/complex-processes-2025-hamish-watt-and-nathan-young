#include "./screen.h"
#include "./graphics.h"
#include "./utils.h"
#include "./vwnd.h"
#include <windows.h>

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

void updatevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, HDC hdc)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    drawstylerect(hdc, vwnd->x, vwnd->y, vwnd->w, vwnd->h);
}

void scalevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, int sclx, int scly)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    vwnd->w += sclx;
    vwnd->h += scly;
}
