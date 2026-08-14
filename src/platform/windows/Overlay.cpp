#include "Overlay.h"

#include <windows.h>

namespace {
    const wchar_t* WINDOW_CLASS_NAME = L"PulseOverlay";

    POINT cursorPosition{};

    LRESULT CALLBACK windowProcedure(
        HWND hwnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    ) {
        switch (message) {
    
            case WM_PAINT: {
                PAINTSTRUCT paint{};
                HDC deviceContext = BeginPaint(hwnd, &paint);

                RECT clientArea{};
                GetClientRect(hwnd, &clientArea);

                HBRUSH transparentBrush =
                    CreateSolidBrush(RGB(0, 0, 0));

                FillRect (
                    deviceContext,
                    &clientArea,
                    transparentBrush
                );

                DeleteObject(transparentBrush);

                HBRUSH Brush =
                    CreateSolidBrush(RGB(255, 255, 255));

                HBRUSH oldBrush =
                    static_cast<HBRUSH> (
                        SelectObject(deviceContext, Brush)
                    );

                const int radius = 25;

                Ellipse (
                    deviceContext,
                    cursorPosition.x - radius,
                    cursorPosition.y - radius,
                    cursorPosition.x + radius,
                    cursorPosition.y + radius
                );

                SelectObject(deviceContext, oldBrush);
                DeleteObject(Brush);

                EndPaint(hwnd, &paint);

                return 0;
            }
    
            case WM_TIMER: {
                GetCursorPos(&cursorPosition);
                InvalidateRect(hwnd, nullptr, TRUE);
                UpdateWindow(hwnd);
    
                return 0;
            }
    
            case WM_NCHITTEST:
                return HTTRANSPARENT;
    
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;

            case WM_ERASEBKGND:
                return 1;
    
            default:
                return DefWindowProcW(
                    hwnd,
                    message,
                    wParam,
                    lParam
                );
        }        
    }
}


bool Overlay::create() {
    HINSTANCE instance = GetModuleHandleW(nullptr);

    WNDCLASSEXW windowClass{};
    windowClass.cbSize = sizeof(WNDCLASSEXW);
    windowClass.lpfnWndProc = windowProcedure;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = WINDOW_CLASS_NAME;

    if (!RegisterClassExW(&windowClass)) {
        return false;
    }

    HWND window = CreateWindowExW(
        WS_EX_LAYERED |
        WS_EX_TRANSPARENT |
        WS_EX_TOPMOST |
        WS_EX_NOACTIVATE,

        WINDOW_CLASS_NAME,
        L"Pulse",

        WS_POPUP,

        0,
        0,
        GetSystemMetrics(SM_CXSCREEN),
        GetSystemMetrics(SM_CYSCREEN),

        nullptr,
        nullptr,
        instance,
        nullptr
    );

    if (!window) {
        return false;
    }

    hwnd = window;

    SetLayeredWindowAttributes(
        window,
        RGB(0, 0, 0),
        0,
        LWA_COLORKEY
    );

    SetTimer (
        window, 
        1,
        16,
        nullptr
    );

    ShowWindow(window, SW_SHOW);
    UpdateWindow(window);

    return true;
}


void Overlay::run() {
    MSG message{};

    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
}