#include "./screen.h"
#include "./utils.h"
#include "./vwnd.h"
#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
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

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "kdemulate", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
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
    }
}

LRESULT __stdcall windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static struct VScreen *vscreen;
    static VWNDIDX vwndidx;
    static VWNDIDX scalingvwnd = MAXINT;
    static POINT prevmouse = {0, 0};

    switch (uMsg)
    {
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    case WM_CREATE: {
        g_hbmtemp = LoadBitmapA(GetModuleHandle(NULL), "temp");
        vscreen = createvscreen(800, 600);

        struct VWnd *test_vwnd = createvwnd(5, 5, 200, 100, DEFAULT);
        vwndidx = bindvwnd(vscreen, test_vwnd);

        if (!g_hbmtemp)
        {
            printf("Could not load test bitmap\n");
        }

        return 0;
    }
    case WM_LBUTTONDOWN: {
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        if (DragDetect(hwnd, pt))
        {
            for (int i = 0; i < veclength(&vscreen->windows); i++)
            {
                struct VWnd *vwnd = vecget(&vscreen->windows, i);
                RECT wndrect;
                GetWindowRect(hwnd, &wndrect);
                if (insclrgn(vscreen, vwnd, pt.x, pt.y, &wndrect))
                {
                    scalingvwnd = i;
                }
            }
        }
        return 0;
    }
    case WM_MOUSEMOVE: {
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        int dx = pt.x - prevmouse.x;
        int dy = pt.y - prevmouse.y;
        prevmouse = pt;

        if (scalingvwnd != MAXINT)
        {
            scalevwnd(vscreen, scalingvwnd, dx, dy);
            InvalidateRect(hwnd, NULL, FALSE);
        }

        return 0;
    }
    case WM_LBUTTONUP: {
        scalingvwnd = MAXINT;
        return 0;
    }
    case WM_SIZE: {
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }
    case WM_PAINT: {
        BITMAP bm;
        PAINTSTRUCT ps;
        HBRUSH hbrush = CreateSolidBrush(0);

        BeginPaint(hwnd, &ps);

        FillRect(ps.hdc, &ps.rcPaint, hbrush);
        RECT rect;
        GetWindowRect(hwnd, &rect);
        updatevwnd(vscreen, vwndidx, ps.hdc, &rect);

        EndPaint(hwnd, &ps);
        DeleteObject(hbrush);

        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
