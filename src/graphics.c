#include "./graphics.h"

#define COLOR_BYTES 4
#define BASE_COLOR 0xd6b9ce
#define BOTTOM_COLOR 0x6b6555
#define TOP_COLOR 0xe3f7e8

void drawstylerect(HDC hdc, int x, int y, int w, int h)
{
    BITMAPINFOHEADER bih;
    BITMAPINFO bi;

    HDC memdc = CreateCompatibleDC(hdc);

    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = (long)w;
    bih.biHeight = (long)h;
    bih.biPlanes = 1;
    bih.biBitCount = 32;
    bih.biCompression = BI_RGB;

    bi.bmiHeader = bih;

    int p_bytes = (COLOR_BYTES * w) * h;

    bi.bmiHeader.biSizeImage = p_bytes;

    void *pixels = NULL;
    HBITMAP dib = CreateDIBSection(memdc, &bi, DIB_RGB_COLORS, &pixels, NULL, 0);

    HBITMAP olddib = SelectObject(memdc, dib);

    // fill entire square
    fillcolor(pixels, BASE_COLOR, p_bytes);

    // fill sides
    fillcolorvertical(pixels, TOP_COLOR, COLOR_BYTES * h, COLOR_BYTES * w);
    fillcolorvertical(pixels + COLOR_BYTES, TOP_COLOR, COLOR_BYTES * h, COLOR_BYTES * w);
    fillcolorvertical(pixels + COLOR_BYTES * (w - 1), BOTTOM_COLOR, COLOR_BYTES * h, COLOR_BYTES * w);
    fillcolorvertical(pixels + COLOR_BYTES * (w - 2), BOTTOM_COLOR, COLOR_BYTES * h, COLOR_BYTES * w);

    // fill bottom and top
    fillcolor(pixels, BOTTOM_COLOR, COLOR_BYTES * w);
    fillcolor(pixels + (COLOR_BYTES * w + 4), BOTTOM_COLOR, COLOR_BYTES * w - 4);
    fillcolor(pixels + COLOR_BYTES * w * (h - 1), TOP_COLOR, COLOR_BYTES * w);
    fillcolor(pixels + COLOR_BYTES * w * (h - 2), TOP_COLOR, COLOR_BYTES * w - 4);

    BitBlt(hdc, x, y, w, h, memdc, 0, 0, SRCCOPY);

    SelectObject(memdc, olddib);
    DeleteObject(dib);
    DeleteDC(memdc);
}
