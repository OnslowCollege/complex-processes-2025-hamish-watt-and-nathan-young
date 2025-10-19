#include "../elements/elements.h"
#include "../vwnd.h"
#include "applications.h"
#include <stdio.h>
#include <stdlib.h>

#define TOPBAR_ELEMENT_WIDTH 80

Application topbar;

typedef struct
{
    int targetid;
    char application[20];
    HELEMENT *helem;
} TopbarElement;

typedef struct
{
    int currentfocusedid;
    VEC vwnds;
    VWNDIDX topbarvwndidx;
} TopbarState;

TopbarState *inittopbarstate(VWNDIDX vwndidx)
{
    TopbarState *topbarstate = malloc(sizeof(TopbarState));
    topbarstate->vwnds = createvec(20);
    topbarstate->topbarvwndidx = vwndidx;
    topbarstate->currentfocusedid = MAXINT;

    return topbarstate;
}

void clrtopbarstate(TopbarState *topbarstate)
{
    for (int i = 0; i < veclength(&topbarstate->vwnds); i++)
    {
        TopbarElement *topbarelem = vecget(&topbarstate->vwnds, i);
        rmelement(*topbarelem->helem);
        free(topbarelem->helem);
        free(topbarelem);
    }
    clrvec(&topbarstate->vwnds);

    free(topbarstate);
}

void focuselement(TopbarState *state, TopbarElement *topbarelement)
{
    state->currentfocusedid = topbarelement->targetid;
    removeattribute(*topbarelement->helem, HASSTYLERECT);
    addattribute(*topbarelement->helem, HASINVERTRECT, 0);
}

void unfocuselement(TopbarState *state, TopbarElement *topbarelement)
{
    removeattribute(*topbarelement->helem, HASINVERTRECT);
    addattribute(*topbarelement->helem, HASSTYLERECT, 0);
}

void focustargetid(TopbarState *state, int targetid)
{
    // unfocus previous

    if (state->currentfocusedid != MAXINT)
    {
        for (int i = 0; i < veclength(&state->vwnds); i++)
        {
            TopbarElement *topbarelement = vecget(&state->vwnds, i);
            if (state->currentfocusedid == topbarelement->targetid)
            {
                unfocuselement(state, topbarelement);
            }
        }
    }

    // focus new
    for (int i = 0; i < veclength(&state->vwnds); i++)
    {
        TopbarElement *topbarelement = vecget(&state->vwnds, i);
        if (topbarelement->targetid == targetid)
        {
            printf("Focussing new element %d\n", targetid);
            focuselement(state, topbarelement);
        }
    }
}

static int messagehandler(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg,
                          MsgFlags *msgflags)
{
    VWnd *topbarvwnd = vecget(&vscreen->windows, vwndidx);
    TopbarState *state = topbarvwnd->applicationstate;

    if (msg & APPOPENED)
    {
        VWNDIDX targetid = msgflags->appopened;
        printf("target: %d opened\n", targetid);

        VWnd *target = vwndbyid(vscreen, targetid);
        const char *name = target->application->name;

        TopbarElement *topbarelement = malloc(sizeof(TopbarElement));
        topbarelement->targetid = targetid;
        strcpy(topbarelement->application, name);

        int xoffset = veclength(&state->vwnds) * TOPBAR_ELEMENT_WIDTH;
        HELEMENT *elem = malloc(sizeof(HELEMENT));
        *elem = newelement(0, TOPBAR_HEIGHT, xoffset,
                           xoffset + TOPBAR_ELEMENT_WIDTH, &topbarvwnd->left,
                           &topbarvwnd->top);

        topbarelement->helem = elem;

        TextInfo *textinfo = malloc(sizeof(TextInfo));
        textinfo->text = topbarelement->application;
        textinfo->color = RGB(0, 0, 0);
        textinfo->highlight = RGB(255, 0, 0);

        addattribute(*elem, HASTEXT, (int)textinfo);
        pushvec(&topbarvwnd->elements, elem);

        pushvec(&state->vwnds, topbarelement);
        focustargetid(state, targetid);

        return REDRAW;
    }

    if (msg & APPFOCUSED)
    {
        VWNDIDX targetid = msgflags->appfocused;

        focustargetid(state, targetid);
    }

    if (msg & APPCLOSED)
    {
        VWNDIDX targetid = msgflags->appclosed;

        int found_at;
        for (int i = 0; i < veclength(&state->vwnds); i++)
        {
            TopbarElement *topbarelement = vecget(&state->vwnds, i);
            if (topbarelement->targetid == targetid)
            {
                printf("target: %d deleted\n", targetid);
                found_at = i;
                rmelement(*topbarelement->helem);
                free(topbarelement->helem);
                rmvec(&state->vwnds, i);
                rmvec(&topbarvwnd->elements, i);
                free(topbarelement);
            }
        }

        // reshuffle into correct positions
        for (int i = found_at; i < veclength(&state->vwnds); i++)
        {
            TopbarElement *topbarelement = vecget(&state->vwnds, i);
            Element *element = getelement(*topbarelement->helem);
            element->left -= TOPBAR_ELEMENT_WIDTH;
            element->right -= TOPBAR_ELEMENT_WIDTH;
        }

        return REDRAW;
    }

    return NO_REDRAW;
}

static void launcher(VScreen *vscreen)
{
    VWnd *topbarwnd = createvwnd(VSCREEN_TOP, VSCREEN_TOP + TOPBAR_HEIGHT,
                                 VSCREEN_LEFT, VSCREEN_RIGHT, TOPBAR);
    VWNDIDX vwndidx = bindvwnd(vscreen, topbarwnd);

    topbarwnd->applicationstate = inittopbarstate(vwndidx);

    bindapplication(vscreen, vwndidx, &topbar);
}

static void unlauncher(VScreen *vscreen, int caller)
{
    printf("Clearing topbar state\n");
    VWnd *vwnd = vwndbyid(vscreen, caller);
    clrtopbarstate(vwnd->applicationstate);
}

Application topbar = {
    .name = "top bar",
    .launcher = launcher,
    .unlauncher = unlauncher,
    .messagehandler = messagehandler,
};
