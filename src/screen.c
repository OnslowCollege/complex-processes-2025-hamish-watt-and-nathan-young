#include "./screen.h"
#include "./graphics.h"
#include "./utils.h"
#include "./vwnd.h"
#include <windows.h>

#define SCALEREGIONSIZE 20

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

void processmsg(struct VScreen *vscreen, VWNDIDX vwndidx, enum VWndMsg msg, long param)
{
    if (msg & SCALED && msg & MOUSEMOVED)
    {
        short dx = HIWORD(param);
        short dy = LOWORD(param);

        scalevwnd(vscreen, vwndidx, dx, dy);
    }
}

void handlevwndmessages(struct VScreen *vscreen)
{
    for (int i = 0; i < veclength(&vscreen->windows); i++)
    {
        struct VWnd *vwnd = vecget(&vscreen->windows, i);

        if (vwnd->msg)
        {
            enum VWndMsg msg = *vwnd->msg;
            int param = vwnd->msgparam;
            processmsg(vscreen, i, msg, param);
        }
    }
}

void drawvwnd(struct VScreen *vscreen, VWNDIDX vwndidx, HDC hdc, LPRECT wnddim)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    int rlx = vwnd->x;
    int rly = vwnd->y;
    int rlw = vwnd->w;
    int rlh = vwnd->h;
    vcoordcvt(vscreen, &rlx, &rly, wnddim);
    vscalecvt(vscreen, &rlw, &rlh, wnddim);
    drawstylerect(hdc, rlx, rly, rlw, rlh);
}

SCLRGN insclrgn(struct VScreen *vscreen, struct VWnd *vwnd, int ptx, int pty, LPRECT wnddim)
{
    int left = vwnd->x;
    int top = vwnd->y;
    int right = vwnd->x + vwnd->w;
    int bottom = vwnd->y + vwnd->h;

    vcoordcvt(vscreen, &right, &top, wnddim);
    vcoordcvt(vscreen, &left, &bottom, wnddim);

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

void vscalecvt(struct VScreen *vscreen, int *w, int *h, LPRECT wnddim)
{
    LONG wndw = wnddim->right - wnddim->left;
    LONG wndh = wnddim->bottom - wnddim->top;
    float wndsclx = (float)wndw / (float)vscreen->w;
    float wndscly = (float)wndh / (float)vscreen->h;
    float aspctscl = wndsclx < wndscly ? wndsclx : wndscly;
    *w = (float)(*w) * aspctscl;
    *h = (float)(*h) * aspctscl;
}

void scalevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, int sclx, int scly)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    vwnd->w += sclx;
    vwnd->h += scly;
}

void sendvwndevent(struct VScreen *vscreen, VWNDIDX vwndidx, enum VWndMsg msg, long param)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    *vwnd->msg = *vwnd->msg | msg;
    if (param)
    {
        vwnd->msgparam = param;
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
        *vwnd->msg = *vwnd->msg ^ msg;
    }
}
