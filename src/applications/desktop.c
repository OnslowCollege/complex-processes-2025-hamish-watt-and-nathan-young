#include "../elements/elements.h"
#include "../vwnd.h"
#include "applications.h"

static HELEMENT* draw_desktop_icon(VWnd *desktop, int top, int bottom,
                              char *icon_name, char *name)
{
    HELEMENT *icon_element = malloc(sizeof(HELEMENT));
    *icon_element =
        newelement(top, bottom, 10, 40, &desktop->left, &desktop->top);

    HBITMAP icon_bmp = LoadBitmap(GetModuleHandle(NULL), icon_name);
    addattribute(*icon_element, HASIMAGE, (int)icon_bmp);

    HELEMENT *text_element = malloc(sizeof(HELEMENT));
    *text_element = newelement(bottom + 5, bottom + 15, 0, 50, &desktop->left,
                               &desktop->top);

    char *text_str = malloc(strlen(name) + 1);
    text_str = name;
    TextInfo *text_info = malloc(sizeof(TextInfo));
    text_info->text = text_str;
    text_info->color = RGB(255, 255, 255);
    text_info->highlight = RGB(0, 0, 0);
    addattribute(*text_element, HASTEXT, (int)text_info);
    pushvec(&desktop->elements, text_element);
    return icon_element;
}

static void launcher(VScreen *vscreen)
{
    VWnd *desktop = createvwnd(VSCREEN_TOP, VSCREEN_BOTTOM, VSCREEN_LEFT,
                               VSCREEN_RIGHT, DESKTOP);
    bindvwnd(vscreen, desktop);

    HELEMENT *trash_icon = draw_desktop_icon(desktop, 35, 65, "trash", "Trash");
    pushvec(&desktop->elements, trash_icon);

    HELEMENT *Templates_icon = draw_desktop_icon(desktop, 95, 125, "folder", "Templates");
    pushvec(&desktop->elements, Templates_icon); 

    HELEMENT *Autostart_icon = draw_desktop_icon(desktop, 155, 185, "folder", "Autostart");
    pushvec(&desktop->elements, Autostart_icon); 

    HELEMENT *CDROM_icon = draw_desktop_icon(desktop, 215, 245, "cdrom", "CDROM");
    pushvec(&desktop->elements, CDROM_icon); 

    HELEMENT *FLOPPY_icon = draw_desktop_icon(desktop, 275, 305, "floppy", "FLOPPY");
    pushvec(&desktop->elements, FLOPPY_icon); 

    HELEMENT *HOME_icon = draw_desktop_icon(desktop, 335, 365, "kfm_home_blue", "HOME");
    pushvec(&desktop->elements, HOME_icon); 

    HELEMENT *RPMmanager_icon = draw_desktop_icon(desktop, 395, 425, "rpm_package", "RPMmanager");
    pushvec(&desktop->elements, RPMmanager_icon); 

    HELEMENT *XKill_icon = draw_desktop_icon(desktop, 455, 485, "xkill", "XKill");
    pushvec(&desktop->elements, XKill_icon); 

    HELEMENT *Knotes_icon = draw_desktop_icon(desktop, 515, 545, "knotes", "Knotes");
    addattribute(*Knotes_icon, DOUBLECLICKABLE, (int)applications[4]->launcher);
    pushvec(&desktop->elements, Knotes_icon);
}

Application desktop = {
    .name = "desktop",
    .launcher = launcher,
};
