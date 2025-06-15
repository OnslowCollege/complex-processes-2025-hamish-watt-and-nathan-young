#include "./vwnd.h"
#include <math.h>
#include <stdlib.h>

#define BORDER_SIZE 32

struct VWnd *createvwnd(unsigned int x, unsigned int y, unsigned int w, unsigned int h, enum VWndStyle vwndstyle)
{
    struct VWnd *vwnd = malloc(sizeof(struct VWnd));
    vwnd->x = x;
    vwnd->y = y;
    vwnd->w = w;
    vwnd->h = h;
    vwnd->pxarr = malloc(3 * (w - BORDER_SIZE) * (h - BORDER_SIZE));
    vwnd->toolbar = malloc(0);
    vwnd->vwndstyle = &vwndstyle;

    return vwnd;
}

int insclrgn(struct VWnd *vwnd, int ptx, int pty)
{
    int cornerx = vwnd->x + vwnd->h;
    int cornery = vwnd->y + vwnd->h;

    if (abs(cornerx - ptx) < 10 && abs(cornery - pty) < 10)
    {
        return 1;
    }

    return 0;
}

void clrvwnd(struct VWnd *vwnd)
{
    free(vwnd->pxarr);
    free(vwnd->toolbar);
}
