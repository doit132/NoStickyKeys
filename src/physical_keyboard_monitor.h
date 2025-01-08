#pragma once
#include <windows.h>
#include "interception.h"
#include "keyboard_common.h"
#include "keyboard_logger.h"

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
    
    // 检查物理按键是否处于按下状态
    bool IsKeyDown(DWORD vkCode) const;

    const keyboard::KeyStates& GetKeyStates() const { return physicalKeyStates; }

private:
    void UpdateKeyState(InterceptionKeyStroke& stroke);

    InterceptionContext context;
    InterceptionDevice device;
    bool isRunning;
    keyboard::KeyStates physicalKeyStates;
};
