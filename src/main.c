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

#define TASKBAR_HEIGHT 30

LRESULT __stdcall windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static struct VScreen *vscreen = NULL;

HBITMAP g_hbmtemp;

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    const char *CLASS_NAME = "Main Window";
    WNDCLASS wndClass = {};

    wndClass.lpfnWndProc = windowProc;
    wndClass.hInstance = hInstance;
    wndClass.lpszClassName = CLASS_NAME;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

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

        struct VWnd *desktop = createvwnd(VSCREEN_TOP, VSCREEN_BOTTOM, VSCREEN_LEFT, VSCREEN_RIGHT, DESKTOP);
        bindvwnd(vscreen, desktop);

        struct VWnd *taskbar =
            createvwnd(VSCREEN_BOTTOM - TASKBAR_HEIGHT, VSCREEN_BOTTOM, VSCREEN_LEFT, VSCREEN_RIGHT, TASKBAR);
        bindvwnd(vscreen, taskbar);

        struct VWnd *test_vwnd = createvwnd(10, 50, 100, 200, DEFAULT);
        bindvwnd(vscreen, test_vwnd);

        struct VWnd *test_vwnd2 = createvwnd(10, 50, 130, 230, DEFAULT);
        bindvwnd(vscreen, test_vwnd2);

        if (!g_hbmtemp)
        {
            printf("Could not load test bitmap\n");
        }

        return 0;
    }
    case WM_LBUTTONDOWN: {
        SetCapture(hwnd);
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

        RECT wndrect;
        GetClientRect(hwnd, &wndrect);

        COORD x = pt.x;
        COORD y = pt.y;
        rcoordcvt(vscreen, &x, &y, &wndrect);

        long param = ((long)x << 16) | (y & 0xffff);

        for (int i = veclength(&vscreen->windows) - 1; i >= 0; i--)
        {
            WNDRGN wndrgn = inwndrgn(vscreen, i, pt.x, pt.y, &wndrect);
            if (wndrgn)
            {
                if (!isfocused(vscreen, i))
                {
                    printf("focusing\n");
                    focusvwnd(vscreen, i);
                    break;
                }

                if (wndrgn == MOVEREGION)
                {
                    sendvwndevent(vscreen, i, MOVED, param);
                    break;
                }

                sendvwndevent(vscreen, i, SCALED, wndrgn);
                break;
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
        ReleaseCapture();
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

        RECT wndrect;
        GetClientRect(hwnd, &wndrect);

        COORD x = pt.x;
        COORD y = pt.y;
        rcoordcvt(vscreen, &x, &y, &wndrect);

        long param = ((long)x << 16) | (y & 0xffff);

        printf("mouse clicked\n");
        sendglobalevent(vscreen, MOUSECLICKED, param);

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
