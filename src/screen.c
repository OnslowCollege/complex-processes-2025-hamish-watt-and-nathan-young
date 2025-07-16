#include "./screen.h"
#include "./graphics.h"
#include "./utils.h"
#include "./vwnd.h"
#include <stdio.h>
#include <windows.h>

#define SCALEREGIONSIZE 10
#define MOVEREGIONSIZE 10
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

void drawvwnd(struct VScreen *vscreen, VWNDIDX vwndidx, HDC hdc, LPRECT wnddim)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    int left = vwnd->left;
    int top = vwnd->top;
    int right = vwnd->right;
    int bottom = vwnd->bottom;

    vcoordcvt(vscreen, &left, &top, wnddim);
    vcoordcvt(vscreen, &right, &bottom, wnddim);

    drawstylerect(hdc, left, top, right - left, bottom - top);
    drawstylerect(hdc, left, top, right - left, TOOLBAR_HEIGHT);
}

int inmvrgn(struct VScreen *vscreen, struct VWnd *vwnd, int ptx, int pty, LPRECT wnddim)
{

    int left = vwnd->left;
    int top = vwnd->top;
    int right = vwnd->right;
    int bottom = vwnd->bottom;

    vcoordcvt(vscreen, &left, &top, wnddim);
    vcoordcvt(vscreen, &right, &bottom, wnddim);

    RECT sclrect = {left - SCALEREGIONSIZE, top - SCALEREGIONSIZE, right + SCALEREGIONSIZE, bottom + SCALEREGIONSIZE};
    POINT pt = {ptx, pty};

    if (!PtInRect(&sclrect, pt))
    {
        return 0;
    }

    int rgny = top + SCALEREGIONSIZE + MOVEREGIONSIZE;
    if (abs(rgny - pty) < MOVEREGIONSIZE)
    {
        return 1;
    }

    return 0;
}

SCLRGN insclrgn(struct VScreen *vscreen, struct VWnd *vwnd, int ptx, int pty, LPRECT wnddim)
{
    int left = vwnd->left;
    int top = vwnd->top;
    int right = vwnd->right;
    int bottom = vwnd->bottom;

    vcoordcvt(vscreen, &left, &top, wnddim);
    vcoordcvt(vscreen, &right, &bottom, wnddim);

    RECT sclrect = {left - SCALEREGIONSIZE, top - SCALEREGIONSIZE, right + SCALEREGIONSIZE, bottom + SCALEREGIONSIZE};
    POINT pt = {ptx, pty};

    if (!PtInRect(&sclrect, pt))
    {
        return 0;
    }

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

    if (abs(top - pty) < SCALEREGIONSIZE)
    {
        return TOP;
    }
    if (abs(bottom - pty) < SCALEREGIONSIZE)
    {
        return BOTTOM;
    }

    return 0;
}

void movevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, short dx, short dy)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    vwnd->left += dx;
    vwnd->right += dx;
    vwnd->top += dy;
    vwnd->bottom += dy;
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
        if ((int)(vwnd->right - vwnd->left) < MIN_WINDOW_WIDTH)
        {
            *px = prevpx;
        }
    }
    if (py != NULL)
    {
        int prevpy = *py;
        *py += scly;
        if ((int)(vwnd->bottom - vwnd->top) < MIN_WINDOW_HEIGHT)
        {
            *py = prevpy;
        }
    }
}

void vcoordcvt(struct VScreen *vscreen, int *x, int *y, LPRECT wnddim)
{
    // Get the width and height of the real window.
    LONG wndw = wnddim->right - wnddim->left;
    LONG wndh = wnddim->bottom - wnddim->top;
    // Get ratio between real and virtual screen.
    float wndsclx = (float)wndw / (float)vscreen->w;
    float wndscly = (float)wndh / (float)vscreen->h;
    // Use smaller ratio to keep aspect ratio constant.
    float aspctscl = wndsclx < wndscly ? wndsclx : wndscly;
    // Calculate offset to virtual window is centered.
    float xoffset = (wndw - (vscreen->w * aspctscl)) / 2;
    float yoffset = (wndh - (vscreen->h * aspctscl)) / 2;
    // Apply aspect scale and offset to values.
    *x = ((float)(*x) * aspctscl) + xoffset;
    *y = ((float)(*y) * aspctscl) + yoffset;
}
