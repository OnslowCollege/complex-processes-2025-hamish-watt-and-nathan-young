#include "../elements/elements.h"
#include "../msg.h"
#include "../vwnd.h"
#include "applications.h"

static int messagehandler(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg,
                          MsgFlags *msgflags)
{
    return 0;
}

Application kfind;

static void launcher(VScreen *vscreen)
{
    VWnd *kfind_vwnd = createvwnd(200, 360, 300, 600, STATIC);
    VWNDIDX vwndidx = bindvwnd(vscreen, kfind_vwnd);

    bindapplication(vscreen, vwndidx, &kfind);

    HELEMENT *optionsbar = malloc(sizeof(HELEMENT));
    *optionsbar = newelement(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT * 2, 5, 295,
                             &kfind_vwnd->left, &kfind_vwnd->top);
    addattribute(*optionsbar, HASSTYLERECT, 0);
    pushvec(&kfind_vwnd->elements, optionsbar);

    HELEMENT *file_elem = malloc(sizeof(HELEMENT));
    *file_elem = newelement(TOOLBAR_HEIGHT * 1.25, TOOLBAR_HEIGHT * 1.75, 5, 50,
                            &kfind_vwnd->left, &kfind_vwnd->top);

    char *file_str = malloc(5);
    file_str = "File\0";
    TextInfo *file_textinfo = malloc(sizeof(TextInfo));
    file_textinfo->text = file_str;
    file_textinfo->color = RGB(0, 0, 0);
    file_textinfo->highlight = RGB(255, 0, 0);

    addattribute(*file_elem, HASTEXT, (int)file_textinfo);
    pushvec(&kfind_vwnd->elements, file_elem);

    HELEMENT *edit_elem = malloc(sizeof(HELEMENT));
    *edit_elem = newelement(TOOLBAR_HEIGHT * 1.25, TOOLBAR_HEIGHT * 1.75, 35,
                            80, &kfind_vwnd->left, &kfind_vwnd->top);

    char *edit_str = malloc(5);
    edit_str = "Edit\0";
    TextInfo *edit_textinfo = malloc(sizeof(TextInfo));
    edit_textinfo->text = edit_str;
    edit_textinfo->color = RGB(0, 0, 0);
    edit_textinfo->highlight = RGB(255, 0, 0);

    addattribute(*edit_elem, HASTEXT, (int)edit_textinfo);
    pushvec(&kfind_vwnd->elements, edit_elem);

    HELEMENT *options_elem = malloc(sizeof(HELEMENT));
    *options_elem = newelement(TOOLBAR_HEIGHT * 1.25, TOOLBAR_HEIGHT * 1.75, 65,
                               125, &kfind_vwnd->left, &kfind_vwnd->top);

    char *options_str = malloc(8);
    options_str = "Options\0";
    TextInfo *options_textinfo = malloc(sizeof(TextInfo));
    options_textinfo->text = options_str;
    options_textinfo->color = RGB(0, 0, 0);
    options_textinfo->highlight = RGB(255, 0, 0);

    addattribute(*options_elem, HASTEXT, (int)options_textinfo);
    pushvec(&kfind_vwnd->elements, options_elem);

    HELEMENT *help_elem = malloc(sizeof(HELEMENT));
    *help_elem = newelement(TOOLBAR_HEIGHT * 1.25, TOOLBAR_HEIGHT * 1.75, 255,
                            295, &kfind_vwnd->left, &kfind_vwnd->top);

    char *help_str = malloc(5);
    help_str = "Help\0";
    TextInfo *help_textinfo = malloc(sizeof(TextInfo));
    help_textinfo->text = help_str;
    help_textinfo->color = RGB(0, 0, 0);
    help_textinfo->highlight = RGB(255, 0, 0);

    addattribute(*help_elem, HASTEXT, (int)help_textinfo);
    pushvec(&kfind_vwnd->elements, help_elem);

    HELEMENT *secondbar = malloc(sizeof(HELEMENT));
    *secondbar = newelement(TOOLBAR_HEIGHT * 2, TOOLBAR_HEIGHT * 3, 5, 295,
                            &kfind_vwnd->left, &kfind_vwnd->top);
    addattribute(*secondbar, HASSTYLERECT, 0);
    pushvec(&kfind_vwnd->elements, secondbar);

    HELEMENT *mag_elem = malloc(sizeof(HELEMENT));
    *mag_elem = newelement(TOOLBAR_HEIGHT * 2.15, TOOLBAR_HEIGHT * 2.85, 7, 20,
                           &kfind_vwnd->left, &kfind_vwnd->top);

    HBITMAP mag_bmp = LoadBitmap(GetModuleHandle(NULL), "kfind_mag");
    addattribute(*mag_elem, HASIMAGE, (int)mag_bmp);
    pushvec(&kfind_vwnd->elements, mag_elem);

    HELEMENT *wheel_elem = malloc(sizeof(HELEMENT));
    *wheel_elem = newelement(TOOLBAR_HEIGHT * 2.25, TOOLBAR_HEIGHT * 2.75, 22,
                             32, &kfind_vwnd->left, &kfind_vwnd->top);

    HBITMAP wheel_bmp = LoadBitmap(GetModuleHandle(NULL), "kfind_wheel");
    addattribute(*wheel_elem, HASIMAGE, (int)wheel_bmp);
    pushvec(&kfind_vwnd->elements, wheel_elem);

    HELEMENT *stop_elem = malloc(sizeof(HELEMENT));
    *stop_elem = newelement(TOOLBAR_HEIGHT * 2.15, TOOLBAR_HEIGHT * 2.85, 35,
                            50, &kfind_vwnd->left, &kfind_vwnd->top);

    HBITMAP stop_bmp = LoadBitmap(GetModuleHandle(NULL), "kfind_stop");
    addattribute(*stop_elem, HASIMAGE, (int)stop_bmp);
    pushvec(&kfind_vwnd->elements, stop_elem);

    HELEMENT *idea_elem = malloc(sizeof(HELEMENT));
    *idea_elem = newelement(TOOLBAR_HEIGHT * 2.15, TOOLBAR_HEIGHT * 2.85, 60,
                            73, &kfind_vwnd->left, &kfind_vwnd->top);

    HBITMAP idea_bmp = LoadBitmap(GetModuleHandle(NULL), "kfind_idea");
    addattribute(*idea_elem, HASIMAGE, (int)idea_bmp);
    pushvec(&kfind_vwnd->elements, idea_elem);

    HELEMENT *square_elem = malloc(sizeof(HELEMENT));
    *square_elem = newelement(TOOLBAR_HEIGHT * 2.25, TOOLBAR_HEIGHT * 2.75, 77,
                              87, &kfind_vwnd->left, &kfind_vwnd->top);

    HBITMAP square_bmp = LoadBitmap(GetModuleHandle(NULL), "kfind_square");
    addattribute(*square_elem, HASIMAGE, (int)square_bmp);
    pushvec(&kfind_vwnd->elements, square_elem);

    HELEMENT *filedel_elem = malloc(sizeof(HELEMENT));
    *filedel_elem = newelement(TOOLBAR_HEIGHT * 2.15, TOOLBAR_HEIGHT * 2.85, 95,
                               110, &kfind_vwnd->left, &kfind_vwnd->top);

    HBITMAP filedel_bmp = LoadBitmap(GetModuleHandle(NULL), "kfind_filedel");
    addattribute(*filedel_elem, HASIMAGE, (int)filedel_bmp);
    pushvec(&kfind_vwnd->elements, filedel_elem);

    HELEMENT *exit_elem = malloc(sizeof(HELEMENT));
    *exit_elem = newelement(TOOLBAR_HEIGHT * 2.15, TOOLBAR_HEIGHT * 2.85, 113,
                            125, &kfind_vwnd->left, &kfind_vwnd->top);

    HBITMAP exit_bmp = LoadBitmap(GetModuleHandle(NULL), "kfind_exit");
    addattribute(*exit_elem, HASIMAGE, (int)exit_bmp);
    pushvec(&kfind_vwnd->elements, exit_elem);

    HELEMENT *fileopen_elem = malloc(sizeof(HELEMENT));
    *fileopen_elem = newelement(TOOLBAR_HEIGHT * 2.15, TOOLBAR_HEIGHT * 2.85,
                                125, 140, &kfind_vwnd->left, &kfind_vwnd->top);

    HBITMAP fileopen_bmp = LoadBitmap(GetModuleHandle(NULL), "kfind_fileopen");
    addattribute(*fileopen_elem, HASIMAGE, (int)fileopen_bmp);
    pushvec(&kfind_vwnd->elements, fileopen_elem);

    HELEMENT *floppy_elem = malloc(sizeof(HELEMENT));
    *floppy_elem = newelement(TOOLBAR_HEIGHT * 2.15, TOOLBAR_HEIGHT * 2.85, 145,
                              160, &kfind_vwnd->left, &kfind_vwnd->top);

    HBITMAP floppy_bmp = LoadBitmap(GetModuleHandle(NULL), "kfind_floppy");
    addattribute(*floppy_elem, HASIMAGE, (int)floppy_bmp);
    pushvec(&kfind_vwnd->elements, floppy_elem);

    HELEMENT *mainwindow = malloc(sizeof(HELEMENT));
    *mainwindow = newelement(TOOLBAR_HEIGHT * 3 + 5,
                             (kfind_vwnd->bottom - kfind_vwnd->top) - 10, 10,
                             290, &kfind_vwnd->left, &kfind_vwnd->top);
    addattribute(*mainwindow, HASSTYLERECT, 0);
    pushvec(&kfind_vwnd->elements, mainwindow);

    default_launcher(vscreen, kfind_vwnd->id);
}

static void unlauncher(VScreen *vscreen, int caller)
{
    default_unlauncher(vscreen, caller);
}

Application kfind = {
    .name = "kfind",
    .launcher = launcher,
    .unlauncher = unlauncher,
    .messagehandler = messagehandler,
};
