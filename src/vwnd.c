#include "./vwnd.h"
#include "./elements/elements.h"
#include <stdio.h>
#include <stdlib.h>

#define BORDER_SIZE 32

VWnd *createvwnd(unsigned int top, unsigned int bottom, unsigned int left, unsigned int right, VWNDSTYLE vwndstyle)
{
    VWnd *vwnd = malloc(sizeof(VWnd));

    vwnd->top = top;
    vwnd->bottom = bottom;
    vwnd->left = left;
    vwnd->right = right;
    vwnd->elements = createvec(15);
    vwnd->vwndstyle = malloc(sizeof(VWNDSTYLE));
    *vwnd->vwndstyle = vwndstyle;
    vwnd->msg = malloc(sizeof(VWNDMSG));
    vwnd->msgflags = malloc(sizeof(MsgFlags));

    switch (*vwnd->vwndstyle)
    {
    case STATIC: {
        HELEMENT *hclosebutton = malloc(sizeof(HELEMENT));
        *hclosebutton = newelement(2, TOOLBAR_HEIGHT, -TOOLBAR_HEIGHT, -2, &vwnd->right, &vwnd->top);
        HBITMAP bmpclosebutton = LoadBitmapA(GetModuleHandle(NULL), "close");
        addattribute(*hclosebutton, HASIMAGE, (int)bmpclosebutton);
        addattribute(*hclosebutton, CLICKABLE, (int)clrvwnd);

        pushvec(&vwnd->elements, hclosebutton);
        break;
    }
    case DEFAULT: {
        HELEMENT *hclosebutton = malloc(sizeof(HELEMENT));
        *hclosebutton = newelement(2, TOOLBAR_HEIGHT, -TOOLBAR_HEIGHT, -2, &vwnd->right, &vwnd->top);
        HBITMAP bmpclosebutton = LoadBitmapA(GetModuleHandle(NULL), "close");
        addattribute(*hclosebutton, HASIMAGE, (int)bmpclosebutton);
        addattribute(*hclosebutton, CLICKABLE, (int)clrvwnd);

        pushvec(&vwnd->elements, hclosebutton);
        break;
    }
    default:
        break;
    }

    return vwnd;
}

VWNDIDX bindvwnd(VScreen *vscreen, VWnd *vwnd)
{
    int vwndidx = veclength(&vscreen->windows);
    pushvec(&vscreen->windows, vwnd);

    printf("vwndidx: %d\n", vwndidx);

    focusvwnd(vscreen, vwndidx);

    return vwndidx;
}

int isfocused(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    if (*vwnd->vwndstyle != DEFAULT && *vwnd->vwndstyle != STATIC)
    {
        return 1;
    }

    return vwnd->focused;
}

void focusvwnd(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    for (int i = 0; i < veclength(&vscreen->windows); i++)
    {
        VWnd *other_wnd = vecget(&vscreen->windows, i);
        other_wnd->focused = 0;

        if (other_wnd->toolbarbmp != NULL)
        {
            DeleteObject(other_wnd->toolbarbmp);
            other_wnd->toolbarbmp = NULL;
        }
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

void clrvwnd(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    rmvec(&vscreen->windows, vwndidx);

    for (int i = 0; i < veclength(&vwnd->elements); i++)
    {
        rmelement(*(HELEMENT *)(vecget(&vwnd->elements, i)));
        free(vecget(&vwnd->elements, i));
    }

    clrvec(&vwnd->elements);
    free(vwnd->application);
    free(vwnd->msg);
    free(vwnd->vwndstyle);
    free(vwnd->msgflags);
}
