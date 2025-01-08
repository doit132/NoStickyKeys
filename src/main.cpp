#include <windows.h>
#include <iostream>
#include <iomanip>
#include "keyboard_monitor.h"

int main() {
    KeyboardMonitor monitor;
    
    if (!monitor.Start()) {
        return 1;
    }

    monitor.RunMessageLoop();
    return 0;
}