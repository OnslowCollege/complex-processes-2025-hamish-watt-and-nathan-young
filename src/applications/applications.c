#include "applications.h"
#include "../vwnd.h"

extern Application desktop;
extern Application taskbar;
extern Application topbar;
extern Application test_app_1;
extern Application test_app_2;

const Application *applications[] = {
    &desktop,
    &taskbar,
    &topbar,
    &test_app_1,
    &test_app_2,
};

void bindapplication(VScreen *vscreen, VWNDIDX vwndidx, Application *application)
{
    VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    vwnd->application = application;
}
