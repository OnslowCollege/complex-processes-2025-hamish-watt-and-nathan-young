#include "./graphics.h"
#include <stdio.h>

#define COLOR_BYTES 3
#define BASE_COLOR 0xd6b9ce
#define BOTTOM_COLOR 0x6b6555
#define TOP_COLOR 0xe3f7e8

extern void fillcolor(void *p_colors, unsigned int color, int size);

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

    // fill left and right
    for (int i = 0; i < p_bytes; i += COLOR_BYTES * w)
    {
        pixels[i] = LOBYTE(HIWORD(TOP_COLOR));
        pixels[i + 1] = HIBYTE(LOWORD(TOP_COLOR));
        pixels[i + 2] = LOBYTE(LOWORD(TOP_COLOR));
        pixels[i + 3] = LOBYTE(HIWORD(TOP_COLOR));
        pixels[i + 4] = HIBYTE(LOWORD(TOP_COLOR));
        pixels[i + 5] = LOBYTE(LOWORD(TOP_COLOR));

        int end = i + (COLOR_BYTES * w - 3);

        pixels[end] = LOBYTE(HIWORD(BOTTOM_COLOR));
        pixels[end + 1] = HIBYTE(LOWORD(BOTTOM_COLOR));
        pixels[end + 2] = LOBYTE(LOWORD(BOTTOM_COLOR));
        pixels[end - 3] = LOBYTE(HIWORD(BOTTOM_COLOR));
        pixels[end - 2] = HIBYTE(LOWORD(BOTTOM_COLOR));
        pixels[end - 1] = LOBYTE(LOWORD(BOTTOM_COLOR));
    }

    // fill bottom and top
    fillcolor(pixels, BOTTOM_COLOR, COLOR_BYTES * w);
    fillcolor(pixels + (COLOR_BYTES * w + 4), BOTTOM_COLOR, COLOR_BYTES * w - 4);
    fillcolor(pixels + COLOR_BYTES * w * (h - 1), TOP_COLOR, COLOR_BYTES * w);
    fillcolor(pixels + COLOR_BYTES * w * (h - 2), TOP_COLOR, COLOR_BYTES * w - 4);

    bi.bmiHeader.biSizeImage = p_bytes;

    StretchDIBits(hdc, x * 2, y * 2, w * 2, h * 2, 0, 0, w, h, pixels, &bi, DIB_RGB_COLORS, SRCCOPY);

    free(pixels);
}
