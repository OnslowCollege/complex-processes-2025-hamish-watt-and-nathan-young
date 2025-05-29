#include "./graphics.h"
#include <stdio.h>

#define COLOR_BYTES 3
#define TOP_COLOR 0xF0F0F0
#define BOTTOM_COLOR 0x404040

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

    memset(pixels, 0x80, p_bytes);

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
    for (int i = 0; i < p_bytes; i += COLOR_BYTES)
    {
        if (i < 2 * w * COLOR_BYTES && i != w * COLOR_BYTES)
        {
            pixels[i] = LOBYTE(HIWORD(BOTTOM_COLOR));
            pixels[i + 1] = HIBYTE(LOWORD(BOTTOM_COLOR));
            pixels[i + 2] = LOBYTE(LOWORD(BOTTOM_COLOR));
        }
        else if (i == 2 * w * COLOR_BYTES)
        {
            i = (h - 2) * w * COLOR_BYTES;
        }
        else if (i >= (h - 2) * w * COLOR_BYTES && i != (h - 1) * w * COLOR_BYTES - COLOR_BYTES)
        {
            pixels[i] = LOBYTE(HIWORD(TOP_COLOR));
            pixels[i + 1] = HIBYTE(LOWORD(TOP_COLOR));
            pixels[i + 2] = LOBYTE(LOWORD(TOP_COLOR));
        }
    }

    bi.bmiHeader.biSizeImage = p_bytes;

    StretchDIBits(hdc, x * 3, y * 3, w * 3, h * 3, 0, 0, w, h, pixels, &bi, DIB_RGB_COLORS, SRCCOPY);

    free(pixels);
}
