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
    enum VWndMsg *msg;
    long msgparam;
};

/* These values cannot overlap in binary so that they can be combined with a  bitwise OR */
enum VWndMsg
{
    SCALED = 0b00000001,
    MOUSEMOVED = 0b00000010,
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
