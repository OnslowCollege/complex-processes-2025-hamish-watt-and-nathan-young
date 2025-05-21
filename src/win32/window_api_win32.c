#include <libloaderapi.h>
#ifdef WIN32
#include "../window_api.h"

#include <stdio.h>
#include <windows.h>

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

struct Window createWindow(const char *name)
{
    struct Window window;

    STARTUPINFO startupInfo;
    GetStartupInfoA(&startupInfo);

    WNDCLASS wndClass = {};

    HINSTANCE hInstance = GetModuleHandleA(0);

    wndClass.lpfnWndProc = windowProc;
    wndClass.hInstance = hInstance;
    wndClass.lpszClassName = name;

    RegisterClass(&wndClass);

    HWND hwnd = CreateWindowEx(0, name, "dwmulate", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        printf("ERROR: COULD NOT CREATE WINDOW");
        exit(0);
    }

    ShowWindow(hwnd, startupInfo.wShowWindow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    window.handle = &hwnd;
    return window;
}

#endif
