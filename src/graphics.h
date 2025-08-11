#pragma once
#include <windows.h>

extern void fillcolor(void *p_colors, unsigned int color, int size);
extern void fillcolorvertical(void *p_colors, unsigned int color, int size, int rect_width);

void drawstylerect(HDC hdc, int x, int y, int w, int h);
void drawimage(HDC hdc, HBITMAP dib, int x, int y, int w, int h);
