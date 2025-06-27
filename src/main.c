#include "./screen.h"
#include "./utils.h"
#include "./vwnd.h"
#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>

#define VSCREEN_TOP 0
#define VSCREEN_LEFT 0
#define VSCREEN_BOTTOM 600
#define VSCREEN_RIGHT 800

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
    static struct VScreen *vscreen = NULL;
    static VWNDIDX vwndidx;
    static POINT prevmouse = {0, 0};

    if (vscreen != NULL)
    {
        if (handlevwndmessages(vscreen) == REDRAW)
        {
            InvalidateRect(hwnd, NULL, FALSE);
        };
    }

    switch (uMsg)
    {
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    case WM_CREATE: {
        g_hbmtemp = LoadBitmapA(GetModuleHandle(NULL), "temp");
        vscreen = createvscreen(VSCREEN_RIGHT - VSCREEN_LEFT, VSCREEN_BOTTOM - VSCREEN_TOP);

        struct VWnd *test_vwnd = createvwnd(10, 50, 100, 200, DEFAULT);
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
                SCLRGN sclrgn = insclrgn(vscreen, vwnd, pt.x, pt.y, &wndrect);
                if (sclrgn)
                {
                    sendvwndevent(vscreen, i, SCALED, sclrgn);
                }
            }
        }
        return 0;
    }
    case WM_MOUSEMOVE: {
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        short dx = (short)pt.x - (short)prevmouse.x;
        short dy = (short)pt.y - (short)prevmouse.y;
        prevmouse = pt;

        long param = ((long)dx << 16) | (dy & 0xffff);

        sendglobalevent(vscreen, MOUSEMOVED, param);

        return 0;
    }
    case WM_LBUTTONUP: {
        removeevent(vscreen, SCALED);
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
        RECT wnddim;
        GetWindowRect(hwnd, &wnddim);
        int screen_top = VSCREEN_TOP;
        int screen_left = VSCREEN_LEFT;
        int screen_bottom = VSCREEN_BOTTOM;
        int screen_right = VSCREEN_RIGHT;
        vcoordcvt(vscreen, &screen_left, &screen_top, &wnddim);
        vcoordcvt(vscreen, &screen_right, &screen_bottom, &wnddim);
        RECT vscreenrect = {screen_left, screen_top, screen_right, screen_bottom};
        FillRect(ps.hdc, &vscreenrect, CreateSolidBrush(RGB(255, 255, 255))); 
        drawvwnd(vscreen, vwndidx, ps.hdc, &wnddim);

        EndPaint(hwnd, &ps);
        DeleteObject(hbrush);

        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
