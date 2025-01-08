#pragma once
#include <windows.h>
#include "interception.h"

// 键盘扫描码定义
#define SCANCODE_LSHIFT    0x2A
#define SCANCODE_RSHIFT    0x36
#define SCANCODE_LCONTROL  0x1D
#define SCANCODE_RCONTROL  0xE01D  // E0 前缀表示扩展键
#define SCANCODE_LALT      0x38
#define SCANCODE_RALT      0xE038  // E0 前缀表示扩展键

class PhysicalKeyboardMonitor {
public:
    PhysicalKeyboardMonitor();
    ~PhysicalKeyboardMonitor();

    bool Start();
    void Stop();
    void RunLoop();

    struct PhysicalKeyStates {
        bool leftCtrl = false;
        bool rightCtrl = false;
        bool leftShift = false;
        bool rightShift = false;
        bool leftAlt = false;
        bool rightAlt = false;
    };

    const PhysicalKeyStates& GetKeyStates() const { return physicalKeyStates; }
    
    // 检查物理按键是否处于按下状态
    bool IsKeyDown(DWORD vkCode) const;
    
    // 扫描码转换为虚拟键码
    static DWORD ScanCodeToVK(DWORD scanCode);
    
    // 虚拟键码转换为扫描码
    static DWORD VKToScanCode(DWORD vkCode);

private:
    void PrintKeyStates() const;
    void UpdateKeyState(InterceptionKeyStroke& stroke);

    InterceptionContext context;
    InterceptionDevice device;
    PhysicalKeyStates physicalKeyStates;
    bool isRunning;
};
