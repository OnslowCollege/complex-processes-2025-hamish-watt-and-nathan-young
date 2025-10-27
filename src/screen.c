#include "./screen.h"
#include "./elements/elements.h"
#include "./graphics.h"
#include "./utils.h"
#include "./vwnd.h"
#include <stdio.h>
#include <windows.h>

#define SCALEREGIONSIZE 7
#define MOVEREGIONSIZE 7

#ifdef DEBUG
void showregions(HDC hdc, VScreen *vscreen, VWnd *vwnd)
{
    COORD left = vwnd->left;
    COORD top = vwnd->top;
    COORD right = vwnd->right;
    COORD bottom = vwnd->bottom;

    vcoordcvt(vscreen, &left, &top);
    vcoordcvt(vscreen, &right, &bottom);

    RECT sclrect = {left - SCALEREGIONSIZE, top - SCALEREGIONSIZE,
                    right + SCALEREGIONSIZE, bottom + SCALEREGIONSIZE};

    HBRUSH scaleregionbrush = CreateSolidBrush(RGB(0, 255, 0));
    SelectObject(hdc, scaleregionbrush);

    Rectangle(hdc, sclrect.left, sclrect.top, sclrect.right,
              sclrect.top + 2 * SCALEREGIONSIZE);
    Rectangle(hdc, sclrect.left, sclrect.top,
              sclrect.left + 2 * SCALEREGIONSIZE, sclrect.bottom);
    Rectangle(hdc, sclrect.left, sclrect.bottom - 2 * SCALEREGIONSIZE,
              sclrect.right, sclrect.bottom);
    Rectangle(hdc, sclrect.right - 2 * SCALEREGIONSIZE, sclrect.top,
              sclrect.right, sclrect.bottom);

    DeleteObject(scaleregionbrush);

    HBRUSH moveregionbrush = CreateSolidBrush(RGB(0, 0, 255));
    SelectObject(hdc, moveregionbrush);

    Rectangle(hdc, left, top + SCALEREGIONSIZE, right,
              top + SCALEREGIONSIZE + 2 * MOVEREGIONSIZE);

    DeleteObject(moveregionbrush);
}

#else
#define showregions(...) (void)0

#endif

VScreen *createvscreen(unsigned int w, unsigned int h, RECT wnddim)
{
    VScreen *screen = malloc(sizeof(VScreen));
    screen->w = w;
    screen->h = h;
    screen->windows = createvec(30);
    screen->wnddim = wnddim;

    return screen;
}

