#include "../vwnd.h"
#include "applications.h"

static void launcher(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *topbar = createvwnd(VSCREEN_TOP, VSCREEN_TOP + TOPBAR_HEIGHT, VSCREEN_LEFT, VSCREEN_RIGHT, TOPBAR);
    bindvwnd(vscreen, topbar);
}

Application topbar = {
    .name = "top bar",
    .launcher = launcher,
};
