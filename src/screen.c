#include "./screen.h"
#include "./graphics.h"
#include "./vwnd.h"
#include <windows.h>

#define MAX_WINDOW_COUNT 30

struct VScreen *createvscreen(unsigned int w, unsigned int h)
{
    struct VScreen *screen = malloc(sizeof(struct VScreen));
    screen->w = w;
    screen->h = h;
    screen->windows = malloc(sizeof(struct VWnd) * MAX_WINDOW_COUNT);

    return screen;
}

VWNDIDX bindvwnd(struct VScreen *vscreen, struct VWnd *vwnd, HDC hdc)
{
    static int next_vwndidx = 0;
    vscreen->windows[next_vwndidx] = vwnd;
    drawstylerect(hdc, vwnd->x, vwnd->y, vwnd->w, vwnd->h);

    return next_vwndidx++;
}
