#include "../elements/elements.h"
#include "../vwnd.h"
#include "applications.h"

static void launcher(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *desktop = createvwnd(VSCREEN_TOP, VSCREEN_BOTTOM, VSCREEN_LEFT, VSCREEN_RIGHT, DESKTOP);
    bindvwnd(vscreen, desktop);
    HELEMENT *testicon = malloc(sizeof(HELEMENT));
    *testicon = newelement(100, 160, 100, 160, &desktop->left, &desktop->top);
    addattribute(*testicon, DOUBLECLICKABLE, (int)applications[3]->launcher);

    HBITMAP hbmp = LoadBitmapA(GetModuleHandle(NULL), "chrome");
    addattribute(*testicon, HASIMAGE, (int)hbmp);

    char *test_str = malloc(9);
    test_str = "test app\0";

    TextInfo *textinfo = malloc(sizeof(TextInfo));
    textinfo->text = test_str;
    textinfo->color = RGB(123, 123, 123);
    textinfo->highlight = RGB(0, 0, 0);

    addattribute(*testicon, HASTEXT, (int)textinfo);

    pushvec(&desktop->elements, testicon);
}

Application desktop = {
    .name = "desktop",
    .launcher = launcher,
};
