#include "../vwnd.h"
#include "applications.h"

static void launcher(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *test_app_2 = createvwnd(100, 300, 100, 300, DEFAULT);
    bindvwnd(vscreen, test_app_2);
}

Application test_app_2 = {
    .name = "test app 2",
    .launcher = launcher,
};
