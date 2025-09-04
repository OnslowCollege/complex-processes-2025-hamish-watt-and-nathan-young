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
    VWnd *kfind_vwnd = createvwnd(200, 500, 300, 600, STATIC);
    bindvwnd(vscreen, kfind_vwnd);

    bindapplication(vscreen, vwndidx, &kfind);
}

Application kfind = {
    .name = "kfind",
    .launcher = launcher,
    .messagehandler = messagehandler,
};
