#include "../elements/elements.h"
#include "../msg.h"
#include "../vwnd.h"
#include "applications.h"

typedef struct
{
    HELEMENT *text_element;
    int vwndid;
} WelcomeState;

static void draw_text_icon(VWnd *vwnd, int top, int bottom, int left, int right,
                           char *icon_name, char *name)
{
    HELEMENT *icon_element = malloc(sizeof(HELEMENT));
    *icon_element =
        newelement(top, bottom, left, right, &vwnd->left, &vwnd->top);

    HBITMAP icon_bmp = LoadBitmap(GetModuleHandle(NULL), icon_name);
    addattribute(*icon_element, HASIMAGE, (int)icon_bmp);
    pushvec(&vwnd->elements, icon_element);

    HELEMENT *text_element = malloc(sizeof(HELEMENT));
    *text_element = newelement(bottom + 5, bottom + 15, left - 10, right + 10,
                               &vwnd->left, &vwnd->top);

    char *text_str = malloc(strlen(name) + 1);
    text_str = name;
    TextInfo *text_info = malloc(sizeof(TextInfo));
    text_info->text = text_str;
    text_info->color = RGB(255, 255, 255);
    text_info->highlight = RGB(0, 0, 0);
    addattribute(*text_element, HASTEXT, (int)text_info);
    pushvec(&vwnd->elements, text_element);
}

Application welcome;

static int messagehandler(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg,
                          MsgFlags *msgflags)
{
    VWnd *vwnd = vecget(&vscreen->windows, vwndidx);

    WelcomeState *state = vwnd->applicationstate;

    if (msg & SCALED)
    {
        Element *text_element = getelement(*state->text_element);
        text_element->bottom = vwnd->bottom - vwnd->top - 5;
        text_element->right = vwnd->right - vwnd->left - 5;
        return REDRAW;
    }
    return NO_REDRAW;
}

static void launcher(VScreen *vscreen)
{
    VWnd *vwnd = createvwnd(100, 350, 100, 600, DEFAULT);
    VWNDIDX vwndidx = bindvwnd(vscreen, vwnd);

    bindapplication(vscreen, vwndidx, &welcome);

    HELEMENT *text_elem = malloc(sizeof(HELEMENT));
    *text_elem =
        newelement(TOOLBAR_HEIGHT, vwnd->bottom - vwnd->top - 5, 5,
                   vwnd->right - vwnd->left - 5, &vwnd->left, &vwnd->top);

    COLORREF color = RGB(255, 255, 255);
    addattribute(*text_elem, HASCOLORRECT, (int)&color);

    const char *text =
        "Welcome to Kdemulate, a simulation of the K desktop environment for "
        "Linux built for Windows.\n\n\nThis simulation aims to mimic the "
        "visual "
        "identity of KDE 1.0 while also including some of the functionality of "
        "the original desktop. This includes the ability to move and resize "
        "windows, as well as some iconic applications from the original "
        "environment such as kfind and knotes.\n\n\nThe Kfind application is "
        "accessed from the taskbar icon with the magnifying glass.\n\nThe "
        "Knotes application is accessed by doubleclicking the labeled desktop "
        "icon.\n\nThis application can be reopened with the K logo on the "
        "taskbar.";
    char *text_str = strdup(text);
    TextInfo *text_info = malloc(sizeof(TextInfo));
    text_info->text = text_str;
    text_info->color = RGB(0, 0, 0);
    text_info->highlight = RGB(255, 255, 255);

    addattribute(*text_elem, HASMULTILINETEXT, (int)text_info);

    WelcomeState *state = malloc(sizeof(WelcomeState));
    state->text_element = text_elem;
    state->vwndid = vwnd->id;

    pushvec(&vwnd->elements, text_elem);

    vwnd->applicationstate = state;

    default_launcher(vscreen, vwnd->id);
}

static void unlauncher(VScreen *vscreen, int caller)
{
    WelcomeState *state = vwndbyid(vscreen, caller)->applicationstate;
    free(state);

    default_unlauncher(vscreen, caller);
}

Application welcome = {
    .name = "Welcome",
    .launcher = launcher,
    .unlauncher = unlauncher,
    .messagehandler = messagehandler,
};
