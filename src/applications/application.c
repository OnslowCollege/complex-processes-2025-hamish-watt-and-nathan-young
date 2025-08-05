#include "application.h"
#include "../vwnd.h"

void launchapp(struct VScreen *vscreen, VWNDIDX idx) {
    struct VWnd *appwnd = createvwnd(100, 300, 100, 300, DEFAULT);
    bindvwnd(vscreen, appwnd);
}
