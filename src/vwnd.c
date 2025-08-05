#include "./vwnd.h"
#include "./applications/applications.h"
#include "./elements/elements.h"
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
        for (int i = 0; i < sizeof(applications) / sizeof(applications[0]); i++)
        {
            HELEMENT *testicon = malloc(sizeof(HELEMENT));
            *testicon = newelement(0, (vwnd->bottom - vwnd->top), i * 40, i * 40 + 40, &vwnd->left, &vwnd->top);
            addattribute(*testicon, CLICKABLE, applications[i]->launcher);

            pushvec(&vwnd->elements, testicon);
        }
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

    printf("vwndidx: %d\n", next_vwndidx);

    focusvwnd(vscreen, next_vwndidx);

    return next_vwndidx++;
}

int isfocused(struct VScreen *vscreen, VWNDIDX vwndidx)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    return vwnd->focused;
}

void focusvwnd(struct VScreen *vscreen, VWNDIDX vwndidx)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    for (int i = 0; i < veclength(&vscreen->windows); i++)
    {
        struct VWnd *other_wnd = vecget(&vscreen->windows, i);
        other_wnd->focused = 0;
    }

    vwnd->focused = 1;

    if (veclength(&vscreen->windows) - 1 == vwndidx)
    {
        return;
    }

    rmvec(&vscreen->windows, vwndidx);
    pushvec(&vscreen->windows, vwnd);

    refreshvwndidx(vscreen);
}

void clrvwnd(struct VScreen *vscreen, VWNDIDX vwndidx)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    rmvec(&vscreen->windows, vwndidx);

    free(vwnd->pxarr);
    for (int i = 0; i < veclength(&vwnd->elements); i++)
    {
        free(vecget(&vwnd->elements, i));
    }
    clrvec(&vwnd->elements);
    free(vwnd->msg);
    free(vwnd->vwndstyle);
    free(vwnd->msgflags);
}
