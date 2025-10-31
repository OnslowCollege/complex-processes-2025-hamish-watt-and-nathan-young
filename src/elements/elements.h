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
    HASTEXT = 0b00100000,
    HASINVERTRECT = 0b01000000,
    HASINPUT = 0b10000000,
    HASMULTILINETEXT = 0b100000000,
    HASCOLORRECT = 0b1000000000,
};

typedef struct
{
    char *text;
    COLORREF color;
    COLORREF highlight;
} TextInfo;

typedef struct
{
    ELEMATTRIBUTE attributes;

    void (*behavior)(VScreen *vscreen, VWNDIDX vwndidx);

    HBITMAP bmp;
    int left, right, bottom, top;

    unsigned int *anchorx, *anchory;

    TextInfo *textinfo;

    COLORREF color;

    HWND hTextEdit;

    VWNDIDX vwndidx;
} Element;

// context must be either vwnd or vscreen depending on the location of the
// element
HELEMENT newelement(int top, int bottom, int left, int right,
                    unsigned int *anchorx, unsigned int *anchory,
                    VWNDIDX vwndidx);

void initelems(HWND hwnd);

void drawelement(HDC hdc, VScreen *vscreen, HELEMENT helem);
Element *getelement(HELEMENT helem);
void addattribute(HELEMENT elem, ELEMATTRIBUTE attrib, int param);
void removeattribute(HELEMENT elem, ELEMATTRIBUTE attrib);
int hasattribute(HELEMENT elem, ELEMATTRIBUTE);
int ptinelem(HELEMENT elem, short x, short y);
void executeelem(HELEMENT elem, VScreen *vscreen, VWNDIDX vwndidx);
void rmelement(HELEMENT helem);
void clrtext(TextInfo *text);
