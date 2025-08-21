#include "applications.h"

extern Application desktop;
extern Application taskbar;
extern Application test_app_1;
extern Application test_app_2;

const Application *applications[] = {
    &desktop,
    &taskbar,
    &test_app_1,
    &test_app_2,
};