void drawvwnd(VScreen *vscreen, VWNDIDX vwndidx, HDC hdc)
{
    VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    if (vwnd == NULL)
    {
        fprintf(stderr, "Could not get vwnd from windows array");
    }

    COORD left = vwnd->left;
    COORD top = vwnd->top;
    COORD right = vwnd->right;
    COORD bottom = vwnd->bottom;

    COORD toolbartop = vwnd->top + 3;
    COORD toolbarleft = vwnd->left + 40;
    COORD toolbarright = vwnd->right - 50;
    COORD toolbarbottom = vwnd->top + TOOLBAR_HEIGHT - 3;

    vcoordcvt(vscreen, &left, &top);
    vcoordcvt(vscreen, &right, &bottom);
    vcoordcvt(vscreen, &toolbarleft, &toolbartop);
    vcoordcvt(vscreen, &toolbarright, &toolbarbottom);

    RECT toolbarrect = {toolbarleft, toolbartop, toolbarright, toolbarbottom};

    // check if the bitmap dimensions are different to the real window
    // dimensions
    if (vwnd->bitmap_w != (right - left) - 1 &&
            vwnd->bitmap_w != right - left ||
        vwnd->bitmap_h != (bottom - top) - 1 && vwnd->bitmap_h != bottom - top)
    {
        // free the bitmap cache for a new bitmap to be able to be created
        DeleteObject(vwnd->bmp);
        DeleteObject(vwnd->toolbarbmp);
        vwnd->bmp = 0;
        vwnd->toolbarbmp = 0;
    }

    HDC memdc = CreateCompatibleDC(hdc);

    switch (*vwnd->vwndstyle)
    {
    case STATIC:
    case DEFAULT: {

        if (vwnd->bmp == 0)
        {
            vwnd->bmp = createstylerect(memdc, right - left, bottom - top);
            vwnd->bitmap_w = right - left;
            vwnd->bitmap_h = bottom - top;
        }

        if (vwnd->toolbarbmp == 0)
        {
            vwnd->toolbarbmp =
                createtoolbarrect(memdc, toolbarright - toolbarleft,
                                  toolbarbottom - toolbartop, vwnd->focused);
        }

        drawimage(hdc, memdc, vwnd->bmp, left, top, right - left, bottom - top);
        drawimage(hdc, memdc, vwnd->toolbarbmp, toolbarleft, toolbartop,
                  toolbarright - toolbarleft, toolbarbottom - toolbartop);

        showregions(hdc, vscreen, vwnd);
        break;
    }
    case DESKTOP: {
        RECT desktoprect = {left, top, right, bottom};
        FillRect(hdc, &desktoprect,
                 CreateSolidBrush(GetNearestColor(hdc, RGB(81, 167, 224))));
        break;
    }
    case TASKBAR: {
        if (vwnd->bmp == 0)
        {
            vwnd->bmp = createstylerect(memdc, right - left, bottom - top);
            vwnd->bitmap_w = right - left;
            vwnd->bitmap_h = bottom - top;
        }

        drawimage(hdc, memdc, vwnd->bmp, left, top, right - left, bottom - top);
        break;
    }
    case TOPBAR: {
        if (vwnd->bmp == 0)
        {
            vwnd->bmp = createstylerect(memdc, right - left, bottom - top);
            vwnd->bitmap_w = right - left;
            vwnd->bitmap_h = bottom - top;
        }

        drawimage(hdc, memdc, vwnd->bmp, left, top, right - left, bottom - top);
        break;
    }
    }

    DeleteDC(memdc);

    for (int i = 0; i < veclength(&vwnd->elements); i++)
    {
        HELEMENT helem = *(HELEMENT *)vecget(&vwnd->elements, i);
        drawelement(hdc, vscreen, helem);
    }
}

