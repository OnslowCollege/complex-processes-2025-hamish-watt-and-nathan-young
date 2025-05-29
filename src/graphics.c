#include "./graphics.h"

#define COLOR_BYTES 3
#define PADDING_BYTES 2

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
        pixels[i] = 0xF0;
        pixels[i + 1] = 0xF0;
        pixels[i + 2] = 0xF0;
        pixels[i + 3] = 0xF0;
        pixels[i + 4] = 0xF0;
        pixels[i + 5] = 0xF0;

        int end = i + (COLOR_BYTES * w - 3);

        pixels[end] = 0x40;
        pixels[end + 1] = 0x40;
        pixels[end + 2] = 0x40;
        pixels[end - 1] = 0x40;
        pixels[end - 2] = 0x40;
        pixels[end - 3] = 0x40;
    }

    // fill bottom and top
    memset(pixels, 0x40, w * COLOR_BYTES);
    memset(pixels + w * COLOR_BYTES + COLOR_BYTES, 0x40, w * COLOR_BYTES - COLOR_BYTES);
    memset(pixels + (h - 2) * (w * COLOR_BYTES), 0xF0, w * COLOR_BYTES - COLOR_BYTES);
    memset(pixels + (h - 1) * (w * COLOR_BYTES), 0xF0, w * COLOR_BYTES);

    bi.bmiHeader.biSizeImage = p_bytes;

    StretchDIBits(hdc, x * 3, y * 3, w * 3, h * 3, 0, 0, w, h, pixels, &bi, DIB_RGB_COLORS, SRCCOPY);

    free(pixels);
}
