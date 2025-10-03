#include "../msg.h"
#include "../vwnd.h"
#include "applications.h"

static void messagehandler(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg, MsgFlags *msgflags)
{
    return;
}

Application test_app_1;

static void launcher(VScreen *vscreen)
{
    VWnd *test_vwnd_1 = createvwnd(200, 500, 300, 600, DEFAULT);
    VWNDIDX vwndidx = bindvwnd(vscreen, test_vwnd_1);

    bindapplication(vscreen, vwndidx, &test_app_1);
    default_launcher(vscreen, vwndidx);
}

static void unlauncher(VScreen *vscreen, VWNDIDX caller)
{
    default_unlauncher(vscreen, caller);
}

Application test_app_1 = {.name = "test app 1", .launcher = launcher, .messagehandler = messagehandler};
