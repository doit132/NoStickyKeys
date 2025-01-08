#pragma once
#include <windows.h>
#include <functional>
#include <unordered_map>
#include <chrono>

class VirtualKeyboardMonitor {
public:
    VirtualKeyboardMonitor();
    ~VirtualKeyboardMonitor();

    bool Start();
    void Stop();
    void RunMessageLoop();

    struct VirtualKeyStates {
        bool leftCtrl = false;
        bool rightCtrl = false;
        bool leftShift = false;
        bool rightShift = false;
        bool leftAlt = false;
        bool rightAlt = false;
    };

    const VirtualKeyStates& GetKeyStates() const { return virtualKeyStates; }
    
    // 添加强制释放按键的方法
    void ForceReleaseKey(DWORD vkCode);
    
    // 设置按键状态检查回调
    using KeyStateCheckCallback = std::function<bool(DWORD)>;
    void SetKeyStateChecker(KeyStateCheckCallback checker) {
        keyStateChecker = checker;
    }

    static const int KEY_CHECK_DELAY_MS = 50;  // 延迟检测时间

private:
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    void PrintKeyStates() const;
    void UpdateKeyState(DWORD vkCode, bool isKeyDown);
    void CheckDelayedKeys();  // 检查延迟的按键

    static VirtualKeyboardMonitor* instance;
    HHOOK keyboardHook;
    VirtualKeyStates virtualKeyStates;
    bool isRunning;
    KeyStateCheckCallback keyStateChecker;
    
    // 记录每个按键的按下时间
    std::unordered_map<DWORD, std::chrono::steady_clock::time_point> keyPressTime;
};
