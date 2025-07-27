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
    void (*behavior)(void);
    HBITMAP bmp;
    int left, right, bottom, top;

    unsigned int *anchorx, *anchory;
};

HELEMENT newelement(int top, int bottom, int left, int right, unsigned int *anchorx, unsigned int *anchory);
void drawelement(HDC hdc, struct VScreen *vscreen, HELEMENT helem, LPRECT wnddim);
void addattribute(HELEMENT elem, enum ElemAttribute, void *param);
