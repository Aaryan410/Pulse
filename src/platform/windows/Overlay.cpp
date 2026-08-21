#include "Overlay.h"
#include "../../effects/ParticleSystem.h"
#include "../../effects/EffectPresets.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

namespace {

    const wchar_t* WINDOW_CLASS_NAME = L"PulseOverlay";

    POINT cursorPosition{};
    bool previousLeftButtonDown = false;
    ParticleSystem particleSystem (
        EffectPresets::createFire()
    );

    LRESULT CALLBACK windowProcedure (
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

                HBRUSH transparentBrush = CreateSolidBrush(RGB(0, 0, 0));

                FillRect(deviceContext, &clientArea, transparentBrush);

                DeleteObject(transparentBrush);

                for (const Particle& particle : particleSystem.getParticles())
                {
                    float lifeRatio = particle.lifetime / particle.maxLifetime;

                    float progress = 1.0f - lifeRatio;

                    int red = static_cast<int> (
                        particle.startColor.red + 
                        (
                            particle.endColor.red - particle.startColor.red
                        ) * progress
                    );

                    int green = static_cast<int> (
                        particle.startColor.green +
                        (
                            particle.endColor.green - particle.startColor.green
                        ) * progress
                    );

                    int blue = static_cast<int> (
                        particle.startColor.blue +
                        (
                            particle.endColor.blue - particle.startColor.blue
                        ) * progress
                    );

                    HBRUSH brush = CreateSolidBrush(RGB(red, green, blue));

                    HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(deviceContext, brush));

                    int halfSize = particle.size / 2;

                    int left = static_cast<int>(particle.x) - halfSize;

                    int top = static_cast<int>(particle.y) - halfSize;

                    int right = static_cast<int>(particle.x) + halfSize;

                    int bottom = static_cast<int>(particle.y) + halfSize;

                    Ellipse(deviceContext, left, top, right, bottom);

                    SelectObject(deviceContext, oldBrush);

                    DeleteObject(brush);
                }

                EndPaint(hwnd, &paint);
                return 0;
            }

            case WM_TIMER: {
                GetCursorPos(&cursorPosition);

                bool leftButtonDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

                if (
                    leftButtonDown &&
                    !previousLeftButtonDown
                ) {
                    particleSystem.spawnBurst(
                        static_cast<float>(cursorPosition.x),
                        static_cast<float>(cursorPosition.y)
                    );
                }

                previousLeftButtonDown = leftButtonDown;

                for (int i = 0; i < 3; ++i) {
                    particleSystem.spawnTrailParticle(
                        static_cast<float>(cursorPosition.x),
                        static_cast<float>(cursorPosition.y)
                    );
                }

                particleSystem.update(
                    0.016f
                );

                InvalidateRect(hwnd, nullptr, TRUE);

                UpdateWindow(hwnd);

                return 0;
            }

            case WM_NCHITTEST:
                return HTTRANSPARENT;

            case WM_ERASEBKGND:
                return 1;

            case WM_DESTROY:    

                KillTimer(hwnd, 1);

                PostQuitMessage(0);

                return 0;

            default:
                return DefWindowProcW(hwnd, message, wParam, lParam);
        }
    }
}


bool Overlay::create() {
    HINSTANCE instance = GetModuleHandleW(nullptr);

    WNDCLASSEXW windowClass{};

    windowClass.cbSize = sizeof(WNDCLASSEXW);

    windowClass.hInstance = instance;

    windowClass.lpszClassName = WINDOW_CLASS_NAME;

    windowClass.lpfnWndProc = windowProcedure;

    if (!RegisterClassExW(&windowClass)) {
        return false;
    }

    HWND window = CreateWindowExW(
        WS_EX_LAYERED |
        WS_EX_TRANSPARENT |
        WS_EX_TOPMOST |
        WS_EX_NOACTIVATE,

        WINDOW_CLASS_NAME,
        L"PULSE",

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

    if (!window) 
    {
        return false;
    }

    hwnd = window;

    SetLayeredWindowAttributes(window, RGB(0, 0, 0), 0, LWA_COLORKEY);

    SetTimer(window, 1, 16, nullptr);

    ShowWindow(window, SW_SHOW);

    UpdateWindow(window);

    return true;
}

void Overlay::run() {
    MSG message{};

    while(GetMessageW(&message, nullptr, 0, 0) > 0 )
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
}