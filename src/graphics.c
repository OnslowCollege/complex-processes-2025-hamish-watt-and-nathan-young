#include "./graphics.h"

#define COLOR_BYTES 3
#define BASE_COLOR 0xd6b9ce
#define BOTTOM_COLOR 0x6b6555
#define TOP_COLOR 0xe3f7e8

void drawstylerect(HDC hdc, int x, int y, int w, int h)
{
    static BITMAPINFOHEADER bih;
    static BITMAPINFO bi;
    static int f_bitmapinit = 0;

    if (!f_bitmapinit)
    {
        bih.biSize = sizeof(BITMAPINFOHEADER);
        bih.biWidth = w;
        bih.biHeight = h;
        bih.biPlanes = 1;
        bih.biBitCount = 24;
        bih.biCompression = BI_RGB;

        bi.bmiHeader = bih;

        f_bitmapinit = 1;
    }

    int p_bytes = (COLOR_BYTES * w) * h;
    char *pixels = malloc(p_bytes);

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

    bi.bmiHeader.biSizeImage = p_bytes;

    StretchDIBits(hdc, x, y, w, h, 0, 0, w, h, pixels, &bi, DIB_RGB_COLORS, SRCCOPY);

    free(pixels);
}
