#include "physical_keyboard_monitor.h"
#include <iostream>
#include <iomanip>

PhysicalKeyboardMonitor::PhysicalKeyboardMonitor() : context(nullptr), device(0), isRunning(false) {
    context = interception_create_context();
}

PhysicalKeyboardMonitor::~PhysicalKeyboardMonitor() {
    Stop();
    if (context) {
        interception_destroy_context(context);
    }
}

bool PhysicalKeyboardMonitor::Start() {
    if (!context) {
        std::cout << "Failed to create interception context!" << std::endl;
        return false;
    }

    interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);
    
    isRunning = true;
    std::cout << "Physical keyboard monitor started. Press Ctrl+C to exit." << std::endl;
    PrintKeyStates();
    return true;
}

void PhysicalKeyboardMonitor::Stop() {
    isRunning = false;
}

void PhysicalKeyboardMonitor::RunLoop() {
    InterceptionStroke stroke;
    
    while (isRunning) {
        device = interception_wait(context);
        
        while (interception_receive(context, device, &stroke, 1) > 0) {
            InterceptionKeyStroke& keystroke = *(InterceptionKeyStroke*)&stroke;
            
            // 输出调试信息
            std::cout << "Physical Key: code=0x" << std::hex << std::setw(2) << std::setfill('0') 
                      << keystroke.code << " state=0x" << std::hex << keystroke.state;

            UpdateKeyState(keystroke);
            
            // 继续传递按键事件
            interception_send(context, device, &stroke, 1);
        }
    }
}

void PhysicalKeyboardMonitor::PrintKeyStates() const {
    std::cout << " [Physical States: ";
    std::cout << "LCtrl:" << (physicalKeyStates.leftCtrl ? "1" : "0") << " ";
    std::cout << "RCtrl:" << (physicalKeyStates.rightCtrl ? "1" : "0") << " ";
    std::cout << "LShift:" << (physicalKeyStates.leftShift ? "1" : "0") << " ";
    std::cout << "RShift:" << (physicalKeyStates.rightShift ? "1" : "0") << " ";
    std::cout << "LAlt:" << (physicalKeyStates.leftAlt ? "1" : "0") << " ";
    std::cout << "RAlt:" << (physicalKeyStates.rightAlt ? "1" : "0");
    std::cout << "]" << std::endl;
}

void PhysicalKeyboardMonitor::UpdateKeyState(InterceptionKeyStroke& stroke) {
    bool isKeyDown = !(stroke.state & INTERCEPTION_KEY_UP);
    
    switch (stroke.code) {
        case SCANCODE_LSHIFT: physicalKeyStates.leftShift = isKeyDown; break;
        case SCANCODE_RSHIFT: physicalKeyStates.rightShift = isKeyDown; break;
        case SCANCODE_LCONTROL: physicalKeyStates.leftCtrl = isKeyDown; break;
        case SCANCODE_RCONTROL: physicalKeyStates.rightCtrl = isKeyDown; break;
        case SCANCODE_LALT: physicalKeyStates.leftAlt = isKeyDown; break;
        case SCANCODE_RALT: physicalKeyStates.rightAlt = isKeyDown; break;
    }

    if (stroke.code == SCANCODE_LSHIFT || stroke.code == SCANCODE_RSHIFT ||
        stroke.code == SCANCODE_LCONTROL || stroke.code == SCANCODE_RCONTROL ||
        stroke.code == SCANCODE_LALT || stroke.code == SCANCODE_RALT) {
        
        std::cout << " Special physical key: ";
        switch (stroke.code) {
            case SCANCODE_LSHIFT: std::cout << "Left Shift"; break;
            case SCANCODE_RSHIFT: std::cout << "Right Shift"; break;
            case SCANCODE_LCONTROL: std::cout << "Left Ctrl"; break;
            case SCANCODE_RCONTROL: std::cout << "Right Ctrl"; break;
            case SCANCODE_LALT: std::cout << "Left Alt"; break;
            case SCANCODE_RALT: std::cout << "Right Alt"; break;
        }
        std::cout << (isKeyDown ? " pressed" : " released");
        PrintKeyStates();
    }
}

bool PhysicalKeyboardMonitor::IsKeyDown(DWORD vkCode) const {
    switch (vkCode) {
        case VK_LSHIFT: return physicalKeyStates.leftShift;
        case VK_RSHIFT: return physicalKeyStates.rightShift;
        case VK_LCONTROL: return physicalKeyStates.leftCtrl;
        case VK_RCONTROL: return physicalKeyStates.rightCtrl;
        case VK_LMENU: return physicalKeyStates.leftAlt;
        case VK_RMENU: return physicalKeyStates.rightAlt;
        default: return false;
    }
}

DWORD PhysicalKeyboardMonitor::ScanCodeToVK(DWORD scanCode) {
    // 处理扩展键
    bool isExtended = (scanCode & 0xE000) == 0xE000;
    DWORD baseScanCode = scanCode & 0xFF;

    switch (baseScanCode) {
        case SCANCODE_LSHIFT: return VK_LSHIFT;
        case SCANCODE_RSHIFT: return VK_RSHIFT;
        case SCANCODE_LCONTROL: return isExtended ? VK_RCONTROL : VK_LCONTROL;
        case SCANCODE_LALT: return isExtended ? VK_RMENU : VK_LMENU;
        default: return 0;
    }
}

DWORD PhysicalKeyboardMonitor::VKToScanCode(DWORD vkCode) {
    switch (vkCode) {
        case VK_LSHIFT: return SCANCODE_LSHIFT;
        case VK_RSHIFT: return SCANCODE_RSHIFT;
        case VK_LCONTROL: return SCANCODE_LCONTROL;
        case VK_RCONTROL: return SCANCODE_RCONTROL;
        case VK_LMENU: return SCANCODE_LALT;
        case VK_RMENU: return SCANCODE_RALT;
        default: return 0;
    }
}
