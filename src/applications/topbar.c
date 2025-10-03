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
    int currentidx;
    VEC vwnds;
    VWNDIDX topbarvwndidx;
} TopbarState;

TopbarState *inittopbarstate(VWNDIDX vwndidx)
{
    TopbarState *topbarstate = malloc(sizeof(TopbarState));
    topbarstate->currentidx = 0;
    topbarstate->vwnds = createvec(20);
    topbarstate->topbarvwndidx = vwndidx;

    return topbarstate;
}

void clrtopbarstate(TopbarState *topbarstate)
{
    for (int i = 0; i < veclength(&topbarstate->vwnds); i++)
    {
        TopbarElement *topbarelem = vecget(&topbarstate->vwnds, i);
        free(topbarelem);
    }
    clrvec(&topbarstate->vwnds);

    free(topbarstate);
}

static int messagehandler(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg, MsgFlags *msgflags)
{
    TopbarState *state = topbar.applicationstate;
    VWnd *topbarvwnd = vecget(&vscreen->windows, state->topbarvwndidx);

    if (msg & APPOPENED)
    {
        VWNDIDX targetid = msgflags->appopened;
        printf("target: %d opened\n", targetid);

        VWnd *target = vwndbyid(vscreen, targetid);
        const char *name = target->application->name;

        TopbarElement *topbarelement = malloc(sizeof(TopbarElement));
        topbarelement->targetid = targetid;
        memcpy(topbarelement->application, name, strlen(name) <= 20 ? strlen(name) : 20);

        int xoffset = veclength(&state->vwnds) * TOPBAR_ELEMENT_WIDTH;
        HELEMENT *elem = malloc(sizeof(HELEMENT));
        *elem =
            newelement(0, TOPBAR_HEIGHT, xoffset, xoffset + TOPBAR_ELEMENT_WIDTH, &topbarvwnd->left, &topbarvwnd->top);

        topbarelement->helem = elem;

        addattribute(*elem, HASSTYLERECT, 0);

        TextInfo *textinfo = malloc(sizeof(TextInfo));
        textinfo->text = topbarelement->application;
        textinfo->color = RGB(0, 0, 0);
        textinfo->highlight = RGB(255, 0, 0);

        addattribute(*elem, HASTEXT, (int)textinfo);
        pushvec(&topbarvwnd->elements, elem);

        pushvec(&state->vwnds, topbarelement);

        return REDRAW;
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
    VWnd *topbarwnd = createvwnd(VSCREEN_TOP, VSCREEN_TOP + TOPBAR_HEIGHT, VSCREEN_LEFT, VSCREEN_RIGHT, TOPBAR);
    VWNDIDX vwndidx = bindvwnd(vscreen, topbarwnd);

    topbar.applicationstate = inittopbarstate(vwndidx);

    bindapplication(vscreen, vwndidx, &topbar);
}

static void unlauncher(VScreen *vscreen, int vwndidx)
{
    printf("Clearing topbar state\n");
    clrtopbarstate(topbar.applicationstate);
}

Application topbar = {
    .name = "top bar",
    .launcher = launcher,
    .unlauncher = unlauncher,
    .messagehandler = messagehandler,
};
