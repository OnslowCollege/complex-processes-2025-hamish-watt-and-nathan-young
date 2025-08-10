#include "msg.h"
#include "elements/elements.h"
#include "screen.h"
#include "vwnd.h"
#include <stdio.h>
#include <windows.h>

static COORD moveinitx;
static COORD moveinity;

void sendvwndevent(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg, long param)
{
    VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    *vwnd->msg = *vwnd->msg | msg;
    if (param)
    {
        switch (msg)
        {
        case SCALED:
            vwnd->msgflags->scaled = param;
            break;
        case MOUSEMOVED:
            vwnd->msgflags->mousemoved = param;
            break;
        case MOVED:
            vwnd->msgflags->windowmoved = param;
            moveinitx = vwnd->left;
            moveinity = vwnd->top;
            break;
        case MOUSECLICKED:
            vwnd->msgflags->mouseclicked = param;
            break;
        case DOUBLECLICKED:
            vwnd->msgflags->mouseclicked = param;
            break;
        default:
            break;
        }
    }
}

void sendglobalevent(VScreen *vscreen, VWNDMSG msg, long param)
{
    for (int i = veclength(&vscreen->windows) - 1; i >= 0; i--)
    {
        sendvwndevent(vscreen, i, msg, param);
    }
}

void removeevent(VScreen *vscreen, VWNDMSG msg)
{
    for (int i = 0; i < veclength(&vscreen->windows); i++)
    {
        VWnd *vwnd = vecget(&vscreen->windows, i);
        if (*vwnd->msg & msg)
        {
            *vwnd->msg = *vwnd->msg ^ msg;
        }
    }
}

int processmsg(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg, MsgFlags *msgflags)
{
    VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    if (msg & SCALED && msg & MOUSEMOVED)
    {
        short x = HIWORD(msgflags->mousemoved);
        short y = LOWORD(msgflags->mousemoved);

        WNDRGN wndrgn = msgflags->scaled;

        scalevwnd(vscreen, vwndidx, wndrgn, x, y);
        return REDRAW;
    }
    else if (msg & MOVED && msg & MOUSEMOVED)
    {
        short xi = HIWORD(msgflags->windowmoved);
        short yi = LOWORD(msgflags->windowmoved);
        short xf = HIWORD(msgflags->mousemoved);
        short yf = LOWORD(msgflags->mousemoved);

        movevwnd(vscreen, vwndidx, xf - xi, yf - yi, moveinitx, moveinity);
        return REDRAW;
    }

    if (msg & MOUSECLICKED)
    {
        *vwnd->msg = *vwnd->msg ^ MOUSECLICKED;
        for (int i = 0; i < veclength(&vwnd->elements); i++)
        {
            HELEMENT helem = *(HELEMENT *)vecget(&vwnd->elements, i);
            if (hasattribute(helem, CLICKABLE))
            {
                COORD x = HIWORD(msgflags->mouseclicked);
                COORD y = LOWORD(msgflags->mouseclicked);

                if (ptinelem(helem, x, y))
                {
                    executeelem(helem, vscreen, vwndidx);
                    return REDRAW;
                }
            }
        }
    }

    if (msg & DOUBLECLICKED)
    {
        *vwnd->msg = *vwnd->msg ^ DOUBLECLICKED;
        for (int i = 0; i < veclength(&vwnd->elements); i++)
        {
            HELEMENT helem = *(HELEMENT *)vecget(&vwnd->elements, i);
            if (hasattribute(helem, DOUBLECLICKABLE))
            {
                COORD x = HIWORD(msgflags->mouseclicked);
                COORD y = LOWORD(msgflags->mouseclicked);

                if (ptinelem(helem, x, y))
                {
                    executeelem(helem, vscreen, vwndidx);
                    return REDRAW;
                }
            }
        }
    }
    return NO_REDRAW;
}

int handlevwndmessages(VScreen *vscreen)
{
    int redraw = NO_REDRAW;
    for (int i = 0; i < veclength(&vscreen->windows); i++)
    {
        VWnd *vwnd = vecget(&vscreen->windows, i);

        if (vwnd->msg && isfocused(vscreen, i))
        {
            VWNDMSG msg = *vwnd->msg;
            if (processmsg(vscreen, i, msg, vwnd->msgflags))
            {
                redraw = REDRAW;
            }
        }
    }

    removeevent(vscreen, MOUSEMOVED);

    return redraw;
}
