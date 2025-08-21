#include "../elements/elements.h"
#include "../vwnd.h"
#include "applications.h"

static void launcher(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *desktop = createvwnd(VSCREEN_TOP, VSCREEN_BOTTOM, VSCREEN_LEFT, VSCREEN_RIGHT, DESKTOP);
    bindvwnd(vscreen, desktop);
    HELEMENT *testicon = malloc(sizeof(HELEMENT));
    *testicon = newelement(100, 120, 100, 120, &desktop->left, &desktop->top);
    addattribute(*testicon, DOUBLECLICKABLE, (int)applications[2]->launcher);

    pushvec(&desktop->elements, testicon);
}

Application desktop = {
    .name = "desktop",
    .launcher = launcher,
};
