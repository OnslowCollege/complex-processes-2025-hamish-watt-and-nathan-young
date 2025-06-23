#include "./vwnd.h"
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
    vwnd->msg = malloc(sizeof(enum VWndMsg));

    return vwnd;
}

void clrvwnd(struct VWnd *vwnd)
{
    free(vwnd->pxarr);
    free(vwnd->toolbar);
    free(vwnd->msg);
}
