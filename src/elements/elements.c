#include "./elements.h"
#include "../graphics.h"
#include "../utils.h"
#include <stdio.h>
#include <windows.h>

static VEC gea;

void initgea()
{
    gea = createvec(50);
}

HELEMENT newelement(int top, int bottom, int left, int right, unsigned int *anchorx, unsigned int *anchory)
{
    struct Element *elem = malloc(sizeof(struct Element));

    elem->left = left;
    elem->right = right;
    elem->bottom = bottom;
    elem->top = top;
    elem->anchorx = anchorx;
    elem->anchory = anchory;

    pushvec(&gea, elem);
    return veclength(&gea) - 1;
}

void rmelement(HELEMENT helem)
{
    struct Element *element = vecget(&gea, helem);
    free(element);
}

void drawelement(HDC hdc, struct VScreen *vscreen, HELEMENT helem, LPRECT wnddim)
{
    struct Element *element = vecget(&gea, helem);
    short top = element->top + *element->anchory;
    short bottom = element->bottom + *element->anchory;
    short left = element->left + *element->anchorx;
    short right = element->right + *element->anchorx;

    vcoordcvt(vscreen, &left, &top, wnddim);
    vcoordcvt(vscreen, &right, &bottom, wnddim);

    printf("top: %d, bottom: %d, left: %d, right %d\n", top, bottom, left, right);

    drawstylerect(hdc, left, top, right - left, bottom - top);
}

void addclickable(HELEMENT helem, void (*behavior)(void))
{
    struct Element *element = vecget(&gea, helem);
    element->attributes = element->attributes | CLICKABLE;
    element->behavior = behavior;
}

void addhoverable(HELEMENT helem, void (*behavior)(void))
{
    struct Element *element = vecget(&gea, helem);
    element->attributes = element->attributes | HOVERABLE;
    element->behavior = behavior;
}

void addhasimage(HELEMENT helem, HBITMAP bmp)
{
    struct Element *element = vecget(&gea, helem);
    element->attributes = element->attributes | HASIMAGE;
    element->bmp = bmp;
}

void addattribute(HELEMENT helem, enum ElemAttribute attribute, void *param)
{
    switch (attribute)
    {
    case CLICKABLE:
        addclickable(helem, param);
        break;
    case HOVERABLE:
        addhoverable(helem, param);
        break;
    case HASIMAGE:
        addhasimage(helem, param);
        break;
    }
}
