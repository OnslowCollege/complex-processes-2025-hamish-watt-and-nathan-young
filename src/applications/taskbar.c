#include "../vwnd.h"
#include "../elements/elements.h"
#include "applications.h"

#define TASKBAR_HEIGHT 30

static void launcher(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *taskbar = createvwnd(VSCREEN_BOTTOM - TASKBAR_HEIGHT, VSCREEN_BOTTOM, VSCREEN_LEFT, VSCREEN_RIGHT, TASKBAR);
    bindvwnd(vscreen, taskbar);
    for (int i = 2; i < sizeof(applications) / sizeof(applications[0]); i++)
    {
        HBITMAP hbmp = LoadBitmapA(GetModuleHandle(NULL), "temp");
        HELEMENT *testicon = malloc(sizeof(HELEMENT));
        *testicon = newelement(0, (taskbar->bottom - taskbar->top), (i-2) * 40, (i-2) * 40 + 40, &taskbar->left, &taskbar->top);
        addattribute(*testicon, CLICKABLE, (int)applications[i]->launcher);
        addattribute(*testicon, HASSTYLERECT, 0);
        addattribute(*testicon, HASIMAGE, (int)hbmp);

        pushvec(&taskbar->elements, testicon);
    }
}

Application taskbar = {
    .name = "taskbar",
    .launcher = launcher,
};
