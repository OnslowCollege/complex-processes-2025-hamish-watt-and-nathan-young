#include "./msg.h"
#include "./screen.h"
#include "./utils.h"
#include "./vwnd.h"
#include "applications/applications.h"
#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>

#define CLICK_TIMER_ID 1

LRESULT __stdcall windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static VScreen *vscreen = NULL;

static COORD lastclick_x;
static COORD lastclick_y;

static BOOL isDragging = FALSE;

HBITMAP g_hbmtemp;

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    const char *CLASS_NAME = "Main Window";
    WNDCLASS wndClass = {};

    wndClass.lpfnWndProc = windowProc;
    wndClass.hInstance = hInstance;
    wndClass.lpszClassName = CLASS_NAME;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.style = CS_DBLCLKS;

    RegisterClass(&wndClass);

    SetDoubleClickTime(200);

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

        RECT wnddim;
        GetClientRect(hwnd, &wnddim);

        vscreen = createvscreen(VSCREEN_RIGHT, VSCREEN_BOTTOM, wnddim);

        applications[0]->launcher(vscreen, 0);
        applications[1]->launcher(vscreen, 0);
        applications[2]->launcher(vscreen, 0);

        VWnd *test_vwnd = createvwnd(10, 50, 100, 200, DEFAULT);
        bindvwnd(vscreen, test_vwnd);

        VWnd *test_vwnd2 = createvwnd(10, 50, 130, 230, DEFAULT);
        bindvwnd(vscreen, test_vwnd2);

        return 0;
    }
    case WM_LBUTTONDOWN: {
        SetCapture(hwnd);
        isDragging = FALSE;
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

        lastclick_x = pt.x;
        lastclick_y = pt.y;

        for (int i = veclength(&vscreen->windows) - 1; i >= 0; i--)
        {
            WNDRGN wndrgn = inwndrgn(vscreen, i, pt.x, pt.y);
            if (wndrgn)
            {
                if (!isfocused(vscreen, i))
                {
                    focusvwnd(vscreen, i);
                    InvalidateRect(hwnd, NULL, FALSE);
                    break;
                }
                break;
            }
        }

        return 0;
    }
    case WM_MOUSEMOVE: {
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        COORD x = (short)pt.x;
        COORD y = (short)pt.y;

        rcoordcvt(vscreen, &x, &y);
        long param = ((long)x << 16) | (y & 0xffff);

        if (wParam & MK_LBUTTON && !isDragging)
        {
            if (abs(x) >= 5 || abs(y) >= 5)
            {
                isDragging = TRUE;
                // Current drag code, should replace with dragstart.
                for (int i = veclength(&vscreen->windows) - 1; i >= 0; i--)
                {
                    WNDRGN wndrgn = inwndrgn(vscreen, i, pt.x, pt.y);
                    if (wndrgn)
                    {
                        if (wndrgn == MOVEREGION)
                        {
                            sendvwndevent(vscreen, i, MOVED, param);
                            break;
                        }

                        else if (wndrgn != INWINDOW)
                        {
                            sendvwndevent(vscreen, i, SCALED, wndrgn);
                            break;
                        }
                        break;
                    }
                }
            }
        }
        if (isDragging)
        {
            sendglobalevent(vscreen, MOUSEMOVED, param);
        }
        return 0;
    }
    case WM_LBUTTONUP: {
        ReleaseCapture();
        if (isDragging)
        {
            removeevent(vscreen, SCALED);
            removeevent(vscreen, MOVED);
            isDragging = FALSE;
        }
        else
        {
            SetTimer(hwnd, CLICK_TIMER_ID, GetDoubleClickTime(), NULL);
        }
        return 0;
    }
    case WM_LBUTTONDBLCLK: {
        KillTimer(hwnd, CLICK_TIMER_ID);
        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

        COORD x = pt.x;
        COORD y = pt.y;
        rcoordcvt(vscreen, &x, &y);

        long param = ((long)x << 16) | (y & 0xffff);

        sendglobalevent(vscreen, DOUBLECLICKED, param);
        return 0;
    }
    case WM_TIMER: {
        if (wParam == CLICK_TIMER_ID)
        {
            KillTimer(hwnd, CLICK_TIMER_ID);

            COORD x = lastclick_x;
            COORD y = lastclick_y;

            rcoordcvt(vscreen, &x, &y);
            long param = ((long)x << 16) | (y & 0xffff);

            for (int i = veclength(&vscreen->windows) - 1; i >= 0; i--)
            {
                WNDRGN wndrgn = inwndrgn(vscreen, i, lastclick_x, lastclick_y);
                // This is temporary to get single click to work with taskbar and desktop
                // inwndrgn automatically assumes that non default is false.
                if (wndrgn || i < 2)
                {
                    if (isfocused(vscreen, i))
                    {
                        sendvwndevent(vscreen, i, MOUSECLICKED, param);
                        break;
                    }
                }
            }
        }
    }
    case WM_SIZE: {
        RECT wnddim;
        GetClientRect(hwnd, &wnddim);
        vscreen->wnddim = wnddim;
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }
    case WM_PAINT: {
        BITMAP bm;
        PAINTSTRUCT ps;
        HBRUSH hbrush = CreateSolidBrush(0);

        BeginPaint(hwnd, &ps);
        SetBkMode(ps.hdc, TRANSPARENT);

        FillRect(ps.hdc, &ps.rcPaint, hbrush);

        for (int i = 0; i < veclength(&vscreen->windows); i++)
        {
            drawvwnd(vscreen, i, ps.hdc);
        }

        EndPaint(hwnd, &ps);
        DeleteObject(hbrush);

        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
