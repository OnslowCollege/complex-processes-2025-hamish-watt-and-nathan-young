#include <stdio.h>
#include <windows.h>
#include <wingdi.h>

LRESULT __stdcall windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void screenmain(HWND hwnd);

HBITMAP g_hbmtemp;

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    const char *CLASS_NAME = "Main Window";
    WNDCLASS wndClass = {};

    wndClass.lpfnWndProc = windowProc;
    wndClass.hInstance = hInstance;
    wndClass.lpszClassName = CLASS_NAME;

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

    case WM_CREATE: {
        g_hbmtemp = LoadBitmapA(GetModuleHandle(NULL), "temp");
        if (!g_hbmtemp)
        {
            printf("Could not load bitmap");
        }
    }
        return 0;

    case WM_PAINT: {
        BITMAP bm;
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hwnd, &ps);

        HDC hdcmem = CreateCompatibleDC(hdc);
        HBITMAP hbmold = SelectObject(hdcmem, g_hbmtemp);

        GetObject(g_hbmtemp, sizeof(bm), &bm);

        int result = BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcmem, 0, 0, SRCCOPY);

        if (result == 0)
        {
            printf("Drawing failed\n");
        }

        SelectObject(hdcmem, hbmold);
        DeleteDC(hdcmem);

        EndPaint(hwnd, &ps);
    }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