WNDRGN inwndrgn(VScreen *vscreen, VWNDIDX vwndidx, int ptx, int pty)
{
    VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    if (*vwnd->vwndstyle == DESKTOP)
    {
        return 0;
    }

    COORD left = vwnd->left;
    COORD top = vwnd->top;
    COORD right = vwnd->right;
    COORD bottom = vwnd->bottom;

    vcoordcvt(vscreen, &left, &top);
    vcoordcvt(vscreen, &right, &bottom);

    RECT sclrect = {left - SCALEREGIONSIZE, top - SCALEREGIONSIZE,
                    right + SCALEREGIONSIZE, bottom + SCALEREGIONSIZE};
    POINT pt = {ptx, pty};

    if (!PtInRect(&sclrect, pt))
    {
        return 0;
    }

    int rgny = top + SCALEREGIONSIZE + MOVEREGIONSIZE;
    if (abs(rgny - pty) < MOVEREGIONSIZE)
    {
        return MOVEREGION;
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

    return INWINDOW;
}

void movevwnd(VScreen *vscreen, VWNDIDX vwndidx, short dx, short dy,
              COORD moveinitx, COORD moveinity)
{
    VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    if (*vwnd->vwndstyle != DEFAULT && *vwnd->vwndstyle != STATIC)
    {
        return;
    }

    COORD x = moveinitx + dx;
    COORD y = moveinity + dy;

    COORD w = vwnd->right - vwnd->left;
    COORD h = vwnd->bottom - vwnd->top;

    // Clamp x to within the vscreen.
    if (x < 0)
        x = 0;
    if (x + w > vscreen->w)
        x = vscreen->w - w;

    // Clamp y to within the vscreen and top/bottom bars.
    if (y < TOPBAR_HEIGHT)
        y = TOPBAR_HEIGHT;
    if (y + h > vscreen->h - TASKBAR_HEIGHT)
        y = vscreen->h - h - TASKBAR_HEIGHT;

    vwnd->left = x;
    vwnd->right = x + w;
    vwnd->top = y;
    vwnd->bottom = y + h;
}

void scalevwnd(VScreen *vscreen, VWNDIDX vwndidx, WNDRGN wndrgn, short x,
               short y)
{
    VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    if (*vwnd->vwndstyle != DEFAULT)
    {
        return;
    }

    COORD prev_top = vwnd->top;
    COORD prev_bottom = vwnd->bottom;
    COORD prev_left = vwnd->left;
    COORD prev_right = vwnd->right;

    switch (wndrgn)
    {
    case TOPLEFT:
        if (x >= vwnd->right || y >= vwnd->bottom)
        {
            return;
        }
        vwnd->left = x;
        vwnd->top = y;
        break;
    case TOPRIGHT:
        if (x <= vwnd->left || y >= vwnd->bottom)
        {
            return;
        }
        vwnd->right = x;
        vwnd->top = y;
        break;
    case BOTTOMLEFT:
        if (x >= vwnd->right || y <= vwnd->top)
        {
            return;
        }
        vwnd->left = x;
        vwnd->bottom = y;
        break;
    case BOTTOMRIGHT:
        if (x <= vwnd->left || y <= vwnd->top)
        {
            return;
        }
        vwnd->right = x;
        vwnd->bottom = y;
        break;
    case TOP:
        if (y >= vwnd->bottom)
        {
            return;
        }
        vwnd->top = y;
        break;
    case BOTTOM:
        if (y <= vwnd->top)
        {
            return;
        }
        vwnd->bottom = y;
        break;
    case LEFT:
        if (x >= vwnd->right)
        {
            return;
        }
        vwnd->left = x;
        break;
    case RIGHT:
        if (x <= vwnd->left)
        {
            return;
        }
        vwnd->right = x;
        break;
    default:
        break;
    }

    if (vwnd->right > vscreen->w) {
        vwnd->left = prev_left;
        vwnd->right = prev_right;
    }
    if (vwnd->bottom > vscreen->h) {
        vwnd->top = prev_top;
        vwnd->bottom = prev_bottom;
    }
    if (vwnd->right - vwnd->left < 200)
    {
        vwnd->left = prev_left;
        vwnd->right = prev_right;
    }
    if (vwnd->bottom - vwnd->top < 230)
    {
        vwnd->top = prev_top;
        vwnd->bottom = prev_bottom;
    }
}

void vcoordcvt(VScreen *vscreen, COORD *x, COORD *y)
{
    LPRECT wnddim = &vscreen->wnddim;
    float aspctscl = getaspctscl(vscreen);
    // Calculate offset to virtual window is centered.
    float xoffset =
        ((wnddim->right - wnddim->left) - (vscreen->w * aspctscl)) / 2;
    float yoffset =
        ((wnddim->bottom - wnddim->top) - (vscreen->h * aspctscl)) / 2;
    // Apply aspect scale and offset to values.
    *x = ((float)(*x) * aspctscl) + xoffset;
    *y = ((float)(*y) * aspctscl) + yoffset;
}

void rcoordcvt(VScreen *vscreen, COORD *x, COORD *y)
{
    LPRECT wnddim = &vscreen->wnddim;
    float aspctscl = getaspctscl(vscreen);
    // Calculate offset to virtual window is centered.
    float xoffset =
        ((wnddim->right - wnddim->left) - (vscreen->w * aspctscl)) / 2;
    float yoffset =
        ((wnddim->bottom - wnddim->top) - (vscreen->h * aspctscl)) / 2;
    // Apply aspect scale and offset to values.
    *x = ((float)(*x - xoffset) / aspctscl);
    *y = ((float)(*y - yoffset) / aspctscl);
}

float getaspctscl(VScreen *vscreen)
{
    LPRECT wnddim = &vscreen->wnddim;
    LONG wndw = wnddim->right - wnddim->left;
    LONG wndh = wnddim->bottom - wnddim->top;
    float wndsclx = (float)wndw / (float)vscreen->w;
    float wndscly = (float)wndh / (float)vscreen->h;

    // Use smaller ratio to keep aspect ratio constant.
    return wndsclx < wndscly ? wndsclx : wndscly;
}

void refreshvwndidx(VScreen *vscreen)
{
    for (int i = 0; i < veclength(&vscreen->windows); i++)
    {
        VWnd *vwnd = vecget(&vscreen->windows, i);
        vwnd->vwndidx = i;
    }
}

void clrvscreen(VScreen *vscreen)
{
    for (int i = 0; i < veclength(&vscreen->windows); i++)
    {
        clrvwnd(vscreen, i);
    }

    clrvec(&vscreen->windows);
    free(vscreen);
}
