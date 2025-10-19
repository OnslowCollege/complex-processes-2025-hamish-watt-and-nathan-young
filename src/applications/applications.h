#pragma once
#include "../msg.h"
#include "../screen.h"
#include <windows.h>

#define VSCREEN_TOP 0
#define VSCREEN_LEFT 0
#define VSCREEN_BOTTOM 600
#define VSCREEN_RIGHT 800

typedef struct
{
    const char *name;

    void (*launcher)(VScreen *vscreen);
    void (*unlauncher)(VScreen *vscreen, int caller);
    int (*messagehandler)(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg,
                          MsgFlags *msgflags);
} Application;

extern const Application *applications[5];

void bindapplication(VScreen *vscreen, VWNDIDX vwndidx,
                     Application *application);
void default_launcher(VScreen *vscreen, int vwndidx);
void default_unlauncher(VScreen *vscreen, int caller);
