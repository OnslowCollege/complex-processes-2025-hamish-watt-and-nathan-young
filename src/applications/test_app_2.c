#include "applications.h"
#include "../vwnd.h"

static void launcher(struct VScreen *vscreen, VWNDIDX vwndidx)
{
    struct VWnd *test_app_2 = createvwnd(100, 300, 100, 300, DEFAULT);
    bindvwnd(vscreen, test_app_2);
}

Application test_app_2 = {
    .name = "test app 2",
    .launcher = launcher,
};
