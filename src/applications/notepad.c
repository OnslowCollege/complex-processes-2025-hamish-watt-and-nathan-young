#include "../elements/elements.h"
#include "../msg.h"
#include "../vwnd.h"
#include "applications.h"

typedef struct
{
    HELEMENT *textinput;
    VWNDIDX vwndidx;
} NotepadState;

Application notepadapp;

static int messagehandler(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg,
                          MsgFlags *msgflags)
{
    NotepadState *state = notepadapp.applicationstate;
    VWnd *vwnd = vecget(&vscreen->windows, state->vwndidx);

    if (msg & SCALED || msg & MOVED)
    {
        Element *element = getelement(*state->textinput);
        element->bottom = vwnd->bottom - vwnd->top - TOOLBAR_HEIGHT;
        element->right = vwnd->right - vwnd->left - 5;

        // HRGN hrgn = CreateRectRgn(0, 0, 0, 0);
        // GetWindowRgn(element->hTextEdit, hrgn);

        return REDRAW;
    }

    return NO_REDRAW;
}

static void launcher(VScreen *vscreen)
{
    VWnd *notepadvwnd = createvwnd(200, 500, 300, 600, DEFAULT);
    VWNDIDX vwndidx = bindvwnd(vscreen, notepadvwnd);

    bindapplication(vscreen, vwndidx, &notepadapp);

    HELEMENT *textinput = malloc(sizeof(HELEMENT));

    *textinput = newelement(
        TOOLBAR_HEIGHT, notepadvwnd->bottom - notepadvwnd->top - TOOLBAR_HEIGHT,
        5, 295, &notepadvwnd->left, &notepadvwnd->top);

    addattribute(*textinput, HASINPUT, 0);

    NotepadState *state = malloc(sizeof(NotepadState));
    state->textinput = textinput;
    state->vwndidx = vwndidx;

    pushvec(&notepadvwnd->elements, textinput);

    notepadapp.applicationstate = state;

    default_launcher(vscreen, notepadvwnd->id);
}

static void unlauncher(VScreen *vscreen, int caller)
{
    NotepadState *state = notepadapp.applicationstate;
    free(state->textinput);
    free(state);

    default_unlauncher(vscreen, caller);
}

Application notepadapp = {.name = "Text Editor",
                          .launcher = launcher,
                          .messagehandler = messagehandler,
                          .unlauncher = unlauncher};
