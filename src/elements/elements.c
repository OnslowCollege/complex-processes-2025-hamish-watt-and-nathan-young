#include "./elements.h"
#include "../graphics.h"
#include "../utils.h"
#include <windows.h>

static VEC gea;

HELEMENT newelement(int top, int bottom, int left, int right)
{
    struct Element *elem = malloc(sizeof(struct Element));

    elem->left = left;
    elem->right = right;
    elem->bottom = bottom;
    elem->top = top;

    pushvec(&gea, elem);
    return veclength(&gea) - 1;
}

void rmelement(HELEMENT helem)
{
    struct Element *element = vecget(&gea, helem);
    free(element);
}

void drawelement(HDC hdc, struct VScreen *vscreen, HELEMENT helem, RECT vwnddim)
{
    struct Element *element = vecget(&gea, helem);
    short top = element->top + vwnddim.top;
    short bottom = element->bottom + vwnddim.bottom;
    short left = element->left + vwnddim.left;
    short right = element->right + vwnddim.right;

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
