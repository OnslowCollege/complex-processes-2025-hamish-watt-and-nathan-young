#pragma once
#include "../screen.h"

#define VSCREEN_TOP 0
#define VSCREEN_LEFT 0
#define VSCREEN_BOTTOM 600
#define VSCREEN_RIGHT 800

typedef struct
{
    const char *name;

    void (*launcher)(VScreen *vscreen, VWNDIDX vwndidx);
} Application;

extern const Application *applications[4];
