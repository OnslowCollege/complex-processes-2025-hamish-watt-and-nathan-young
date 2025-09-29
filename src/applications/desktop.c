#include "../elements/elements.h"
#include "../vwnd.h"
#include "applications.h"

static void draw_desktop_icon(VWnd *desktop, int top, int bottom, char *icon_name, char *name)
{
    HELEMENT *icon_element = malloc(sizeof(HELEMENT));
    *icon_element = newelement(top, bottom, 10, 40, &desktop->left, &desktop->top);

    HBITMAP icon_bmp = LoadBitmap(GetModuleHandle(NULL), icon_name);
    addattribute(*icon_element, HASIMAGE, (int)icon_bmp);
    pushvec(&desktop->elements, icon_element);

    HELEMENT *text_element = malloc(sizeof(HELEMENT));
    *text_element = newelement(bottom + 5, bottom + 15, 0, 50, &desktop->left, &desktop->top);

    char *text_str = malloc(strlen(name) + 1);
    text_str = name;
    TextInfo *text_info = malloc(sizeof(TextInfo));
    text_info->text = text_str;
    text_info->color = RGB(255, 255, 255);
    text_info->highlight = RGB(0, 0, 0);
    addattribute(*text_element, HASTEXT, (int)text_info);
    pushvec(&desktop->elements, text_element);
}

static void launcher(VScreen *vscreen, VWNDIDX vwndidx)
{
    VWnd *desktop = createvwnd(VSCREEN_TOP, VSCREEN_BOTTOM, VSCREEN_LEFT, VSCREEN_RIGHT, DESKTOP);
    bindvwnd(vscreen, desktop);

    draw_desktop_icon(desktop, 35, 65, "trash", "Trash");

    draw_desktop_icon(desktop, 95, 125, "folder", "Templates");

    draw_desktop_icon(desktop, 155, 185, "folder", "Autostart");

    draw_desktop_icon(desktop, 215, 245, "cdrom", "CDROM");

    draw_desktop_icon(desktop, 275, 305, "floppy", "FLOPPY");

    draw_desktop_icon(desktop, 335, 365, "kfm_home_blue", "HOME");

    draw_desktop_icon(desktop, 395, 425, "rpm_package", "RPMmanager");

    draw_desktop_icon(desktop, 455, 485, "xkill", "XKill");
}

Application desktop = {
    .name = "desktop",
    .launcher = launcher,
};
