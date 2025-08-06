#include "../vwnd.h"
#include "applications.h"

static void launcher(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *test_app_1 = createvwnd(100, 300, 400, 500, DEFAULT);
    bindvwnd(vscreen, test_app_1);
}

Application test_app_1 = {
    .name = "test app 1",
    .launcher = launcher,
};
