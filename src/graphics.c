#include "./graphics.h"
#include <stdio.h>

#define COLOR_BYTES 4
#define BASE_COLOR 0xd6b9ce
#define BOTTOM_COLOR 0x6b6555
#define TOP_COLOR 0xe3f7e8

HBITMAP createstylerect(HDC hdc, int w, int h)
{
    BITMAPINFOHEADER bih;
    BITMAPINFO bi;

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
    HBITMAP dib = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &pixels, NULL, 0);

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

    return dib;
}

void drawstylerect(HDC hdc, HDC memdc, int x, int y, int w, int h, HBITMAP cache)
{
    HBITMAP dib;

    if (cache == NULL)
    {
        dib = createstylerect(memdc, w, h);
    }
    else
    {
        dib = cache;
    }

    HBITMAP olddib = SelectObject(memdc, dib);

    BitBlt(hdc, x, y, w, h, memdc, 0, 0, SRCCOPY);

    SelectObject(memdc, olddib);

    if (cache == NULL)
    {
        DeleteObject(dib);
    }

    DeleteDC(memdc);
}

void drawimage(HDC hdc, HBITMAP dib, int x, int y, int w, int h)
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

    bi.bmiHeader.biSizeImage = (COLOR_BYTES * w) * h;

    BITMAP bm;
    GetObject(dib, sizeof(BITMAP), &bm);

    HBITMAP olddib = SelectObject(memdc, dib);
    StretchBlt(hdc, x, y, w, h, memdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    SelectObject(memdc, olddib);
    DeleteDC(memdc);
}
