#pragma once
#include <windows.h>
#include "interception.h"
#include "keyboard_common.h"

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
