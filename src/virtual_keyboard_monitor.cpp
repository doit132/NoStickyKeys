#include "virtual_keyboard_monitor.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <unordered_map>

const int KEY_CHECK_DELAY_MS = 1000; // 延时检测时间（毫秒）

VirtualKeyboardMonitor* VirtualKeyboardMonitor::instance = nullptr;

VirtualKeyboardMonitor::VirtualKeyboardMonitor() : keyboardHook(NULL), isRunning(false), keyStateChecker(nullptr) {
    instance = this;
}

VirtualKeyboardMonitor::~VirtualKeyboardMonitor() {
    Stop();
    instance = nullptr;
}

bool VirtualKeyboardMonitor::Start() {
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (!keyboardHook) {
        std::cout << "Failed to install keyboard hook!" << std::endl;
        return false;
    }

    // 创建定时器，每100ms检查一次
    SetTimer(NULL, 0, 100, [](HWND, UINT, UINT_PTR, DWORD) {
        if (instance) {
            instance->CheckDelayedKeys();
        }
    });

    isRunning = true;
    std::cout << "Virtual keyboard hook installed. Press Ctrl+C to exit." << std::endl;
    PrintKeyStates();
    return true;
}

void VirtualKeyboardMonitor::Stop() {
    if (keyboardHook) {
        UnhookWindowsHookEx(keyboardHook);
        keyboardHook = NULL;
    }
    isRunning = false;
}

void VirtualKeyboardMonitor::RunMessageLoop() {
    MSG msg;
    while (isRunning && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void VirtualKeyboardMonitor::CheckDelayedKeys() {
    auto now = std::chrono::steady_clock::now();
    std::vector<DWORD> keysToRelease;
    
    for (const auto& pair : keyPressTime) {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - pair.second).count();
        if (duration >= KEY_CHECK_DELAY_MS) {
            if (keyStateChecker && !keyStateChecker(pair.first)) {
                keysToRelease.push_back(pair.first);
            }
        }
    }
    
    for (DWORD vkCode : keysToRelease) {
        keyPressTime.erase(vkCode);
        ForceReleaseKey(vkCode);
        // 增加一个调试输出, 输出卡住的按键, 只需要输出按键名称, 不用输出按键码, 让输出信息更清晰
        std::cout << "Delayed key released: ";
        switch (vkCode) {
            case VK_LSHIFT: std::cout << "Left Shift"; break;
            case VK_RSHIFT: std::cout << "Right Shift"; break;
            case VK_LCONTROL: std::cout << "Left Ctrl"; break;
            case VK_RCONTROL: std::cout << "Right Ctrl"; break;
            case VK_LMENU: std::cout << "Left Alt"; break;
            case VK_RMENU: std::cout << "Right Alt"; break;
        }
        std::cout << std::endl;
    }
}

LRESULT CALLBACK VirtualKeyboardMonitor::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && instance) {
        KBDLLHOOKSTRUCT* kbStruct = (KBDLLHOOKSTRUCT*)lParam;
        
        if (wParam == WM_KEYDOWN || wParam == WM_KEYUP || 
            wParam == WM_SYSKEYDOWN || wParam == WM_SYSKEYUP) {
            
            bool isKeyDown = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
            
            // 检查是否为模拟按键
            if (kbStruct->flags & LLKHF_INJECTED) {
                instance->UpdateKeyState(kbStruct->vkCode, isKeyDown);
                
                // 如果是按下状态，记录时间
                if (isKeyDown) {
                    instance->keyPressTime[kbStruct->vkCode] = std::chrono::steady_clock::now();
                } else {
                    instance->keyPressTime.erase(kbStruct->vkCode);
                }
                
                std::cout << "Virtual Key: vkCode=0x" << std::hex << std::setw(2) << std::setfill('0') 
                          << kbStruct->vkCode << " scanCode=0x" << std::hex << kbStruct->scanCode 
                          << " flags=0x" << std::hex << kbStruct->flags;

                if (kbStruct->vkCode == VK_LSHIFT || kbStruct->vkCode == VK_RSHIFT ||
                    kbStruct->vkCode == VK_LCONTROL || kbStruct->vkCode == VK_RCONTROL ||
                    kbStruct->vkCode == VK_LMENU || kbStruct->vkCode == VK_RMENU) {
                    
                    std::cout << " Special virtual key: ";
                    switch (kbStruct->vkCode) {
                        case VK_LSHIFT: std::cout << "Left Shift"; break;
                        case VK_RSHIFT: std::cout << "Right Shift"; break;
                        case VK_LCONTROL: std::cout << "Left Ctrl"; break;
                        case VK_RCONTROL: std::cout << "Right Ctrl"; break;
                        case VK_LMENU: std::cout << "Left Alt"; break;
                        case VK_RMENU: std::cout << "Right Alt"; break;
                    }
                    std::cout << (isKeyDown ? " pressed" : " released");
                    instance->PrintKeyStates();
                }
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void VirtualKeyboardMonitor::PrintKeyStates() const {
    std::cout << " [Virtual States: ";
    std::cout << "LCtrl:" << (virtualKeyStates.leftCtrl ? "1" : "0") << " ";
    std::cout << "RCtrl:" << (virtualKeyStates.rightCtrl ? "1" : "0") << " ";
    std::cout << "LShift:" << (virtualKeyStates.leftShift ? "1" : "0") << " ";
    std::cout << "RShift:" << (virtualKeyStates.rightShift ? "1" : "0") << " ";
    std::cout << "LAlt:" << (virtualKeyStates.leftAlt ? "1" : "0") << " ";
    std::cout << "RAlt:" << (virtualKeyStates.rightAlt ? "1" : "0");
    std::cout << "]" << std::endl;
}

void VirtualKeyboardMonitor::UpdateKeyState(DWORD vkCode, bool isKeyDown) {
    switch (vkCode) {
        case VK_LSHIFT: virtualKeyStates.leftShift = isKeyDown; break;
        case VK_RSHIFT: virtualKeyStates.rightShift = isKeyDown; break;
        case VK_LCONTROL: virtualKeyStates.leftCtrl = isKeyDown; break;
        case VK_RCONTROL: virtualKeyStates.rightCtrl = isKeyDown; break;
        case VK_LMENU: virtualKeyStates.leftAlt = isKeyDown; break;
        case VK_RMENU: virtualKeyStates.rightAlt = isKeyDown; break;
    }
}

void VirtualKeyboardMonitor::ForceReleaseKey(DWORD vkCode) {
    UpdateKeyState(vkCode, false);
    
    // 模拟按键释放事件
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vkCode;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
    
    PrintKeyStates();
}
