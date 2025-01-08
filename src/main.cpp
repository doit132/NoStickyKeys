#include "virtual_keyboard_monitor.h"
#include "physical_keyboard_monitor.h"
#include <thread>
#include <windows.h>
#include <iostream>
#include <iomanip>

int main() {
    VirtualKeyboardMonitor virtualMonitor;
    PhysicalKeyboardMonitor physicalMonitor;
    
    if (!virtualMonitor.Start() || !physicalMonitor.Start()) {
        return 1;
    }

    // 使用独立线程运行物理键盘监听
    std::thread physicalThread([&physicalMonitor]() {
        physicalMonitor.RunLoop();
    });
    
    // 主线程运行虚拟键盘监听
    virtualMonitor.RunMessageLoop();
    
    // 清理
    physicalMonitor.Stop();
    if (physicalThread.joinable()) {
        physicalThread.join();
    }
    
    return 0;
}