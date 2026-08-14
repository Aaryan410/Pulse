#include "Overlay.h"

#include <windows.h>

namespace {
    const wchar_t* WINDOW_CLASS_NAME = L"PulseOverlay";

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
    
                HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
    
                HBRUSH oldBrush =
                    static_cast<HBRUSH>(
                        SelectObject(deviceContext, brush)
                    );
    
                Ellipse(
                    deviceContext,
                    900,
                    500,
                    950,
                    550
                );
    
                SelectObject(deviceContext, oldBrush);
                DeleteObject(brush);
    
                EndPaint(hwnd, &paint);
    
                return 0;
            }
    
            case WM_NCHITTEST:
                return HTTRANSPARENT;
    
            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
    
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