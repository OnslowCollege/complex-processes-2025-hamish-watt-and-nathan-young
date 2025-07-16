#include "./msg.h"

void sendvwndevent(struct VScreen *vscreen, VWNDIDX vwndidx, enum VWndMsg msg, long param)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
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
        default:
            break;
        }
    }
}

void sendglobalevent(struct VScreen *vscreen, enum VWndMsg msg, long param)
{
    for (int i = 0; i < veclength(&vscreen->windows); i++)
    {
        sendvwndevent(vscreen, i, msg, param);
    }
}

void removeevent(struct VScreen *vscreen, enum VWndMsg msg)
{
    for (int i = 0; i < veclength(&vscreen->windows); i++)
    {
        struct VWnd *vwnd = vecget(&vscreen->windows, i);
        if (*vwnd->msg & msg)
        {
            *vwnd->msg = *vwnd->msg ^ msg;
        }
    }
}

int processmsg(struct VScreen *vscreen, VWNDIDX vwndidx, enum VWndMsg msg, struct MsgFlags *msgflags)
{
    if (msg & SCALED && msg & MOUSEMOVED)
    {
        short dx = HIWORD(msgflags->mousemoved);
        short dy = LOWORD(msgflags->mousemoved);

        SCLRGN sclrgn = msgflags->scaled;

        scalevwnd(vscreen, vwndidx, sclrgn, dx, dy);
        return REDRAW;
    }

    else if (msg & MOVED && msg & MOUSEMOVED)
    {
        short dx = HIWORD(msgflags->mousemoved);
        short dy = LOWORD(msgflags->mousemoved);

        movevwnd(vscreen, vwndidx, dx, dy);
        return REDRAW;
    }

    return NO_REDRAW;
}

int handlevwndmessages(struct VScreen *vscreen)
{
    int redraw = NO_REDRAW;
    for (int i = 0; i < veclength(&vscreen->windows); i++)
    {
        struct VWnd *vwnd = vecget(&vscreen->windows, i);

        if (vwnd->msg)
        {
            enum VWndMsg msg = *vwnd->msg;
            if (processmsg(vscreen, i, msg, vwnd->msgflags))
            {
                redraw = REDRAW;
            }
        }
    }

    return redraw;
}
