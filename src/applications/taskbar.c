#include "../vwnd.h"
#include "applications.h"

#define VSCREEN_TOP 0
#define VSCREEN_LEFT 0
#define VSCREEN_BOTTOM 600
#define VSCREEN_RIGHT 800
#define TASKBAR_HEIGHT 30

static void launcher(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *taskbar = createvwnd(VSCREEN_BOTTOM - TASKBAR_HEIGHT, VSCREEN_BOTTOM, VSCREEN_LEFT, VSCREEN_RIGHT, TASKBAR);
    bindvwnd(vscreen, taskbar);
}

Application taskbar = {
    .name = "taskbar",
    .launcher = launcher,
};
