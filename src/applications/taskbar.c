#include "../elements/elements.h"
#include "../graphics.h"
#include "../vwnd.h"
#include "applications.h"
#include <stdio.h>

static char *get_month(int x)
{
    switch (x)
    {
    case 1:
        return "Jan";
    case 2:
        return "Feb";
    case 3:
        return "Mar";
    case 4:
        return "Apr";
    case 5:
        return "May";
    case 6:
        return "Jun";
    case 7:
        return "Jul";
    case 8:
        return "Aug";
    case 9:
        return "Sep";
    case 10:
        return "Oct";
    case 11:
        return "Nov";
    case 12:
        return "Dec";
    default:
        return "Err";
    }
}

static void launcher(VScreen *vscreen)
{
    VWnd *taskbar = createvwnd(VSCREEN_BOTTOM - TASKBAR_HEIGHT, VSCREEN_BOTTOM,
                               VSCREEN_LEFT, VSCREEN_RIGHT, TASKBAR);
    bindvwnd(vscreen, taskbar);

    // Kde start logo.
    HELEMENT *kde_logo = drawicon(taskbar, "kde_logo", 2, TASKBAR_HEIGHT - 2, 2,
                                  TASKBAR_HEIGHT - 2);
    addattribute(*kde_logo, CLICKABLE, (int)applications[5]->launcher);
    pushvec(&taskbar->elements, kde_logo);
    // Window list icon.
    HELEMENT *wnd_list =
        drawicon(taskbar, "large-window_list", 2, TASKBAR_HEIGHT - 2,
                 (TASKBAR_HEIGHT - 2), 2 * (TASKBAR_HEIGHT - 2));
    pushvec(&taskbar->elements, wnd_list);
    // Kfm Home icon.
    HELEMENT *kfm_home =
        drawicon(taskbar, "kfm_home", 2, TASKBAR_HEIGHT - 2,
                 2 * (TASKBAR_HEIGHT - 2), 3 * (TASKBAR_HEIGHT - 2));
    pushvec(&taskbar->elements, kfm_home);
    // Settings Icon.
    HELEMENT *kcontrol =
        drawicon(taskbar, "large-kcontrol", 2, TASKBAR_HEIGHT - 2,
                 3 * (TASKBAR_HEIGHT - 2), 4 * (TASKBAR_HEIGHT - 2));
    pushvec(&taskbar->elements, kcontrol);
    // Find files application.
    HBITMAP kfindbmp = LoadBitmapA(GetModuleHandle(NULL), "large-kfind");
    HELEMENT *kfind = malloc(sizeof(HELEMENT));
    *kfind =
        newelement(2, TASKBAR_HEIGHT - 2, 4 * (TASKBAR_HEIGHT - 2),
                   5 * (TASKBAR_HEIGHT - 2), &taskbar->left, &taskbar->top, 0);
    addattribute(*kfind, HASIMAGE, (int)kfindbmp);
    addattribute(*kfind, CLICKABLE, (int)applications[3]->launcher);
    pushvec(&taskbar->elements, kfind);
    // Utilities Package.
    HELEMENT *utils_package =
        drawicon(taskbar, "utils_package", 2, TASKBAR_HEIGHT - 2,
                 5 * (TASKBAR_HEIGHT - 2), 6 * (TASKBAR_HEIGHT - 2));
    pushvec(&taskbar->elements, utils_package);
<<<<<<< HEAD
    // Notepad application
    HBITMAP knotes_bmp = LoadBitmapA(GetModuleHandle(NULL), "knotes");
    HELEMENT *knotes = malloc(sizeof(HELEMENT));
    *knotes =
        newelement(2, TASKBAR_HEIGHT - 2, 6 * (TASKBAR_HEIGHT - 2),
                   7 * (TASKBAR_HEIGHT - 2), &taskbar->left, &taskbar->top, 0);
    addattribute(*knotes, HASIMAGE, (int)knotes_bmp);
    addattribute(*knotes, CLICKABLE, (int)applications[4]->launcher);
    pushvec(&taskbar->elements, knotes);
=======
>>>>>>> 21ded74544a584b57285a3e2d406b4cf939ecbf3

    SYSTEMTIME st;
    GetLocalTime(&st);
    HELEMENT *time = malloc(sizeof(HELEMENT));
    *time = newelement(2, (TASKBAR_HEIGHT / 2) - 2, -TASKBAR_HEIGHT + 2, -2,
                       &taskbar->right, &taskbar->top, 0);
    HELEMENT *date = malloc(sizeof(HELEMENT));
    *date =
        newelement((TASKBAR_HEIGHT / 2) - 2, TASKBAR_HEIGHT - 2,
                   -TASKBAR_HEIGHT + 2, -2, &taskbar->right, &taskbar->top, 0);

    char *time_str = malloc(8);
    sprintf(time_str, "%02d:%02d\0", st.wHour, st.wMinute);

    char *date_str = malloc(8);
    sprintf(date_str, "%s %02d\0", get_month(st.wMonth), st.wDay);

    TextInfo *time_textinfo = malloc(sizeof(TextInfo));
    time_textinfo->text = time_str;
    time_textinfo->color = RGB(0, 0, 0);
    time_textinfo->highlight = RGB(255, 0, 0);

    TextInfo *date_textinfo = malloc(sizeof(TextInfo));
    date_textinfo->text = date_str;
    date_textinfo->color = RGB(0, 0, 0);
    date_textinfo->highlight = RGB(255, 0, 0);

    addattribute(*time, HASTEXT, (int)time_textinfo);
    addattribute(*date, HASTEXT, (int)date_textinfo);
    pushvec(&taskbar->elements, time);
    pushvec(&taskbar->elements, date);
}

Application taskbar = {
    .name = "taskbar",
    .launcher = launcher,
};
