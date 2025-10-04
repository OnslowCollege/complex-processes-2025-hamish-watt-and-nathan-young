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
    Element *elem = malloc(sizeof(Element));

    if (elem == NULL)
    {
        fprintf(stderr, "Could not allocate new element\n");
    }

    elem->left = left;
    elem->right = right;
    elem->bottom = bottom;
    elem->top = top;
    elem->anchorx = anchorx;
    elem->anchory = anchory;
    elem->attributes = 0;

    pushvec(&gea, elem);

    return veclength(&gea) - 1;
}

Element *getelement(HELEMENT helem)
{
    Element *element = vecget(&gea, helem);
    return element;
}

void rmelement(HELEMENT helem)
{
    Element *element = vecget(&gea, helem);
    if (hasattribute(helem, HASIMAGE))
    {
        DeleteObject(element->bmp);
    }
    if (hasattribute(helem, HASTEXT))
    {
        clrtext(element->textinfo);
    }
    free(element);
}

void executeelem(HELEMENT helem, VScreen *vscreen, VWNDIDX vwndidx)
{
    Element *element = vecget(&gea, helem);

    if (element->behavior == NULL)
    {
        return;
    }

    element->behavior(vscreen, vwndidx);
}

int ptinelem(HELEMENT helem, short x, short y)
{
    Element *element = vecget(&gea, helem);

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

void drawelement(HDC hdc, VScreen *vscreen, HELEMENT helem)
{
    Element *element = vecget(&gea, helem);
    COORD top = element->top + *element->anchory;
    COORD bottom = element->bottom + *element->anchory;
    COORD left = element->left + *element->anchorx;
    COORD right = element->right + *element->anchorx;

    vcoordcvt(vscreen, &left, &top);
    vcoordcvt(vscreen, &right, &bottom);

    RECT elemrect = {left, top, right, bottom};

    HDC memdc = CreateCompatibleDC(hdc);

    if (hasattribute(helem, HASSTYLERECT))
    {
        HBITMAP stylerect_dib = createstylerect(memdc, right - left, bottom - top);
        drawimage(hdc, memdc, stylerect_dib, left, top, right - left, bottom - top);
        DeleteObject(stylerect_dib);
    }

    if (hasattribute(helem, HASINVERTRECT))
    {
        HBITMAP stylerect_dib = createinvertrect(memdc, right - left, bottom - top);
        drawimage(hdc, memdc, stylerect_dib, left, top, right - left, bottom - top);
        DeleteObject(stylerect_dib);
    }

    if (hasattribute(helem, HASIMAGE))
    {
        drawimage_stretched(hdc, memdc, element->bmp, left, top, right - left, bottom - top);
    }

    if (hasattribute(helem, HASTEXT))
    {
        // Surely we'll never have red highlight.
        /* I seriously looked through the code to find out whether we had functionality
        For no highlight just to find that you had implemented it like this ahahahahahah */
        if (element->textinfo->highlight != RGB(255, 0, 0))
        {
            SetBkMode(hdc, OPAQUE);
            SetBkColor(hdc, GetNearestColor(hdc, element->textinfo->highlight));
        }
        SetTextColor(hdc, GetNearestColor(hdc, element->textinfo->color));

        int result = DrawTextA(hdc, element->textinfo->text, -1, &elemrect, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
        SetBkMode(hdc, TRANSPARENT);
    }

    DeleteDC(memdc);
}

void addclickable(HELEMENT helem, void (*behavior)(VScreen *vscreen, VWNDIDX vwndidx))
{
    Element *element = vecget(&gea, helem);
    element->attributes = element->attributes | CLICKABLE;
    element->behavior = behavior;
}

void addhastext(HELEMENT helem, TextInfo *text)
{
    Element *element = vecget(&gea, helem);
    element->attributes = element->attributes | HASTEXT;
    element->textinfo = text;
};

void addhoverable(HELEMENT helem, void (*behavior)(VScreen *vscreen, VWNDIDX vwndidx))
{
    Element *element = vecget(&gea, helem);
    element->attributes = element->attributes | HOVERABLE;
    element->behavior = behavior;
}

void adddoubleable(HELEMENT helem, void (*behavior)(VScreen *vscreen, VWNDIDX vwndidx))
{
    Element *element = vecget(&gea, helem);
    element->attributes = element->attributes | DOUBLECLICKABLE;
    element->behavior = behavior;
}

void addhasimage(HELEMENT helem, HBITMAP bmp)
{
    Element *element = vecget(&gea, helem);
    element->attributes = element->attributes | HASIMAGE;
    element->bmp = bmp;
}

void removeattribute(HELEMENT helem, ELEMATTRIBUTE attribute)
{
    Element *element = vecget(&gea, helem);
    element->attributes = element->attributes & (~attribute);
}

void addattribute(HELEMENT helem, ELEMATTRIBUTE attribute, int param)
{
    switch (attribute)
    {
    case CLICKABLE:
        addclickable(helem, (void *)param);
        break;
    case DOUBLECLICKABLE:
        adddoubleable(helem, (void *)param);
        break;
    case HOVERABLE:
        addhoverable(helem, (void *)param);
        break;
    case HASSTYLERECT: {
        Element *element = vecget(&gea, helem);
        element->attributes = element->attributes | HASSTYLERECT;
        break;
    }
    case HASINVERTRECT: {
        Element *element = vecget(&gea, helem);
        element->attributes = element->attributes | HASINVERTRECT;
        break;
    }
    case HASIMAGE:
        addhasimage(helem, (HBITMAP)param);
        break;
    case HASTEXT:
        addhastext(helem, (TextInfo *)param);
        break;
    }
}

int hasattribute(HELEMENT helem, ELEMATTRIBUTE attribute)
{
    Element *element = vecget(&gea, helem);
    return (element->attributes & attribute) != 0;
}

void clrtext(TextInfo *text)
{
    free(text->text);
    free(text);
}
