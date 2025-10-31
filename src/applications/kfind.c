#include "../elements/elements.h"
#include "../graphics.h"
#include "../msg.h"
#include "../vwnd.h"
#include "applications.h"

typedef struct
{
    HELEMENT *textinput;
    int vwndid;
} KfindState;

Application kfind;

static int messagehandler(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg,
                          MsgFlags *msgflags)
{
    return NO_REDRAW;
}

static void launcher(VScreen *vscreen)
{
    VWnd *kfind_vwnd = createvwnd(200, 360, 300, 600, STATIC);
    VWNDIDX vwndidx = bindvwnd(vscreen, kfind_vwnd);

    bindapplication(vscreen, vwndidx, &kfind);

    HELEMENT *optionsbar = malloc(sizeof(HELEMENT));
    *optionsbar = newelement(TOOLBAR_HEIGHT, TOOLBAR_HEIGHT * 2, 5, 295,
                             &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);
    addattribute(*optionsbar, HASSTYLERECT, 0);
    pushvec(&kfind_vwnd->elements, optionsbar);

    HELEMENT *file_elem = malloc(sizeof(HELEMENT));
    *file_elem = newelement(TOOLBAR_HEIGHT * 1.25, TOOLBAR_HEIGHT * 1.75, 5, 50,
                            &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);

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
                            80, &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);

    char *edit_str = malloc(5);
    edit_str = "Edit\0";
    TextInfo *edit_textinfo = malloc(sizeof(TextInfo));
    edit_textinfo->text = edit_str;
    edit_textinfo->color = RGB(0, 0, 0);
    edit_textinfo->highlight = RGB(255, 0, 0);

    addattribute(*edit_elem, HASTEXT, (int)edit_textinfo);
    pushvec(&kfind_vwnd->elements, edit_elem);

    HELEMENT *options_elem = malloc(sizeof(HELEMENT));
    *options_elem =
        newelement(TOOLBAR_HEIGHT * 1.25, TOOLBAR_HEIGHT * 1.75, 65, 125,
                   &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);

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
                            295, &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);

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
                            &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);
    addattribute(*secondbar, HASSTYLERECT, 0);
    pushvec(&kfind_vwnd->elements, secondbar);

    HELEMENT *mag_elem =
        drawicon(kfind_vwnd, "kfind_mag", TOOLBAR_HEIGHT * 2.15,
                 TOOLBAR_HEIGHT * 2.85, 7, 20);
    pushvec(&kfind_vwnd->elements, mag_elem);

    HELEMENT *wheel_elem =
        drawicon(kfind_vwnd, "kfind_wheel", TOOLBAR_HEIGHT * 2.25,
                 TOOLBAR_HEIGHT * 2.75, 22, 32);
    pushvec(&kfind_vwnd->elements, wheel_elem);

    HELEMENT *stop_elem =
        drawicon(kfind_vwnd, "kfind_stop", TOOLBAR_HEIGHT * 2.15,
                 TOOLBAR_HEIGHT * 2.85, 35, 50);
    pushvec(&kfind_vwnd->elements, stop_elem);

    HELEMENT *idea_elem =
        drawicon(kfind_vwnd, "kfind_idea", TOOLBAR_HEIGHT * 2.15,
                 TOOLBAR_HEIGHT * 2.85, 60, 73);
    pushvec(&kfind_vwnd->elements, idea_elem);

    HELEMENT *square_elem =
        drawicon(kfind_vwnd, "kfind_square", TOOLBAR_HEIGHT * 2.25,
                 TOOLBAR_HEIGHT * 2.75, 77, 87);
    pushvec(&kfind_vwnd->elements, square_elem);

    HELEMENT *filedel_elem =
        drawicon(kfind_vwnd, "kfind_filedel", TOOLBAR_HEIGHT * 2.15,
                 TOOLBAR_HEIGHT * 2.85, 95, 110);
    pushvec(&kfind_vwnd->elements, filedel_elem);

    HELEMENT *exit_elem =
        drawicon(kfind_vwnd, "kfind_exit", TOOLBAR_HEIGHT * 2.15,
                 TOOLBAR_HEIGHT * 2.85, 113, 125);
    pushvec(&kfind_vwnd->elements, exit_elem);

    HELEMENT *fileopen_elem =
        drawicon(kfind_vwnd, "kfind_fileopen", TOOLBAR_HEIGHT * 2.15,
                 TOOLBAR_HEIGHT * 2.85, 125, 140);
    pushvec(&kfind_vwnd->elements, fileopen_elem);

    HELEMENT *floppy_elem =
        drawicon(kfind_vwnd, "kfind_floppy", TOOLBAR_HEIGHT * 2.15,
                 TOOLBAR_HEIGHT * 2.85, 145, 160);
    pushvec(&kfind_vwnd->elements, floppy_elem);

    HELEMENT *book_elem =
        drawicon(kfind_vwnd, "kfind_book", TOOLBAR_HEIGHT * 2.15,
                 TOOLBAR_HEIGHT * 2.85, 165, 180);
    pushvec(&kfind_vwnd->elements, book_elem);

    HELEMENT *exit2_elem =
        drawicon(kfind_vwnd, "kfind_exit2", TOOLBAR_HEIGHT * 2.15,
                 TOOLBAR_HEIGHT * 2.85, 185, 200);
    pushvec(&kfind_vwnd->elements, exit2_elem);

    HELEMENT *mainwindow = malloc(sizeof(HELEMENT));
    *mainwindow = newelement(TOOLBAR_HEIGHT * 3 + 5,
                             (kfind_vwnd->bottom - kfind_vwnd->top) - 10, 10,
                             290, &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);
    addattribute(*mainwindow, HASSTYLERECT, 0);
    pushvec(&kfind_vwnd->elements, mainwindow);

    HELEMENT *maintitle_elem = malloc(sizeof(HELEMENT));
    *maintitle_elem =
        newelement(TOOLBAR_HEIGHT * 3.5, TOOLBAR_HEIGHT * 4, 10, 200,
                   &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);

    char *maintitle_str = malloc(42);
    maintitle_str = "Name&Location   Date Modified   Advanced\0";
    TextInfo *maintitle_text = malloc(sizeof(TextInfo));
    maintitle_text->text = maintitle_str;
    maintitle_text->color = RGB(0, 0, 0);
    maintitle_text->highlight = RGB(255, 0, 0);

    addattribute(*maintitle_elem, HASTEXT, (int)maintitle_text);
    pushvec(&kfind_vwnd->elements, maintitle_elem);

    HELEMENT *mainwindow_label_elem = malloc(sizeof(HELEMENT));
    *mainwindow_label_elem =
        newelement(TOOLBAR_HEIGHT * 5, TOOLBAR_HEIGHT * 5.75, 10, 50,
                   &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);

    char *mainwindow_label_str = malloc(7);
    mainwindow_label_str = "Named:\0";
    TextInfo *mainwindow_label_text = malloc(sizeof(TextInfo));
    mainwindow_label_text->text = mainwindow_label_str;
    mainwindow_label_text->color = RGB(0, 0, 0);
    mainwindow_label_text->highlight = RGB(255, 0, 0);

    addattribute(*mainwindow_label_elem, HASTEXT, (int)mainwindow_label_text);
    pushvec(&kfind_vwnd->elements, mainwindow_label_elem);

    HELEMENT *mainwindow_text_field = malloc(sizeof(HELEMENT));
    *mainwindow_text_field =
        newelement(TOOLBAR_HEIGHT * 5, TOOLBAR_HEIGHT * 5.75, 50, 280,
                   &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);

    addattribute(*mainwindow_text_field, HASINPUT, 0);

    KfindState *state = malloc(sizeof(KfindState));
    state->textinput = mainwindow_text_field;
    state->vwndid = kfind_vwnd->id;

    pushvec(&kfind_vwnd->elements, mainwindow_text_field);

    kfind_vwnd->applicationstate = state;

    HELEMENT *look_in_elem = malloc(sizeof(HELEMENT));
    *look_in_elem =
        newelement(TOOLBAR_HEIGHT * 6, TOOLBAR_HEIGHT * 6.75, 10, 50,
                   &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);

    char *look_in_str = malloc(9);
    look_in_str = "Look in:\0";
    TextInfo *look_in_text = malloc(sizeof(TextInfo));
    look_in_text->text = look_in_str;
    look_in_text->color = RGB(0, 0, 0);
    look_in_text->highlight = RGB(255, 0, 0);

    addattribute(*look_in_elem, HASTEXT, (int)look_in_text);
    pushvec(&kfind_vwnd->elements, look_in_elem);

    HELEMENT *path_elem = malloc(sizeof(HELEMENT));
    *path_elem = newelement(TOOLBAR_HEIGHT * 6, TOOLBAR_HEIGHT * 6.75, 50, 200,
                            &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);
    addattribute(*path_elem, HASSTYLERECT, 0);
    pushvec(&kfind_vwnd->elements, path_elem);

    HELEMENT *path_text_elem = malloc(sizeof(HELEMENT));
    *path_text_elem =
        newelement(TOOLBAR_HEIGHT * 6, TOOLBAR_HEIGHT * 6.75, 50, 200,
                   &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);

    char *path_str = malloc(16);
    path_str = "/home/kdemulate\0";
    TextInfo *path_text = malloc(sizeof(TextInfo));
    path_text->text = path_str;
    path_text->color = RGB(0, 0, 0);
    path_text->highlight = RGB(255, 0, 0);

    addattribute(*path_text_elem, HASTEXT, (int)path_text);
    pushvec(&kfind_vwnd->elements, path_text_elem);

    HELEMENT *browse_elem = malloc(sizeof(HELEMENT));
    *browse_elem =
        newelement(TOOLBAR_HEIGHT * 6, TOOLBAR_HEIGHT * 6.75, 220, 280,
                   &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);
    addattribute(*browse_elem, HASSTYLERECT, 0);
    pushvec(&kfind_vwnd->elements, browse_elem);

    HELEMENT *browse_text_elem = malloc(sizeof(HELEMENT));
    *browse_text_elem =
        newelement(TOOLBAR_HEIGHT * 6, TOOLBAR_HEIGHT * 6.75, 220, 280,
                   &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);

    char *browse_str = malloc(7);
    browse_str = "Browse\0";
    TextInfo *browse_text = malloc(sizeof(TextInfo));
    browse_text->text = browse_str;
    browse_text->color = RGB(0, 0, 0);
    browse_text->highlight = RGB(255, 0, 0);

    addattribute(*browse_text_elem, HASTEXT, (int)browse_text);
    pushvec(&kfind_vwnd->elements, browse_text_elem);

    HELEMENT *include_elem = malloc(sizeof(HELEMENT));
    *include_elem =
        newelement(TOOLBAR_HEIGHT * 7, TOOLBAR_HEIGHT * 7.75, 50, 150,
                   &kfind_vwnd->left, &kfind_vwnd->top, vwndidx);

    char *include_str = malloc(19);
    include_str = "Include subfolders\0";
    TextInfo *include_text = malloc(sizeof(TextInfo));
    include_text->text = include_str;
    include_text->color = RGB(0, 0, 0);
    include_text->highlight = RGB(255, 0, 0);

    addattribute(*include_elem, HASTEXT, (int)include_text);
    pushvec(&kfind_vwnd->elements, include_elem);

    default_launcher(vscreen, kfind_vwnd->id);
}

static void unlauncher(VScreen *vscreen, int caller)
{
    KfindState *state = vwndbyid(vscreen, caller)->applicationstate;
    rmelement(*(state->textinput));
    free(state);

    default_unlauncher(vscreen, caller);
}

Application kfind = {
    .name = "kfind",
    .launcher = launcher,
    .unlauncher = unlauncher,
    .messagehandler = messagehandler,
};
