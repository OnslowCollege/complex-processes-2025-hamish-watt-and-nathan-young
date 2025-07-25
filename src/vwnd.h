#pragma once

#include <windows.h>

#define TOOLBAR_HEIGHT 20
struct Toolbar;

struct VWnd
{
    int vwndidx;
    unsigned int top, bottom, left, right;
    unsigned int *pxarr;
    struct Toolbar *toolbar;
    enum VWndStyle *vwndstyle;
    enum VWndState *vwndstate;
    enum VWndMsg *msg;

    struct MsgFlags *msgflags;
};

struct MsgFlags
{
    long mousemoved;
    long scaled;
    long windowmoved;
};

/* These values cannot overlap in binary so that they can be combined with a  bitwise OR */
enum VWndMsg
{
    SCALED = 0b00000001,
    MOUSEMOVED = 0b00000010,
    MOVED = 0b00000100
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

struct VWnd *createvwnd(unsigned int top, unsigned int bottom, unsigned int left, unsigned int right, enum VWndStyle vwndstyle);

void clrvwnd(struct VWnd *vwnd);
