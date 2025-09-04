#include "../elements/elements.h"
#include "../vwnd.h"
#include "applications.h"

#define TASKBAR_HEIGHT 40

static void drawicon(VWnd *taskbar, char *name, int top, int bottom, int left, int right)
{
    HBITMAP bmp = LoadBitmapA(GetModuleHandle(NULL), name);
    HELEMENT *icon = malloc(sizeof(HELEMENT));
    *icon = newelement(top, bottom, left, right, &taskbar->left, &taskbar->top);
    addattribute(*icon, HASIMAGE, (int)bmp);
    pushvec(&taskbar->elements, icon);
}

static void launcher(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *taskbar = createvwnd(VSCREEN_BOTTOM - TASKBAR_HEIGHT, VSCREEN_BOTTOM, VSCREEN_LEFT, VSCREEN_RIGHT, TASKBAR);
    bindvwnd(vscreen, taskbar);

    // Kde start logo.
    drawicon(taskbar, "kde_logo", 2, TASKBAR_HEIGHT - 2, 2, TASKBAR_HEIGHT - 2);
    // Window list icon.
    drawicon(taskbar, "large-window_list", 2, TASKBAR_HEIGHT - 2, (TASKBAR_HEIGHT - 2), 2 * (TASKBAR_HEIGHT - 2));
    // Kfm Home icon.
    drawicon(taskbar, "kfm_home", 2, TASKBAR_HEIGHT - 2, 2 * (TASKBAR_HEIGHT - 2), 3 * (TASKBAR_HEIGHT - 2));
    // Settings Icon.
    drawicon(taskbar, "large-kcontrol", 2, TASKBAR_HEIGHT - 2, 3 * (TASKBAR_HEIGHT - 2), 4 * (TASKBAR_HEIGHT - 2));
    // Find files application.
    drawicon(taskbar, "large-kfind", 2, TASKBAR_HEIGHT - 2, 4 * (TASKBAR_HEIGHT - 2), 5 * (TASKBAR_HEIGHT - 2));
    // Utilities Package.
    drawicon(taskbar, "utils_package", 2, TASKBAR_HEIGHT - 2, 5 * (TASKBAR_HEIGHT - 2), 6 * (TASKBAR_HEIGHT - 2));
}

Application taskbar = {
    .name = "taskbar",
    .launcher = launcher,
};
