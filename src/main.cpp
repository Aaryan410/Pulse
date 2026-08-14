#include <iostream>
#include "platform/windows/Overlay.h"
using namespace std;

int main() {
    cout << "Pulse is starting...\n";

    Overlay overlay;

    if (!overlay.create()) {
        cerr << "Failed to create Pulse overlay.\n";
        return 1;
    } 

    cout << "Overlay created.\n";

    overlay.run();

    return 0;
}