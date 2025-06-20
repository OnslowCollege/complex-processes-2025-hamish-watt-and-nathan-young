#pragma once

#include <windows.h>

struct Toolbar;

struct VWnd
{
    int vwndidx;
    unsigned int x, y, w, h;
    unsigned int *pxarr;
    struct Toolbar *toolbar;
    enum VWndStyle *vwndstyle;
    enum VWndState *vwndstate;
};

enum VWndStyle
{
    DEFAULT,
};

enum VWndState
{
    FLOATING,
    MAXIMIZED,
    MINIMIZED,
};

struct VWnd *createvwnd(unsigned int x, unsigned int y, unsigned int w, unsigned int h, enum VWndStyle vwndstyle);

void clrvwnd(struct VWnd *vwnd);
