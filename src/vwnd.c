#include "./vwnd.h"
#include "./elements/elements.h"
#include "./applications/application.h"
#include <stdio.h>
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
    vwnd->elements = createvec(15);
    vwnd->vwndstyle = malloc(sizeof(enum VWndStyle));
    *vwnd->vwndstyle = vwndstyle;
    vwnd->msg = malloc(sizeof(enum VWndMsg));
    vwnd->msgflags = malloc(sizeof(struct MsgFlags));

    switch (*vwnd->vwndstyle)
    {
    case DEFAULT: {
        HELEMENT *hclosebutton = malloc(sizeof(HELEMENT));
        *hclosebutton = newelement(0, TOOLBAR_HEIGHT, -TOOLBAR_HEIGHT, 0, &vwnd->right, &vwnd->top);
        addattribute(*hclosebutton, CLICKABLE, clrvwnd);

        pushvec(&vwnd->elements, hclosebutton);
        break;
    }
    case TASKBAR: {
        HELEMENT *testicon = malloc(sizeof(HELEMENT));
        *testicon = newelement(0, (vwnd->bottom - vwnd->top), 0, 40, &vwnd->left, &vwnd->top);
        addattribute(*testicon, CLICKABLE, launchapp);

        pushvec(&vwnd->elements, testicon);
        break;
    }
    case DESKTOP: {
        break;
    }
    }

    return vwnd;
}

VWNDIDX bindvwnd(struct VScreen *vscreen, struct VWnd *vwnd)
{
    static int next_vwndidx = 0;
    pushvec(&vscreen->windows, vwnd);

    return next_vwndidx++;
}

void clrvwnd(struct VScreen *vscreen, VWNDIDX vwndidx)
{
    printf("removing window: %d\n", vwndidx);
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    rmvec(&vscreen->windows, vwndidx);

    free(vwnd->pxarr);
    for (int i = 0; i<veclength(&vwnd->elements); i++) {
        free(vecget(&vwnd->elements, i));
    }
    clrvec(&vwnd->elements);
    free(vwnd->msg);
    free(vwnd->vwndstyle);
    free(vwnd->msgflags);
}
