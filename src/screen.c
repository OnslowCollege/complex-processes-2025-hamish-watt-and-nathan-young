#include "./screen.h"
#include "./graphics.h"
#include "./utils.h"
#include "./vwnd.h"
#include <stdio.h>
#include <windows.h>

#define SCALEREGIONSIZE 20
#define MIN_WINDOW_WIDTH 50
#define MIN_WINDOW_HEIGHT 40

struct VScreen *createvscreen(unsigned int w, unsigned int h)
{
    struct VScreen *screen = malloc(sizeof(struct VScreen));
    screen->w = w;
    screen->h = h;
    screen->windows = createvec(30);

    return screen;
}

VWNDIDX bindvwnd(struct VScreen *vscreen, struct VWnd *vwnd)
{
    static int next_vwndidx = 0;
    pushvec(&vscreen->windows, vwnd);

    return next_vwndidx++;
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

void drawvwnd(struct VScreen *vscreen, VWNDIDX vwndidx, HDC hdc, LPRECT wnddim)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    int left = vwnd->left;
    int top = vwnd->top;
    int right = vwnd->right;
    int bottom = vwnd->bottom;

    printf("Left: %d, Top: %d, Right: %d, Bottom: %d\n", left, top, right, bottom);

    vcoordcvt(vscreen, &left, &top, wnddim);
    vcoordcvt(vscreen, &right, &bottom, wnddim);

    drawstylerect(hdc, left, top, right - left, bottom - top);
}

SCLRGN insclrgn(struct VScreen *vscreen, struct VWnd *vwnd, int ptx, int pty, LPRECT wnddim)
{
    int left = vwnd->left;
    int top = vwnd->top;
    int right = vwnd->right;
    int bottom = vwnd->bottom;

    vcoordcvt(vscreen, &left, &top, wnddim);
    vcoordcvt(vscreen, &right, &bottom, wnddim);

    if (abs(right - ptx) < SCALEREGIONSIZE)
    {
        if (abs(bottom - pty) < SCALEREGIONSIZE)
        {
            return BOTTOMRIGHT;
        }
        else if (abs(top - pty) < SCALEREGIONSIZE)
        {
            return TOPRIGHT;
        }
        else
        {
            return RIGHT;
        }
    }

    if (abs(left - ptx) < SCALEREGIONSIZE)
    {
        if (abs(bottom - pty) < SCALEREGIONSIZE)
        {
            return BOTTOMLEFT;
        }
        else if (abs(top - pty) < SCALEREGIONSIZE)
        {
            return TOPLEFT;
        }
        else
        {
            return LEFT;
        }
    }

    return 0;
}

void scalevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, SCLRGN sclrgn, short sclx, short scly)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    unsigned int *px = NULL;
    unsigned int *py = NULL;

    switch (sclrgn)
    {
    case TOPLEFT:
        px = &vwnd->left;
        py = &vwnd->top;
        break;
    case TOPRIGHT:
        px = &vwnd->right;
        py = &vwnd->top;
        break;
    case BOTTOMLEFT:
        px = &vwnd->left;
        py = &vwnd->bottom;
        break;
    case BOTTOMRIGHT:
        px = &vwnd->right;
        py = &vwnd->bottom;
        break;
    case TOP:
        py = &vwnd->top;
        break;
    case BOTTOM:
        py = &vwnd->bottom;
        break;
    case LEFT:
        px = &vwnd->left;
        break;
    case RIGHT:
        px = &vwnd->right;
        break;
    }

    if (px != NULL)
    {
        int prevpx = *px;
        *px += sclx;
        if (vwnd->right - vwnd->left < MIN_WINDOW_WIDTH)
        {
            *px = prevpx;
        }
    }
    if (py != NULL)
    {
        int prevpy = *py;
        *py += scly;
        if (vwnd->bottom - vwnd->top < MIN_WINDOW_HEIGHT)
        {
            *py = prevpy;
        }
    }
}

void vcoordcvt(struct VScreen *vscreen, int *x, int *y, LPRECT wnddim)
{
    LONG wndw = wnddim->right - wnddim->left;
    LONG wndh = wnddim->bottom - wnddim->top;
    float wndsclx = (float)wndw / (float)vscreen->w;
    float wndscly = (float)wndh / (float)vscreen->h;
    float aspctscl = wndsclx < wndscly ? wndsclx : wndscly;
    float xoffset = (wndw - (vscreen->w * aspctscl)) / 2;
    float yoffset = (wndh - (vscreen->h * aspctscl)) / 2;
    *x = ((float)(*x) * aspctscl) + xoffset;
    *y = ((float)(*y) * aspctscl) + yoffset;
}

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
