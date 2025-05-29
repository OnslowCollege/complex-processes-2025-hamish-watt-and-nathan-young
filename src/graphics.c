#include "./graphics.h"

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

    int p_bytes = (3 * w + 2) * h;
    char *pixels = malloc(p_bytes);
    memset(pixels, 0x80, p_bytes);

    bi.bmiHeader.biSizeImage = p_bytes;

    StretchDIBits(hdc, x * 4, y * 4, w * 4, h * 4, 0, 0, w, h, pixels, &bi, DIB_RGB_COLORS, SRCCOPY);

    free(pixels);
}
