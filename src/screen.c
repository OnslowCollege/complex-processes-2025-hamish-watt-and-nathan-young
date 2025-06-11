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

VWNDIDX bindvwnd(struct VScreen *vscreen, struct VWnd *vwnd, HDC hdc)
{
    static int next_vwndidx = 0;
    pushvec(&vscreen->windows, vwnd);
    drawstylerect(hdc, vwnd->x, vwnd->y, vwnd->w, vwnd->h);

    return next_vwndidx++;
}
