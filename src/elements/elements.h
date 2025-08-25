#pragma once

#include "../screen.h"
#include <windows.h>

#define HELEMENT int
#define ELEMATTRIBUTE enum ElemAttribute

enum ElemAttribute
{
    CLICKABLE = 0b00000001,
    HOVERABLE = 0b00000100,
    HASIMAGE = 0b00000010,
    DOUBLECLICKABLE = 0b00001000,
    HASSTYLERECT = 0b00010000,
};

typedef struct
{
    ELEMATTRIBUTE attributes;

    void (*behavior)(VScreen *vscreen, VWNDIDX vwndidx);

    HBITMAP bmp;
    int left, right, bottom, top;

    unsigned int *anchorx, *anchory;
} Element;

// context must be either vwnd or vscreen depending on the location of the element
HELEMENT newelement(int top, int bottom, int left, int right, unsigned int *anchorx, unsigned int *anchory);

void drawelement(HDC hdc, VScreen *vscreen, HELEMENT helem);
void addattribute(HELEMENT elem, ELEMATTRIBUTE, int param);
int hasattribute(HELEMENT elem, ELEMATTRIBUTE);
int ptinelem(HELEMENT elem, short x, short y);
void executeelem(HELEMENT elem, VScreen *vscreen, VWNDIDX vwndidx);
void rmelement(HELEMENT helem);
