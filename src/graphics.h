#pragma once
#include <windows.h>

extern void fillcolor(void *p_colors, unsigned int color, int size);
extern void fillcolorvertical(void *p_colors, unsigned int color, int size, int rect_width);

/* rect_width is in pixels not bytes */
extern void fillgradient(void *p_colors, unsigned int start_color, unsigned int end_color, int size, int rect_width);

HBITMAP createstylerect(HDC hdc, int w, int h);
HBITMAP createtoolbarrect(HDC hdc, int w, int h, int isfocused);

void drawimage(HDC hdc, HDC memdc, HBITMAP dib, int x, int y, int w, int h);
void drawimage_stretched(HDC hdc, HDC memdc, HBITMAP dib, int x, int y, int w, int h);
