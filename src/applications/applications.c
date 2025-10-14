#include "applications.h"
#include "../vwnd.h"

extern Application desktop;
extern Application taskbar;
extern Application topbar;
extern Application kfind;
extern Application notepadapp;

const Application *applications[] = {
    &topbar, &desktop, &taskbar, &kfind, &notepadapp,
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
