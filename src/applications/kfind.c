#include "../elements/elements.h"
#include "../msg.h"
#include "../vwnd.h"
#include "applications.h"

static void messagehandler(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg, MsgFlags *msgflags)
{
    return;
}

Application kfind;

static void launcher(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *kfind_vwnd = createvwnd(200, 360, 300, 600, STATIC);
    bindvwnd(vscreen, kfind_vwnd);

    bindapplication(vscreen, vwndidx, &kfind);

    HELEMENT *optionsbar = malloc(sizeof(HELEMENT));
    *optionsbar = newelement(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT * 2, 5, 295, &kfind_vwnd->left, &kfind_vwnd->top);
    addattribute(*optionsbar, HASSTYLERECT, 0);
    pushvec(&kfind_vwnd->elements, optionsbar);

    HELEMENT *file = malloc(sizeof(HELEMENT));
    *file = newelement(0, TOOLBAR_HEIGHT / 2, 5, 50, &kfind_vwnd->left, &kfind_vwnd->top);

    char *file_str = malloc(5);
    file_str = "File\0";
    TextInfo *filetext = malloc(sizeof(TextInfo));
    filetext->text = file_str;
    filetext->color = RGB(0, 0, 0);
    filetext->highlight = RGB(255, 0, 0);

    addattribute(*file, HASTEXT, (int)filetext);
    pushvec(&kfind_vwnd->elements, file);

    HELEMENT *secondbar = malloc(sizeof(HELEMENT));
    *secondbar = newelement(TOOLBAR_HEIGHT * 2, TOOLBAR_HEIGHT * 3, 5, 295, &kfind_vwnd->left, &kfind_vwnd->top);
    addattribute(*secondbar, HASSTYLERECT, 0);
    pushvec(&kfind_vwnd->elements, secondbar);

    HELEMENT *mainwindow = malloc(sizeof(HELEMENT));
    *mainwindow = newelement(TOOLBAR_HEIGHT * 3 + 5, (kfind_vwnd->bottom - kfind_vwnd->top) - 10, 10, 290,
                             &kfind_vwnd->left, &kfind_vwnd->top);
    addattribute(*mainwindow, HASSTYLERECT, 0);
    pushvec(&kfind_vwnd->elements, mainwindow);
}

Application kfind = {
    .name = "kfind",
    .launcher = launcher,
    .messagehandler = messagehandler,
};
