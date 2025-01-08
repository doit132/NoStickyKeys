#pragma once
#include <windows.h>
#include <stdexcept>

namespace keyboard {

// 按键状态结构体
struct KeyStates {
    bool leftShift = false;
    bool rightShift = false;
    bool leftCtrl = false;
    bool rightCtrl = false;
    bool leftAlt = false;
    bool rightAlt = false;

    void reset() {
        leftShift = rightShift = leftCtrl = rightCtrl = leftAlt = rightAlt = false;
    }
};

// 按键码转换工具
class KeyCodeConverter {
public:
    static DWORD ScanCodeToVK(DWORD scanCode) {
        bool isExtended = (scanCode & 0xE000) == 0xE000;
        DWORD baseScanCode = scanCode & 0xFF;

        switch (baseScanCode) {
            case 0x2A: return VK_LSHIFT;
            case 0x36: return VK_RSHIFT;
            case 0x1D: return isExtended ? VK_RCONTROL : VK_LCONTROL;
            case 0x38: return isExtended ? VK_RMENU : VK_LMENU;
            default: return 0;
        }
    }

    static DWORD VKToScanCode(DWORD vkCode) {
        switch (vkCode) {
            case VK_LSHIFT: return 0x2A;
            case VK_RSHIFT: return 0x36;
            case VK_LCONTROL: return 0x1D;
            case VK_RCONTROL: return 0xE01D;
            case VK_LMENU: return 0x38;
            case VK_RMENU: return 0xE038;
            default: return 0;
        }
    }

    static const char* GetKeyName(DWORD vkCode) {
        switch (vkCode) {
            case VK_LSHIFT: return "Left Shift";
            case VK_RSHIFT: return "Right Shift";
            case VK_LCONTROL: return "Left Ctrl";
            case VK_RCONTROL: return "Right Ctrl";
            case VK_LMENU: return "Left Alt";
            case VK_RMENU: return "Right Alt";
            default: return "Unknown";
        }
    }
};

// 错误处理
class KeyboardError : public std::runtime_error {
public:
    explicit KeyboardError(const char* message) : std::runtime_error(message) {}
};

} // namespace keyboard
