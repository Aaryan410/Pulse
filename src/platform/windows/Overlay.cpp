#include "Overlay.h"

#include <windows.h>

#include <algorithm>
#include <random>
#include <vector>
#include <cmath>
using namespace std;

namespace {
    const wchar_t* WINDOW_CLASS_NAME = L"PulseOverlay";

    POINT cursorPosition{};

    bool previousLeftButtonDown = false;

    struct Particle {
        float x;
        float y;

        float velocityX;
        float velocityY;

        float lifetime;
        float maxLifetime;

        int size;

        int red;
        int green;
        int blue;
    };

    vector<Particle> particles;

    random_device randomDevice;
    mt19937 randomGenerator(randomDevice());

    uniform_real_distribution<float> velocityDistribution(-1.5f, 1.5f);
    uniform_real_distribution<float> lifetimeDistribution(0.4f, 0.8f);
    uniform_int_distribution<int> sizeDistribution(5, 10);
    uniform_int_distribution<int> colorDistribution(150, 255);

    void spawnParticle() {
        Particle particle{};

        particle.x = static_cast<float>(cursorPosition.x);
        particle.y = static_cast<float>(cursorPosition.y);

        particle.velocityX = velocityDistribution(randomGenerator);
        particle.velocityY = velocityDistribution(randomGenerator);

        particle.lifetime = lifetimeDistribution(randomGenerator);
        particle.maxLifetime = particle.lifetime;

        particle.size = sizeDistribution(randomGenerator);

        particle.red = colorDistribution(randomGenerator);
        particle.green = colorDistribution(randomGenerator);
        particle.blue = 255;

        particles.push_back(particle);
    }

    void spawnBurst() {
        uniform_real_distribution<float> angleDistribution (
            0.0f,
            6.283185f
        );

        uniform_real_distribution<float> speedDistribution (
            3.0f,
            7.0f
        );

        for (int i = 0; i < 30; i++)
        {
            Particle particle{};

            particle.x = static_cast<float>(cursorPosition.x);
            particle.y = static_cast<float>(cursorPosition.y);

            float angle = angleDistribution(randomGenerator);
            float speed = speedDistribution(randomGenerator);

            particle.velocityX = cosf(angle) * speed;

            particle.velocityY = sinf(angle) * speed;

            particle.lifetime = lifetimeDistribution(randomGenerator);

            particle.maxLifetime = particle.lifetime;

            particle.size = sizeDistribution(randomGenerator);

            particle.red = colorDistribution(randomGenerator);

            particle.green = colorDistribution(randomGenerator);

            particle.blue = 255;

            particles.push_back(particle);
        }
    }

    void updateParticles(float deltaTime) {
        for (Particle& particle : particles) {
            particle.velocityY += 0.35f * deltaTime * 60.0f;
            particle.velocityX *= 0.98f;
            particle.velocityY *= 0.98f;

            particle.x += particle.velocityX * deltaTime * 60.0f;
            particle.y += particle.velocityY * deltaTime * 60.0f;

            particle.lifetime -= deltaTime;
        }

        particles.erase (
            remove_if (
                particles.begin(),
                particles.end(),
                [](const Particle& particle) {
                    return particle.lifetime <= 0.0f;
                }
            ),
            particles.end()
        );
    }
}


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

            HBRUSH transparentBrush =
                CreateSolidBrush(RGB(0, 0, 0));
            
            FillRect (
                deviceContext,
                &clientArea,
                transparentBrush
            );

            DeleteObject(transparentBrush);

            for (const Particle& particle : particles) {

                float lifeRatio =
                    particle.lifetime / particle.maxLifetime;

                int brightness = 
                    static_cast<int> (255.0f * lifeRatio);

                int red = static_cast<int> (
                    particle.red * lifeRatio
                );

                int green = static_cast<int> (
                    particle.green * lifeRatio
                );

                int blue = static_cast<int> (
                    particle.blue * lifeRatio
                );

                HBRUSH brush =
                    CreateSolidBrush (
                        RGB (
                            red,
                            green,
                            blue
                        )
                    );

                HBRUSH oldBrush =
                    static_cast<HBRUSH> (
                        SelectObject (deviceContext, brush)
                    );

                int halfSize = particle.size / 2;

                int left =
                    static_cast<int>(particle.x) - halfSize;

                int top =
                    static_cast<int>(particle.y) - halfSize;

                int right =
                    static_cast<int>(particle.x) + halfSize;

                int bottom =
                    static_cast<int>(particle.y) + halfSize;

                Ellipse (
                    deviceContext,
                    left,
                    top,
                    right,
                    bottom
                );

                SelectObject(deviceContext, oldBrush);
                DeleteObject(brush);
            }

            EndPaint(hwnd, &paint);

            return 0;
        }

        case WM_TIMER: {
            GetCursorPos(&cursorPosition);

            bool leftButtonDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

            if (leftButtonDown && !previousLeftButtonDown) {
                spawnBurst();
            }

            previousLeftButtonDown = leftButtonDown;

            for (int i = 0; i < 3; i++)
            {
                spawnParticle();
            }

            updateParticles(0.016f);

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
            return DefWindowProcW (
                hwnd,
                message,
                wParam,
                lParam
            );
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

    HWND window = CreateWindowExW (
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

    if (!window) {
        return false;
    }

    hwnd = window;

    SetLayeredWindowAttributes (
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