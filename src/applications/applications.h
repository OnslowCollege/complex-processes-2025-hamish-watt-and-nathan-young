#pragma once
#include "../screen.h"

typedef struct
{
    const char *name;

    void (*launcher)(VScreen *vscreen, VWNDIDX vwndidx);
} Application;

extern const Application *applications[3];
