#include "../elements/elements.h"
#include "../vwnd.h"
#include "applications.h"

#define TASKBAR_HEIGHT 40

static void launcher(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *taskbar = createvwnd(VSCREEN_BOTTOM - TASKBAR_HEIGHT, VSCREEN_BOTTOM, VSCREEN_LEFT, VSCREEN_RIGHT, TASKBAR);
    bindvwnd(vscreen, taskbar);

    // Kde start logo.
    HBITMAP kbmp = LoadBitmapA(GetModuleHandle(NULL), "kde_logo");
    HELEMENT *kde_logo = malloc(sizeof(HELEMENT));
    *kde_logo = newelement(0, TASKBAR_HEIGHT, 0, TASKBAR_HEIGHT, &taskbar->left, &taskbar->top);
    addattribute(*kde_logo, HASIMAGE, (int)kbmp);
    pushvec(&taskbar->elements, kde_logo);
    // Window list icon.
    HBITMAP wndlstbmp = LoadBitmapA(GetModuleHandle(NULL), "large-window_list");
    HELEMENT *wndlst = malloc(sizeof(HELEMENT));
    *wndlst = newelement(0, TASKBAR_HEIGHT, TASKBAR_HEIGHT, 2 * TASKBAR_HEIGHT, &taskbar->left, &taskbar->top);
    addattribute(*wndlst, HASIMAGE, (int)wndlstbmp);
    pushvec(&taskbar->elements, wndlst);
    // Kfm Home icon.
    HBITMAP fmhbmp = LoadBitmapA(GetModuleHandle(NULL), "kfm_home");
    HELEMENT *kfmh = malloc(sizeof(HELEMENT));
    *kfmh = newelement(0, TASKBAR_HEIGHT, 2 * TASKBAR_HEIGHT, 3 * TASKBAR_HEIGHT, &taskbar->left, &taskbar->top);
    addattribute(*kfmh, HASIMAGE, (int)fmhbmp);
    pushvec(&taskbar->elements, kfmh);
    // Settings Icon.
    HBITMAP kcbmp = LoadBitmapA(GetModuleHandle(NULL), "large-kcontrol");
    HELEMENT *kcontrol = malloc(sizeof(HELEMENT));
    *kcontrol = newelement(0, TASKBAR_HEIGHT, 3 * TASKBAR_HEIGHT, 4 * TASKBAR_HEIGHT, &taskbar->left, &taskbar->top);
    addattribute(*kcontrol, HASIMAGE, (int)kcbmp);
    pushvec(&taskbar->elements, kcontrol);
    // Find files application.
    HBITMAP ffbmp = LoadBitmapA(GetModuleHandle(NULL), "large-kfind");
    HELEMENT *kfind = malloc(sizeof(HELEMENT));
    *kfind = newelement(0, TASKBAR_HEIGHT, 4 * TASKBAR_HEIGHT, 5 * TASKBAR_HEIGHT, &taskbar->left, &taskbar->top);
    addattribute(*kfind, HASIMAGE, (int)ffbmp);
    pushvec(&taskbar->elements, kfind);
    // Utilities Package.
    HBITMAP utlbmp = LoadBitmapA(GetModuleHandle(NULL), "utils_package");
    HELEMENT *utils = malloc(sizeof(HELEMENT));
    *utils = newelement(0, TASKBAR_HEIGHT, 5 * TASKBAR_HEIGHT, 6 * TASKBAR_HEIGHT, &taskbar->left, &taskbar->top);
    addattribute(*utils, HASIMAGE, (int)utlbmp);
    pushvec(&taskbar->elements, utils);
}

Application taskbar = {
    .name = "taskbar",
    .launcher = launcher,
};
