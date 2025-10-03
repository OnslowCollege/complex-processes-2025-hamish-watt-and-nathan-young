#include "./vwnd.h"
#include "./elements/elements.h"
#include "msg.h"
#include <stdlib.h>

#define BORDER_SIZE 32

static int next_id = 0;

VWnd *createvwnd(unsigned int top, unsigned int bottom, unsigned int left, unsigned int right, VWNDSTYLE vwndstyle)
{

    VWnd *vwnd = malloc(sizeof(VWnd));

    vwnd->id = next_id;

    next_id++;

    vwnd->top = top;
    vwnd->bottom = bottom;
    vwnd->left = left;
    vwnd->right = right;
    vwnd->elements = createvec(15);
    vwnd->vwndstyle = malloc(sizeof(VWNDSTYLE));
    *vwnd->vwndstyle = vwndstyle;

    vwnd->msg = malloc(sizeof(VWNDMSG));
    memset(vwnd->msg, 0, sizeof(VWNDMSG));

    vwnd->msgflags = malloc(sizeof(MsgFlags));
    memset(vwnd->msgflags, 0, sizeof(MsgFlags));

    vwnd->application = malloc(sizeof(Application));

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

    focusvwnd(vscreen, vwndidx);

    return vwndidx;
}

VWnd *vwndbyid(VScreen *vscreen, int id)
{
    for (int i = 0; i < veclength(&vscreen->windows); i++)
    {
        VWnd *vwnd = vecget(&vscreen->windows, i);
        if (vwnd->id == id)
            return vwnd;
    }

    return 0;
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
    sendglobalevent(vscreen, APPFOCUSED, vwnd->id);

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

    if (vwnd->application != NULL)
    {
        if (vwnd->application->unlauncher != NULL)
        {
            vwnd->application->unlauncher(vscreen, vwnd->id);
        }
    }

    free(vwnd->application);

    rmvec(&vscreen->windows, vwndidx);

    for (int i = 0; i < veclength(&vwnd->elements); i++)
    {
        rmelement(*(HELEMENT *)(vecget(&vwnd->elements, i)));
        free(vecget(&vwnd->elements, i));
    }

    clrvec(&vwnd->elements);
    free(vwnd->msg);
    free(vwnd->vwndstyle);
    free(vwnd->msgflags);
}
