#include "physical_keyboard_monitor.h"
#include "keyboard_logger.h"

using namespace keyboard;

PhysicalKeyboardMonitor::PhysicalKeyboardMonitor() : context(nullptr), device(0), isRunning(false) {
}

PhysicalKeyboardMonitor::~PhysicalKeyboardMonitor() {
    Stop();
}

bool PhysicalKeyboardMonitor::Start() {
    context = interception_create_context();
    if (!context) {
        Logger::LogError("Failed to create interception context!");
        return false;
    }

    interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);
    Logger::LogMessage("Physical keyboard monitor started.");
    Logger::LogKeyStates(physicalKeyStates);
    
    isRunning = true;
    return true;
}

void PhysicalKeyboardMonitor::Stop() {
    if (context) {
        interception_destroy_context(context);
        context = nullptr;
    }
    isRunning = false;
}

void PhysicalKeyboardMonitor::RunLoop() {
    InterceptionStroke stroke;
    while (isRunning) {
        device = interception_wait(context);
        while (isRunning && interception_receive(context, device, &stroke, 1) > 0) {
            InterceptionKeyStroke& keystroke = reinterpret_cast<InterceptionKeyStroke&>(stroke);
            UpdateKeyState(keystroke);
            interception_send(context, device, &stroke, 1);
        }
    }
}

void PhysicalKeyboardMonitor::UpdateKeyState(InterceptionKeyStroke& stroke) {
    bool isKeyDown = !(stroke.state & INTERCEPTION_KEY_UP);
    DWORD vkCode = KeyCodeConverter::ScanCodeToVK(stroke.code);

    if (vkCode != 0) {
        switch (vkCode) {
            case VK_LSHIFT: physicalKeyStates.leftShift = isKeyDown; break;
            case VK_RSHIFT: physicalKeyStates.rightShift = isKeyDown; break;
            case VK_LCONTROL: physicalKeyStates.leftCtrl = isKeyDown; break;
            case VK_RCONTROL: physicalKeyStates.rightCtrl = isKeyDown; break;
            case VK_LMENU: physicalKeyStates.leftAlt = isKeyDown; break;
            case VK_RMENU: physicalKeyStates.rightAlt = isKeyDown; break;
        }
        
        Logger::LogKeyEvent(vkCode, stroke.code, stroke.state, isKeyDown);
        Logger::LogKeyStates(physicalKeyStates);
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
