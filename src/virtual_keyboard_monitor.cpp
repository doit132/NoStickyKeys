#include "virtual_keyboard_monitor.h"
#include <iostream>
#include <iomanip>

VirtualKeyboardMonitor* VirtualKeyboardMonitor::instance = nullptr;

VirtualKeyboardMonitor::VirtualKeyboardMonitor() : keyboardHook(NULL), isRunning(false) {
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

LRESULT CALLBACK VirtualKeyboardMonitor::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && instance) {
        KBDLLHOOKSTRUCT* kbStruct = (KBDLLHOOKSTRUCT*)lParam;
        
        if (wParam == WM_KEYDOWN || wParam == WM_KEYUP || 
            wParam == WM_SYSKEYDOWN || wParam == WM_SYSKEYUP) {
            
            bool isKeyDown = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
            instance->UpdateKeyState(kbStruct->vkCode, isKeyDown);
            
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
