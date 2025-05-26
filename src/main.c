#include "./screen.h"
#include <stdio.h>
#include <windows.h>
#include <wingdi.h>

LRESULT __stdcall windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void screenmain(HWND hwnd);

static HBITMAP himage;

struct
{
    struct VScreen *screen;
    BITMAPINFO bitmap_info;
    HDC hdc;
} ctx = {0};

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    const char *CLASS_NAME = "Main Window";
    WNDCLASS wndClass = {};

    wndClass.lpfnWndProc = windowProc;
    wndClass.hInstance = hInstance;
    wndClass.lpszClassName = CLASS_NAME;

    himage = (HBITMAP)LoadImageA(hInstance, "res/test.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    if (himage == 0)
    {
        printf("Loading image failed");
    }

    ctx.bitmap_info.bmiHeader.biSize = sizeof(ctx.bitmap_info.bmiHeader);
    ctx.bitmap_info.bmiHeader.biPlanes = 1;
    ctx.bitmap_info.bmiHeader.biBitCount = 0;
    ctx.bitmap_info.bmiHeader.biCompression = BI_RGB;
    ctx.hdc = CreateCompatibleDC(0);

    RegisterClass(&wndClass);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "dwmulate", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nShowCmd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        InvalidateRect(hwnd, 0, 0);
        UpdateWindow(hwnd);
    }
}

LRESULT __stdcall windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_SIZE: {
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);

        createvscreen(width, height);
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        if (himage)
        {
            BITMAP bm;
            GetObject(himage, sizeof(bm), &bm);
            // Put an image with coords 0, 0, w, h
            // to a destination x: 10, y: 10, w: 100, h: 100
            int result = StretchDIBits(hdc, 10, 10, 100, 100, 0, 0, 100, 100, bm.bmBits, &ctx.bitmap_info,
                                       DIB_RGB_COLORS, SRCCOPY);

            if (result == 0)
            {
                printf("Drawing failed\n");
            }
        }

        EndPaint(hwnd, &ps);
    }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
