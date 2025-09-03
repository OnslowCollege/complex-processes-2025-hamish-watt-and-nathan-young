#pragma once
#include "../screen.h"
#include "../msg.h"
#include <windows.h>

#define VSCREEN_TOP 0
#define VSCREEN_LEFT 0
#define VSCREEN_BOTTOM 600
#define VSCREEN_RIGHT 800

typedef struct
{
    const char *name;

    void (*launcher)(VScreen *vscreen, VWNDIDX vwndidx);
    void (*messagehandler)(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg, MsgFlags *msgflags);
} Application;

extern const Application *applications[5];

void bindapplication(VScreen *vscreen, VWNDIDX vwndidx, Application *application);
