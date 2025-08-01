#include "./elements.h"
#include "../graphics.h"
#include "../utils.h"
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

void executeelem(HELEMENT helem, struct VScreen *vscreen, VWNDIDX vwndidx)
{
    struct Element *element = vecget(&gea, helem);

    if (element->behavior == NULL)
    {
        return;
    }

    element->behavior(vscreen, vwndidx);
}

int ptinelem(HELEMENT helem, short x, short y)
{
    struct Element *element = vecget(&gea, helem);

    RECT elemrect = {0};
    elemrect.top = element->top + *element->anchory;
    elemrect.bottom = element->bottom + *element->anchory;
    elemrect.left = element->left + *element->anchorx;
    elemrect.right = element->right + *element->anchorx;

    POINT pt = {0};
    pt.x = x;
    pt.y = y;

    return PtInRect(&elemrect, pt);
}

void drawelement(HDC hdc, struct VScreen *vscreen, HELEMENT helem, LPRECT wnddim)
{
    struct Element *element = vecget(&gea, helem);
    COORD top = element->top + *element->anchory;
    COORD bottom = element->bottom + *element->anchory;
    COORD left = element->left + *element->anchorx;
    COORD right = element->right + *element->anchorx;

    vcoordcvt(vscreen, &left, &top, wnddim);
    vcoordcvt(vscreen, &right, &bottom, wnddim);

    drawstylerect(hdc, left, top, right - left, bottom - top);
}

void addclickable(HELEMENT helem, void (*behavior)(struct VScreen *vscreen, VWNDIDX vwndidx))
{
    struct Element *element = vecget(&gea, helem);
    element->attributes = element->attributes | CLICKABLE;
    element->behavior = behavior;
}

void addhoverable(HELEMENT helem, void (*behavior)(struct VScreen *vscreen, VWNDIDX vwndidx))
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

int hasattribute(HELEMENT helem, enum ElemAttribute attribute)
{
    struct Element *element = vecget(&gea, helem);
    return (element->attributes & attribute) != 0;
}
