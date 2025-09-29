#include "../msg.h"
#include "../vwnd.h"
#include "applications.h"

#define APPLICATIONINDEX 3

static void messagehandler(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg, MsgFlags *msgflags)
{
    return;
}

Application test_app_1;

static void launcher(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *test_vwnd_1 = createvwnd(200, 500, 300, 600, DEFAULT);
    bindvwnd(vscreen, test_vwnd_1);

    bindapplication(vscreen, vwndidx, &test_app_1);
    default_launcher(vscreen, APPLICATIONINDEX);
}

Application test_app_1 = {.name = "test app 1", .launcher = launcher, .messagehandler = messagehandler};
