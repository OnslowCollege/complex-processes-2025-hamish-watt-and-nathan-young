#include "./msg.h"
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
        initgea();

        g_hbmtemp = LoadBitmapA(GetModuleHandle(NULL), "temp");
        vscreen = createvscreen(VSCREEN_RIGHT, VSCREEN_BOTTOM);

        struct VWnd *test_vwnd = createvwnd(10, 50, 100, 200, DEFAULT);
        bindvwnd(vscreen, test_vwnd);

        if (!g_hbmtemp)
        {
            printf("Could not load test bitmap\n");
        }

        return 0;
    }
    case WM_LBUTTONDOWN: {
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

        RECT wndrect;
        GetClientRect(hwnd, &wndrect);

        COORD x = pt.x;
        COORD y = pt.y;
        rcoordcvt(vscreen, &x, &y, &wndrect);

        long param = ((long)x << 16) | (y & 0xffff);

        sendglobalevent(vscreen, MOUSECLICKED, param);

        if (DragDetect(hwnd, pt))
        {
            for (int i = 0; i < veclength(&vscreen->windows); i++)
            {
                SCLRGN sclrgn = insclrgn(vscreen, i, pt.x, pt.y, &wndrect);
                if (sclrgn)
                {
                    sendvwndevent(vscreen, i, SCALED, sclrgn);
                }

                else if (inmvrgn(vscreen, i, pt.x, pt.y, &wndrect))
                {
                    sendvwndevent(vscreen, i, MOVED, param);
                }
            }
        }
        return 0;
    }
    case WM_MOUSEMOVE: {
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        COORD x = (short)pt.x;
        COORD y = (short)pt.y;

        RECT wnddim;
        GetClientRect(hwnd, &wnddim);
        rcoordcvt(vscreen, &x, &y, &wnddim);
        long param = ((long)x << 16) | (y & 0xffff);

        sendglobalevent(vscreen, MOUSEMOVED, param);

        return 0;
    }
    case WM_LBUTTONUP: {
        removeevent(vscreen, SCALED);
        removeevent(vscreen, MOVED);
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
        GetClientRect(hwnd, &wnddim);
        COORD screen_top = VSCREEN_TOP;
        COORD screen_left = VSCREEN_LEFT;
        COORD screen_bottom = VSCREEN_BOTTOM;
        COORD screen_right = VSCREEN_RIGHT;
        vcoordcvt(vscreen, &screen_left, &screen_top, &wnddim);
        vcoordcvt(vscreen, &screen_right, &screen_bottom, &wnddim);
        RECT vscreenrect = {screen_left, screen_top, screen_right, screen_bottom};
        FillRect(ps.hdc, &vscreenrect, CreateSolidBrush(GetNearestColor(ps.hdc, RGB(81, 167, 224))));

        for (int i = 0; i < veclength(&vscreen->windows); i++)
        {
            drawvwnd(vscreen, i, ps.hdc, &wnddim);
        }

        EndPaint(hwnd, &ps);
        DeleteObject(hbrush);

        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
