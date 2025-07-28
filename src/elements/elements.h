#pragma once

#include "../screen.h"
#include <windows.h>

#define HELEMENT int

enum ElemAttribute
{
    CLICKABLE = 0b00000001,
    HOVERABLE = 0b00000101,
    HASIMAGE = 0b00000010,
};

struct Element
{
    enum ElemAttribute attributes;

    void (*behavior)(struct VScreen *vscreen, VWNDIDX vwndidx);

    HBITMAP bmp;
    int left, right, bottom, top;

    unsigned int *anchorx, *anchory;
};

// context must be either vwnd or vscreen depending on the location of the element
HELEMENT newelement(int top, int bottom, int left, int right, unsigned int *anchorx, unsigned int *anchory);

void drawelement(HDC hdc, struct VScreen *vscreen, HELEMENT helem, LPRECT wnddim);
void addattribute(HELEMENT elem, enum ElemAttribute, void *param);
int hasattribute(HELEMENT elem, enum ElemAttribute);
int ptinelem(HELEMENT elem, short x, short y);
void executeelem(HELEMENT elem, struct VScreen *vscreen, VWNDIDX vwndidx);
