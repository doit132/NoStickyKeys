#include "virtual_keyboard_monitor.h"
#include "physical_keyboard_monitor.h"
#include "keyboard_logger.h"
#include <thread>
#include <windows.h>
#include <iostream>
#include <csignal>

using namespace keyboard;

// 全局变量用于处理 Ctrl+C
bool g_running = true;

void SignalHandler(int signal) {
    if (signal == SIGINT) {
        g_running = false;
    }
}

int main() {
    // 设置 Ctrl+C 处理器
    signal(SIGINT, SignalHandler);
    
    VirtualKeyboardMonitor virtualMonitor;
    PhysicalKeyboardMonitor physicalMonitor;
    
    // 设置虚拟按键监听器的状态检查回调
    virtualMonitor.SetKeyStateChecker([&physicalMonitor](DWORD vkCode) {
        return physicalMonitor.IsKeyDown(vkCode);
    });
    
    if (!virtualMonitor.Start() || !physicalMonitor.Start()) {
        Logger::LogError("Failed to start keyboard monitors!");
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