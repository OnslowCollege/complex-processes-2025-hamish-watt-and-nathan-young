#include "applications.h"
#include "../vwnd.h"

extern Application desktop;
extern Application taskbar;
extern Application topbar;
extern Application kfind;
extern Application test_app_1;

const Application *applications[] = {
    &desktop, &taskbar, &topbar, &kfind, &test_app_1,
};

void bindapplication(VScreen *vscreen, VWNDIDX vwndidx, Application *application)
{
    VWnd *vwnd = vecget(&vscreen->windows, vwndidx);
    vwnd->application = application;
}

void default_launcher(VScreen *vscreen, int vwndidx)
{
    sendglobalevent(vscreen, APPOPENED, vwndidx);
}

void default_unlauncher(VScreen *vscreen, int caller)
{
    sendglobalevent(vscreen, APPCLOSED, caller);
}
