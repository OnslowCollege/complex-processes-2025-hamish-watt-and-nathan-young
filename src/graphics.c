#include "./graphics.h"

#define COLOR_BYTES 4
#define BASE_COLOR 0xd6b9ce
#define BOTTOM_COLOR 0x6b6555
#define TOP_COLOR 0xe3f7e8

#define NOT_FOCUSED_COLOR 0x40444a

#define TOOLBARSTARTCOLOR 0x0d065e
#define TOOLBARENDCOLOR 0x5648f7

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

HBITMAP createtoolbarrect(HDC hdc, int w, int h, int isfocused)
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

    if (isfocused)
    {
        // fill entire square
        fillgradient(pixels, TOOLBARSTARTCOLOR, TOOLBARENDCOLOR, p_bytes, w);

        // fill sides
        fillcolorvertical(pixels, BOTTOM_COLOR, COLOR_BYTES * h, COLOR_BYTES * w);
        fillcolorvertical(pixels + COLOR_BYTES * (w - 1), TOP_COLOR, COLOR_BYTES * h, COLOR_BYTES * w);

        // fill bottom and top
        fillcolor(pixels, TOP_COLOR, COLOR_BYTES * w);
        fillcolor(pixels + COLOR_BYTES * w * (h - 1), BOTTOM_COLOR, COLOR_BYTES * w);
    } 
    else
    {
        fillgradient(pixels, NOT_FOCUSED_COLOR, BASE_COLOR, p_bytes, w);
    }


    return dib;
}

/* Draws a HBITMAP */
void drawimage(HDC hdc, HDC memdc, HBITMAP dib, int x, int y, int w, int h)
{
    HBITMAP olddib = SelectObject(memdc, dib);

    BitBlt(hdc, x, y, w, h, memdc, 0, 0, SRCCOPY);

    SelectObject(memdc, olddib);
}

/* Draws a HBITMAP stretching to fit destination size passed in */
void drawimage_stretched(HDC hdc, HDC memdc, HBITMAP dib, int x, int y, int w, int h)
{
    BITMAP bm;
    GetObject(dib, sizeof(BITMAP), &bm);

    HBITMAP olddib = SelectObject(memdc, dib);
    StretchBlt(hdc, x, y, w, h, memdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    SelectObject(memdc, olddib);
}
