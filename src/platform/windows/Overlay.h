#pragma once

class Overlay {
public:
    bool create();
    void run();

private:
    void* hwnd = nullptr;
};