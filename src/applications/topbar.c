#include "../vwnd.h"
#include "applications.h"
#include <stdio.h>
#include <stdlib.h>

Application topbar;

typedef struct
{
    VWNDIDX vwndidx;
    char application[20];
} TopbarElement;

typedef struct
{
    int currentidx;
    VEC vwnds;
} TopbarState;

TopbarState *inittopbarstate()
{
    TopbarState *topbarstate = malloc(sizeof(TopbarState));
    topbarstate->currentidx = 0;
    topbarstate->vwnds = createvec(20);

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

static void messagehandler(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg, MsgFlags *msgflags)
{
    TopbarState *state = topbar.applicationstate;

    if (msg & APPOPENED)
    {
        VWNDIDX targetidx = msgflags->appopened;
        printf("targetidx = %d\n", targetidx);

        VWnd *target = vecget(&vscreen->windows, targetidx);
        const char *name = target->application->name;

        TopbarElement *topbarelement = malloc(sizeof(TopbarElement));
        topbarelement->vwndidx = targetidx;
        memcpy(topbarelement->application, name, strlen(name) <= 20 ? strlen(name) : 20);

        pushvec(&state->vwnds, topbarelement);

        printf("Added %s to topbar array\n", name);
    }

    if (msg & APPCLOSED)
    {
        VWNDIDX targetidx = msgflags->appclosed;

        for (int i = 0; i < veclength(&state->vwnds); i++)
        {
            TopbarElement *topbarelement = vecget(&state->vwnds, i);
            if (topbarelement->vwndidx == targetidx)
            {
                rmvec(&state->vwnds, i);
                printf("removed %s from topbar array\n", topbarelement->application);
            }
        }
    }
}

static void launcher(VScreen *vscreen)
{
    VWnd *topbarwnd = createvwnd(VSCREEN_TOP, VSCREEN_TOP + TOPBAR_HEIGHT, VSCREEN_LEFT, VSCREEN_RIGHT, TOPBAR);
    VWNDIDX vwndidx = bindvwnd(vscreen, topbarwnd);

    topbar.applicationstate = inittopbarstate();

    bindapplication(vscreen, vwndidx, &topbar);
}

static void unlauncher(VScreen *vscreen, VWNDIDX vwndidx)
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
