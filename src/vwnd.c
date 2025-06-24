#include "./vwnd.h"
#include <stdlib.h>

#define BORDER_SIZE 32

struct VWnd *createvwnd(unsigned int top, unsigned int bottom, unsigned int left, unsigned int right,
                        enum VWndStyle vwndstyle)
{
    struct VWnd *vwnd = malloc(sizeof(struct VWnd));
    vwnd->top = top;
    vwnd->bottom = bottom;
    vwnd->left = left;
    vwnd->right = right;
    vwnd->pxarr = malloc(3 * (right - left - BORDER_SIZE) * (bottom - top - BORDER_SIZE));
    vwnd->toolbar = malloc(0);
    vwnd->vwndstyle = &vwndstyle;
    vwnd->msg = malloc(sizeof(enum VWndMsg));
    vwnd->msgflags = malloc(sizeof(struct MsgFlags));

    return vwnd;
}

void clrvwnd(struct VWnd *vwnd)
{
    free(vwnd->pxarr);
    free(vwnd->toolbar);
    free(vwnd->msg);
}
