#include "../vwnd.h"
#include "applications.h"

static void messagehandler(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg, MsgFlags *msgflags)
{
    return;
}

static void launcher(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *topbar = createvwnd(VSCREEN_TOP, VSCREEN_TOP + TOPBAR_HEIGHT, VSCREEN_LEFT, VSCREEN_RIGHT, TOPBAR);
    bindvwnd(vscreen, topbar);
}

Application topbar = {
    .name = "top bar",
    .launcher = launcher,
};
