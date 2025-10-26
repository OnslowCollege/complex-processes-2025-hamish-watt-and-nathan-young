#include "../elements/elements.h"
#include "../msg.h"
#include "../vwnd.h"
#include "applications.h"

Application welcome;

static int messagehandler(VScreen *screen, VWNDIDX vwndidx, VWNDMSG msg,
                          MsgFlags *msgflags)
{
    return NO_REDRAW;
}

static void launcher(VScreen *vscreen)
{
    VWnd *vwnd = createvwnd(100, 500, 100, 700, DEFAULT);
    VWNDIDX vwndidx = bindvwnd(vscreen, vwnd);

    bindapplication(vscreen, vwndidx, &welcome);

    HELEMENT *text_elem = malloc(sizeof(HELEMENT));
    *text_elem =
        newelement(TOOLBAR_HEIGHT, vwnd->bottom - vwnd->top - TOOLBAR_HEIGHT, 5,
                   595, &vwnd->left, &vwnd->top);

    const char *text =
        "Welcome to Kdemulate, a simulation of the K desktop environment for "
        "Linux built for Windows.\n\nThis simulation aims to mimic the visual "
        "identity of KDE 1.0 while also including some of the functionality of "
        "the original desktop. This includes the ability to move and resize "
        "windows, as well as some iconic applications from the original "
        "environment such as kfind and knotes.\n\nBoth applications are "
        "accessed through the relevent taskbar icons shown here.\n(Note that "
        "the kfind application cannot be resized due to how the application "
        "works)";
    char *text_str = strdup(text);
    TextInfo *text_info = malloc(sizeof(TextInfo));
    text_info->text = text_str;
    text_info->color = RGB(0, 0, 0);
    text_info->highlight = RGB(255, 255, 255);

    addattribute(*text_elem, HASTEXT, (int)text_info);
    pushvec(&vwnd->elements, text_elem);

    default_launcher(vscreen, vwnd->id);
}

static void unlauncher(VScreen *vscreen, int caller)
{
    default_unlauncher(vscreen, caller);
}

Application welcome = {
    .name = "welcome",
    .launcher = launcher,
    .unlauncher = unlauncher,
    .messagehandler = messagehandler,
};
