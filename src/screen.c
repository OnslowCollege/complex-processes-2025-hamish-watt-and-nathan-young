#include "./screen.h"
#include "./elements/elements.h"
#include "./graphics.h"
#include "./utils.h"
#include "./vwnd.h"
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

void drawvwnd(struct VScreen *vscreen, VWNDIDX vwndidx, HDC hdc, LPRECT wnddim)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    COORD left = vwnd->left;
    COORD top = vwnd->top;
    COORD right = vwnd->right;
    COORD bottom = vwnd->bottom;

    vcoordcvt(vscreen, &left, &top, wnddim);
    vcoordcvt(vscreen, &right, &bottom, wnddim);

    switch (*vwnd->vwndstyle)
    {
    case DEFAULT: {
        COORD toolbarright = vwnd->right;
        COORD toolbarbottom = vwnd->top + TOOLBAR_HEIGHT;
        vcoordcvt(vscreen, &toolbarright, &toolbarbottom, wnddim);

        drawstylerect(hdc, left, top, right - left, bottom - top);
        drawstylerect(hdc, left, top, toolbarright - left, toolbarbottom - top);
        break;
    }
    case DESKTOP: {
        RECT desktoprect = {left, top, right, bottom};
        FillRect(hdc, &desktoprect, CreateSolidBrush(GetNearestColor(hdc, RGB(81, 167, 224))));
        break;
    }
    case TASKBAR: {
        drawstylerect(hdc, left, top, right - left, bottom - top);
        break;
    }
    }

    for (int i = 0; i < veclength(&vwnd->elements); i++)
    {
        HELEMENT helem = *(HELEMENT *)vecget(&vwnd->elements, i);
        drawelement(hdc, vscreen, helem, wnddim);
    }
}

WNDRGN inwndrgn(struct VScreen *vscreen, VWNDIDX vwndidx, int ptx, int pty, LPRECT wnddim)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    if (*vwnd->vwndstyle != DEFAULT)
    {
        return 0;
    }

    COORD left = vwnd->left;
    COORD top = vwnd->top;
    COORD right = vwnd->right;
    COORD bottom = vwnd->bottom;

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

    int rgny = top + SCALEREGIONSIZE + MOVEREGIONSIZE;
    if (abs(rgny - pty) < MOVEREGIONSIZE)
    {
        return MOVEREGION;
    }

    return INWINDOW;
}

void movevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, short dx, short dy, COORD moveinitx, COORD moveinity)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    short x = moveinitx + dx;
    short y = moveinity + dy;

    short w = vwnd->right - vwnd->left;
    short h = vwnd->bottom - vwnd->top;

    vwnd->left = x;
    vwnd->right = x + w;
    vwnd->top = y;
    vwnd->bottom = y + h;
}

void scalevwnd(struct VScreen *vscreen, VWNDIDX vwndidx, WNDRGN wndrgn, short x, short y)
{
    struct VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    switch (wndrgn)
    {
    case TOPLEFT:
        vwnd->left = x;
        vwnd->top = y;
        break;
    case TOPRIGHT:
        vwnd->right = x;
        vwnd->top = y;
        break;
    case BOTTOMLEFT:
        vwnd->left = x;
        vwnd->bottom = y;
        break;
    case BOTTOMRIGHT:
        vwnd->right = x;
        vwnd->bottom = y;
        break;
    case TOP:
        vwnd->top = y;
        break;
    case BOTTOM:
        vwnd->bottom = y;
        break;
    case LEFT:
        vwnd->left = x;
        break;
    case RIGHT:
        vwnd->right = x;
        break;
    default:
        break;
    }
}

void vcoordcvt(struct VScreen *vscreen, COORD *x, COORD *y, LPRECT wnddim)
{
    float aspctscl = getaspctscl(vscreen, wnddim);
    // Calculate offset to virtual window is centered.
    float xoffset = ((wnddim->right - wnddim->left) - (vscreen->w * aspctscl)) / 2;
    float yoffset = ((wnddim->bottom - wnddim->top) - (vscreen->h * aspctscl)) / 2;
    // Apply aspect scale and offset to values.
    *x = ((float)(*x) * aspctscl) + xoffset;
    *y = ((float)(*y) * aspctscl) + yoffset;
}

void rcoordcvt(struct VScreen *vscreen, COORD *x, COORD *y, LPRECT wnddim)
{
    float aspctscl = getaspctscl(vscreen, wnddim);
    // Calculate offset to virtual window is centered.
    float xoffset = ((wnddim->right - wnddim->left) - (vscreen->w * aspctscl)) / 2;
    float yoffset = ((wnddim->bottom - wnddim->top) - (vscreen->h * aspctscl)) / 2;
    // Apply aspect scale and offset to values.
    *x = ((float)(*x - xoffset) / aspctscl);
    *y = ((float)(*y - yoffset) / aspctscl);
}

float getaspctscl(struct VScreen *vscreen, LPRECT wnddim)
{
    LONG wndw = wnddim->right - wnddim->left;
    LONG wndh = wnddim->bottom - wnddim->top;
    float wndsclx = (float)wndw / (float)vscreen->w;
    float wndscly = (float)wndh / (float)vscreen->h;

    // Use smaller ratio to keep aspect ratio constant.
    return wndsclx < wndscly ? wndsclx : wndscly;
}

void refreshvwndidx(struct VScreen *vscreen)
{
    for (int i = 0; i < veclength(&vscreen->windows); i++)
    {
        struct VWnd *vwnd = vecget(&vscreen->windows, i);
        vwnd->vwndidx = i;
    }
}
